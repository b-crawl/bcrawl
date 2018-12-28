/**
 * @file
 * @brief Functions related to special abilities.
**/

#include "AppHdr.h"

#include "ability.h"

#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <sstream>

#include "abyss.h"
#include "acquire.h"
#include "areas.h"
#include "art-enum.h"
#include "branch.h"
#include "butcher.h"
#include "chardump.h"
#include "cleansing-flame-source-type.h"
#include "cloud.h"
#include "coordit.h"
#include "database.h"
#include "decks.h"
#include "delay.h"
#include "describe.h"
#include "directn.h"
#include "dungeon.h"
#include "evoke.h"
#include "exercise.h"
#include "fight.h"
#include "food.h"
#include "god-abil.h"
#include "god-companions.h"
#include "god-conduct.h"
#include "god-prayer.h"
#include "god-wrath.h"
#include "hints.h"
#include "invent.h"
#include "item-prop.h"
#include "items.h"
#include "item-status-flag-type.h"
#include "item-use.h"
#include "level-state-type.h"
#include "libutil.h"
#include "macro.h"
#include "maps.h"
#include "menu.h"
#include "message.h"
#include "mon-place.h"
#include "mutation.h"
#include "notes.h"
#include "options.h"
#include "output.h"
#include "player-stats.h"
#include "potion.h"
#include "prompt.h"
#include "religion.h"
#include "skills.h"
#include "spl-cast.h"
#include "spl-clouds.h"
#include "spl-damage.h"
#include "spl-goditem.h"
#include "spl-miscast.h"
#include "spl-other.h"
#include "spl-selfench.h"
#include "spl-summoning.h"
#include "spl-transloc.h"
#include "stairs.h"
#include "state.h"
#include "stepdown.h"
#include "stringutil.h"
#include "target.h"
#include "terrain.h"
#include "tilepick.h"
#include "transform.h"
#include "traps.h"
#include "uncancel.h"
#include "unicode.h"
#include "view.h"

#ifdef USE_TILE
# include "tiledef-icons.h"
#endif

enum class abflag
{
    none                = 0x00000000,
    breath              = 0x00000001, // ability uses DUR_BREATH_WEAPON
    delay               = 0x00000002, // ability has its own delay
    pain                = 0x00000004, // ability must hurt player (ie torment)
    piety               = 0x00000008, // ability has its own piety cost
    exhaustion          = 0x00000010, // fails if you.exhausted
    instant             = 0x00000020, // doesn't take time to use
                        //0x00000040,
                        //0x00000080,
    conf_ok             = 0x00000100, // can use even if confused
    rations             = 0x00000200, // ability requires 2 rations per target
    rations_or_piety    = 0x00000400, // ability requires 2 rations or piety
    variable_mp         = 0x00000800, // costs a variable amount of MP
                        //0x00001000,
                        //0x00002000,
                        //0x00004000,
                        //0x00008000,
                        //0x00010000,
                        //0x00020000,
    remove_curse_scroll = 0x00040000, // Uses ?rc
    skill_drain         = 0x00080000, // drains skill levels
    gold                = 0x00100000, // costs gold
    sacrifice           = 0x00200000, // sacrifice (Ru)
    hostile             = 0x00400000, // failure summons a hostile (Makhleb)
    starve_ok           = 0x00800000, // can use even if starving
    berserk_ok          = 0x01000000, // can use even if berserk
    card                = 0x02000000, // deck drawing (Nemelex)
};
DEF_BITFIELD(ability_flags, abflag);

struct generic_cost
{
    int base, add, rolls;

    generic_cost(int num)
        : base(num), add(num == 0 ? 0 : (num + 1) / 2 + 1), rolls(1)
    {
    }
    generic_cost(int num, int _add, int _rolls = 1)
        : base(num), add(_add), rolls(_rolls)
    {
    }
    static generic_cost fixed(int fixed)
    {
        return generic_cost(fixed, 0, 1);
    }
    static generic_cost range(int low, int high, int _rolls = 1)
    {
        return generic_cost(low, high - low + 1, _rolls);
    }

    int cost() const PURE;

    operator bool () const { return base > 0 || add > 0; }
};

struct scaling_cost
{
    int value;

    scaling_cost(int permille) : value(permille) {}

    static scaling_cost fixed(int fixed)
    {
        return scaling_cost(-fixed);
    }

    int cost(int max) const;

    operator bool () const { return value != 0; }
};

/// What affects the failure chance of the ability?
enum class fail_basis
{
    xl,
    evo,
    invo,
};

/**
 * What skill is used to determine the player's god's invocations' failure
 * chance?
 *
 * XXX: deduplicate this with the similar code for divine titles, etc
 * (skills.cc:skill_title_by_rank)
 *
 * IMPORTANT NOTE: functions that depend on this will be wrong if you aren't
 * currently worshipping a god that grants the given ability (e.g. in ?/A)!
 *
 * @return      The appropriate skill type; e.g. SK_INVOCATIONS.
 */
skill_type invo_skill(god_type god)
{
    switch (god)
    {
        case GOD_KIKUBAAQUDGHA:
            return SK_NECROMANCY;

#if TAG_MAJOR_VERSION == 34
        case GOD_PAKELLAS:
            return SK_EVOCATIONS;
#endif
        case GOD_ASHENZARI:
        case GOD_JIYVA:
        case GOD_GOZAG:
        case GOD_RU:
        case GOD_TROG:
        case GOD_WU_JIAN:
            return SK_NONE; // ugh
        default:
            return SK_INVOCATIONS;
    }
}

/// How to determine the odds of the ability failing?
struct failure_info
{
    /// what determines the variable portion of failure: e.g. xl, evo, invo
    fail_basis basis;
    /// base failure chance
    int base_chance;
    /// multiplier to skill/xl; subtracted from base fail chance
    int variable_fail_mult;
    /// denominator to piety; subtracted from base fail chance if invo
    int piety_fail_denom;

    /**
     * What's the chance of the ability failing if the player tries to use it
     * right now?
     *
     * See spl-cast.cc:_get_true_fail_rate() for details on what this 'chance'
     * actually means.
     *
     * @return  A failure chance; may be outside the 0-100 range.
     */
    int chance() const
    {
        switch (basis)
        {
        case fail_basis::xl:
            return base_chance - you.experience_level * variable_fail_mult;
        case fail_basis::evo:
            return base_chance - you.skill(SK_EVOCATIONS, variable_fail_mult);
        case fail_basis::invo:
        {
            const int sk_mod = invo_skill() == SK_NONE ? 0 :
                                 you.skill(invo_skill(), variable_fail_mult);
            const int piety_mod
                = piety_fail_denom ? you.piety / piety_fail_denom : 0;
            return base_chance - sk_mod - piety_mod;
        }
        default:
            die("unknown failure basis %d!", (int)basis);
        }
    }

    /// What skill governs the use of this ability, if any?
    skill_type skill() const
    {
        switch (basis)
        {
        case fail_basis::evo:
            return SK_EVOCATIONS;
        case fail_basis::invo:
            return invo_skill();
        case fail_basis::xl:
        default:
            return SK_NONE;
        }
    }
};

// Structure for representing an ability:
struct ability_def
{
    ability_type        ability;
    const char *        name;
    unsigned int        mp_cost;        // magic cost of ability
    scaling_cost        hp_cost;        // hit point cost of ability
    unsigned int        food_cost;      // + rand2avg(food_cost, 2)
    generic_cost        piety_cost;     // + random2((piety_cost + 1) / 2 + 1)
    failure_info        failure;        // calculator for failure odds
    ability_flags       flags;          // used for additional cost notices
};

static int _lookup_ability_slot(ability_type abil);
static spret_type _do_ability(const ability_def& abil, bool fail);
static void _pay_ability_costs(const ability_def& abil);
static int _scale_piety_cost(ability_type abil, int original_cost);

// The description screen was way out of date with the actual costs.
// This table puts all the information in one place... -- bwr
//
// The four numerical fields are: MP, HP, food, and piety.
// Note:  food_cost  = val + random2avg(val, 2)
//        piety_cost = val + random2((val + 1) / 2 + 1);
//        hp cost is in per-mil of maxhp (i.e. 20 = 2% of hp, rounded up)
static const ability_def Ability_List[] =
{
    // NON_ABILITY should always come first
    { ABIL_NON_ABILITY, "No ability", 0, 0, 0, 0, {}, abflag::none },
    { ABIL_SPIT_POISON, "Spit Poison",
        0, 0, 40, 0, {fail_basis::xl, 20, 1}, abflag::breath },

    { ABIL_BLINK, "Blink", 0, 50, 50, 0, {fail_basis::xl, -1}, abflag::none },
    // ^ failure special-cased

    { ABIL_BREATHE_FIRE, "Breathe Fire",
        0, 0, 125, 0, {fail_basis::xl, 30, 1}, abflag::breath },
    { ABIL_BREATHE_FROST, "Breathe Frost",
        0, 0, 125, 0, {fail_basis::xl, 30, 1}, abflag::breath },
    { ABIL_BREATHE_POISON, "Breathe Poison Gas",
      0, 0, 125, 0, {fail_basis::xl, 30, 1}, abflag::breath },
    { ABIL_BREATHE_MEPHITIC, "Breathe Noxious Fumes",
      0, 0, 125, 0, {fail_basis::xl, 30, 1}, abflag::breath },
    { ABIL_BREATHE_LIGHTNING, "Breathe Lightning",
      0, 0, 125, 0, {fail_basis::xl, 30, 1}, abflag::breath },
    { ABIL_BREATHE_POWER, "Breathe Dispelling Energy",
      0, 0, 125, 0, {fail_basis::xl, 30, 1}, abflag::breath },
    { ABIL_BREATHE_STEAM, "Breathe Steam",
      0, 0, 75, 0, {fail_basis::xl, 20, 1}, abflag::breath },
    { ABIL_TRAN_BAT, "Bat Form",
      2, 0, 0, 0, {}, abflag::none },

    { ABIL_BREATHE_ACID, "Breathe Acid",
      0, 0, 125, 0, {fail_basis::xl, 30, 1}, abflag::breath },

    { ABIL_FLY, "Fly", 3, 0, 100, 0, {fail_basis::xl, 42, 3}, abflag::none },
    { ABIL_STOP_FLYING, "Stop Flying", 0, 0, 0, 0, {}, abflag::starve_ok },
    { ABIL_DAMNATION, "Hurl Damnation",
        0, 150, 200, 0, {fail_basis::xl, 50, 1}, abflag::none },

    { ABIL_CANCEL_PPROJ, "Cancel Portal Projectile",
      0, 0, 0, 0, {}, abflag::instant | abflag::starve_ok },

    { ABIL_CANCEL_PIERCE, "Cancel Piercing Shot",
      0, 0, 0, 0, {}, abflag::instant },

    { ABIL_DIG, "Dig", 0, 0, 0, 0, {}, abflag::instant | abflag::starve_ok },
    { ABIL_SHAFT_SELF, "Shaft Self", 0, 0, 250, 0, {}, abflag::delay },

    { ABIL_HOP, "Hop", 0, 0, 0, 0, {}, abflag::none },

    // EVOKE abilities use Evocations and come from items.
    // Teleportation and Blink can also come from mutations
    // so we have to distinguish them (see above). The off items
    // below are labeled EVOKE because they only work now if the
    // player has an item with the evocable power (not just because
    // you used a wand, potion, or miscast effect). I didn't see
    // any reason to label them as "Evoke" in the text, they don't
    // use or train Evocations (the others do).  -- bwr
    { ABIL_EVOKE_BLINK, "Evoke Blink",
      1, 0, 50, 0, {fail_basis::evo, 40, 2}, abflag::none },
    { ABIL_EVOKE_BERSERK, "Evoke Berserk Rage",
      0, 0, 600, 0, {fail_basis::evo, 50, 2}, abflag::none },

    { ABIL_EVOKE_TURN_INVISIBLE, "Evoke Invisibility",
      2, 0, 250, 0, {fail_basis::evo, 60, 2}, abflag::none },
#if TAG_MAJOR_VERSION == 34
    { ABIL_EVOKE_TURN_VISIBLE, "Turn Visible",
      0, 0, 0, 0, {}, abflag::starve_ok },
#endif
    { ABIL_EVOKE_FLIGHT, "Evoke Flight",
      1, 0, 100, 0, {fail_basis::evo, 40, 2}, abflag::none },
    { ABIL_EVOKE_FOG, "Evoke Fog",
      2, 0, 250, 0, {fail_basis::evo, 50, 2}, abflag::none },
    { ABIL_EVOKE_RATSKIN, "Evoke Ratskin",
      3, 0, 200, 0, {fail_basis::evo, 50, 2}, abflag::none },
    { ABIL_EVOKE_THUNDER, "Evoke Thunderclouds",
      5, 0, 200, 0, {fail_basis::evo, 60, 2}, abflag::none },


    { ABIL_END_TRANSFORMATION, "End Transformation",
      0, 0, 0, 0, {}, abflag::starve_ok },

    // INVOCATIONS:
    // Zin
    { ABIL_ZIN_RECITE, "Recite",
      0, 0, 0, 0, {fail_basis::invo, 30, 6, 20}, abflag::none },
    { ABIL_ZIN_VITALISATION, "Vitalisation",
      2, 0, 0, 1, {fail_basis::invo, 40, 5, 20}, abflag::none },
    { ABIL_ZIN_IMPRISON, "Imprison",
      5, 0, 0, 4, {fail_basis::invo, 60, 5, 20}, abflag::none },
    { ABIL_ZIN_SANCTUARY, "Sanctuary",
      7, 0, 0, 15, {fail_basis::invo, 80, 4, 25}, abflag::none },
    { ABIL_ZIN_DONATE_GOLD, "Donate Gold",
      0, 0, 0, 0, {fail_basis::invo}, abflag::none },

    // The Shining One
    { ABIL_TSO_DIVINE_SHIELD, "Divine Shield",
      3, 0, 50, 2, {fail_basis::invo, 40, 5, 20}, abflag::none },
    { ABIL_TSO_CLEANSING_FLAME, "Cleansing Flame",
      5, 0, 100, 2, {fail_basis::invo, 70, 4, 25}, abflag::none },
    { ABIL_TSO_SUMMON_DIVINE_WARRIOR, "Summon Divine Warrior",
      8, 0, 150, 5, {fail_basis::invo, 80, 4, 25}, abflag::none },
    { ABIL_TSO_BLESS_WEAPON, "Brand Weapon With Holy Wrath", 0, 0, 0, 0,
      {fail_basis::invo}, abflag::none },

    // Kikubaaqudgha
    { ABIL_KIKU_RECEIVE_CORPSES, "Receive Corpses",
      3, 0, 200, 2, {fail_basis::invo, 40, 5, 20}, abflag::none },
    { ABIL_KIKU_TORMENT, "Torment",
      4, 0, 0, 8, {fail_basis::invo, 60, 5, 20}, abflag::none },
    { ABIL_KIKU_GIFT_NECRONOMICON, "Receive Necronomicon", 0, 0, 0, 0,
      {fail_basis::invo}, abflag::none },
    { ABIL_KIKU_BLESS_WEAPON, "Brand Weapon With Pain", 0, 0, 0, 0,
      {fail_basis::invo}, abflag::pain },

    // Yredelemnul
    { ABIL_YRED_INJURY_MIRROR, "Injury Mirror",
      0, 0, 0, 0, {fail_basis::invo, 40, 4, 20}, abflag::piety },
    { ABIL_YRED_ANIMATE_REMAINS, "Animate Remains",
      2, 0, 200, 0, {fail_basis::invo, 40, 4, 20}, abflag::none },
    { ABIL_YRED_RECALL_UNDEAD_SLAVES, "Recall Undead Slaves",
      2, 0, 0, 0, {fail_basis::invo, 50, 4, 20}, abflag::none },
    { ABIL_YRED_ANIMATE_DEAD, "Animate Dead",
      2, 0, 200, 0, {fail_basis::invo, 40, 4, 20}, abflag::none },
    { ABIL_YRED_DRAIN_LIFE, "Drain Life",
      6, 0, 200, 2, {fail_basis::invo, 60, 4, 25}, abflag::none },
    { ABIL_YRED_ENSLAVE_SOUL, "Enslave Soul",
      8, 0, 500, 4, {fail_basis::invo, 80, 4, 25}, abflag::none },

    // Okawaru
    { ABIL_OKAWARU_HEROISM, "Heroism",
      2, 0, 0, 1, {fail_basis::invo, 30, 6, 20}, abflag::none },
    { ABIL_OKAWARU_FINESSE, "Finesse",
      5, 0, 0, 3, {fail_basis::invo, 60, 4, 25}, abflag::none },

    // Makhleb
    { ABIL_MAKHLEB_MINOR_DESTRUCTION, "Minor Destruction",
      0, scaling_cost::fixed(1), 0, 0, {fail_basis::invo, 40, 5, 20}, abflag::none },
    { ABIL_MAKHLEB_LESSER_SERVANT_OF_MAKHLEB, "Lesser Servant of Makhleb",
      0, scaling_cost::fixed(4), 0, 2, {fail_basis::invo, 40, 5, 20}, abflag::hostile },
    { ABIL_MAKHLEB_MAJOR_DESTRUCTION, "Major Destruction",
      0, scaling_cost::fixed(6), 0, generic_cost::range(0, 1),
      {fail_basis::invo, 60, 4, 25}, abflag::none },
    { ABIL_MAKHLEB_GREATER_SERVANT_OF_MAKHLEB, "Greater Servant of Makhleb",
      0, scaling_cost::fixed(10), 0, 5,
      {fail_basis::invo, 90, 2, 5}, abflag::hostile },

    // Sif Muna
    { ABIL_SIF_MUNA_DIVINE_ENERGY, "Divine Energy",
      0, 0, 0, 0, {fail_basis::invo}, abflag::instant | abflag::starve_ok },
    { ABIL_SIF_MUNA_STOP_DIVINE_ENERGY, "Stop Divine Energy",
      0, 0, 0, 0, {fail_basis::invo}, abflag::instant | abflag::starve_ok },
    { ABIL_SIF_MUNA_FORGET_SPELL, "Forget Spell",
      0, 0, 0, 8, {fail_basis::invo}, abflag::none },
    { ABIL_SIF_MUNA_CHANNEL_ENERGY, "Channel Magic",
      0, 0, 200, 2, {fail_basis::invo, 60, 4, 25}, abflag::none },

    // Trog
    { ABIL_TROG_BERSERK, "Berserk",
      0, 0, 600, 0, {fail_basis::invo}, abflag::none },
    { ABIL_TROG_REGEN_MR, "Trog's Hand",
      0, 0, 200, 2, {fail_basis::invo, piety_breakpoint(2), 0, 1}, abflag::none },
    { ABIL_TROG_BROTHERS_IN_ARMS, "Brothers in Arms",
      0, 0, 250, generic_cost::range(5, 6),
      {fail_basis::invo, piety_breakpoint(5), 0, 1}, abflag::none },

    // Elyvilon
    { ABIL_ELYVILON_LIFESAVING, "Divine Protection",
      0, 0, 0, 0, {fail_basis::invo}, abflag::piety },
    { ABIL_ELYVILON_LESSER_HEALING, "Lesser Healing", 2, 0, 100,
      generic_cost::fixed(1), {fail_basis::invo, 30, 6, 20}, abflag::none },
    { ABIL_ELYVILON_HEAL_OTHER, "Heal Other",
      2, 0, 250, 2, {fail_basis::invo, 40, 5, 20}, abflag::none },
    { ABIL_ELYVILON_PURIFICATION, "Purification",
      3, 0, 300, 3, {fail_basis::invo, 20, 5, 20}, abflag::conf_ok },
    { ABIL_ELYVILON_GREATER_HEALING, "Greater Healing",
      4, 0, 250, 3, {fail_basis::invo, 40, 5, 20}, abflag::none },
    { ABIL_ELYVILON_DIVINE_VIGOUR, "Divine Vigour",
      0, 0, 600, 6, {fail_basis::invo, 80, 4, 25}, abflag::none },

    // Lugonu
    { ABIL_LUGONU_ABYSS_EXIT, "Depart the Abyss",
      1, 0, 0, 10, {fail_basis::invo, 30, 6, 20}, abflag::none },
    { ABIL_LUGONU_BEND_SPACE, "Bend Space",
      1, scaling_cost::fixed(2), 0, 0, {fail_basis::invo, 40, 5, 20}, abflag::none },
    { ABIL_LUGONU_BANISH, "Banish", 4, 0, 200, generic_cost::range(3, 4),
      {fail_basis::invo, 85, 7, 20}, abflag::none },
    { ABIL_LUGONU_CORRUPT, "Corrupt", 7, scaling_cost::fixed(5), 500, 10,
      {fail_basis::invo, 70, 4, 25}, abflag::none },
    { ABIL_LUGONU_ABYSS_ENTER, "Enter the Abyss", 10, 0, 500, 28,
      {fail_basis::invo, 80, 4, 25}, abflag::pain },
    { ABIL_LUGONU_BLESS_WEAPON, "Brand Weapon With Distortion", 0, 0, 0, 0,
      {fail_basis::invo}, abflag::none },

    // Nemelex
    { ABIL_NEMELEX_DRAW_DESTRUCTION, "Draw Destruction",
      0, 0, 0, 0, {}, abflag::card },
    { ABIL_NEMELEX_DRAW_ESCAPE, "Draw Escape",
      0, 0, 0, 0, {}, abflag::card },
    { ABIL_NEMELEX_DRAW_SUMMONING, "Draw Summoning",
      0, 0, 0, 0, {}, abflag::card },
    { ABIL_NEMELEX_DRAW_STACK, "Draw Stack",
      0, 0, 0, 0, {}, abflag::card },
    { ABIL_NEMELEX_TRIPLE_DRAW, "Triple Draw",
      3, 0, 0, 0, {fail_basis::invo, 60, 5, 20}, abflag::none },
    { ABIL_NEMELEX_DEAL_FOUR, "Deal Four",
      4, 0, 0, 6, {fail_basis::invo, -1}, abflag::none }, // failure special-cased
    { ABIL_NEMELEX_STACK_FIVE, "Stack Five",
      5, 0, 0, 10, {fail_basis::invo, 80, 4, 25}, abflag::none },

    // Beogh
    { ABIL_BEOGH_SMITING, "Smiting",
      3, 0, 0, generic_cost::fixed(3), {fail_basis::invo, 40, 5, 20}, abflag::none },
    { ABIL_BEOGH_RECALL_ORCISH_FOLLOWERS, "Recall Orcish Followers",
      2, 0, 0, 0, {fail_basis::invo, 30, 6, 20}, abflag::none },
    { ABIL_BEOGH_GIFT_ITEM, "Give Item to Named Follower",
      0, 0, 0, 0, {fail_basis::invo}, abflag::none },
    { ABIL_BEOGH_RESURRECTION, "Resurrection",
      0, 0, 0, 28, {fail_basis::invo}, abflag::none },

    // Jiyva
    { ABIL_JIYVA_CALL_JELLY, "Request Jelly",
      2, 0, 0, 1, {fail_basis::invo}, abflag::none },
    { ABIL_JIYVA_SLIMIFY, "Slimify",
      4, 0, 0, 8, {fail_basis::invo, 90, 0, 2}, abflag::none },
    { ABIL_JIYVA_CURE_BAD_MUTATION, "Cure Bad Mutation",
      0, 0, 0, 15, {fail_basis::invo}, abflag::none },

    // Fedhas
    { ABIL_FEDHAS_FUNGAL_BLOOM, "Fungal Bloom",
      0, 0, 0, 0, {fail_basis::invo}, abflag::none },
    { ABIL_FEDHAS_SUNLIGHT, "Sunlight",
      2, 0, 50, 1, {fail_basis::invo, 30, 6, 20}, abflag::none },
    { ABIL_FEDHAS_EVOLUTION, "Evolution",
      2, 0, 0, 0, {fail_basis::invo, 30, 6, 20}, abflag::piety },
    { ABIL_FEDHAS_PLANT_RING, "Growth",
      2, 0, 100, 4, {fail_basis::invo, 40, 5, 20}, abflag::none },
    { ABIL_FEDHAS_SPAWN_SPORES, "Reproduction",
      4, 0, 100, 1, {fail_basis::invo, 60, 4, 25}, abflag::none },
    { ABIL_FEDHAS_RAIN, "Rain",
      4, 0, 150, 4, {fail_basis::invo, 70, 4, 25}, abflag::none },

    // Cheibriados
    { ABIL_CHEIBRIADOS_TIME_BEND, "Bend Time",
      3, 0, 50, 1, {fail_basis::invo, 40, 4, 20}, abflag::none },
    { ABIL_CHEIBRIADOS_DISTORTION, "Temporal Distortion",
      4, 0, 200, 3, {fail_basis::invo, 60, 5, 20}, abflag::instant },
    { ABIL_CHEIBRIADOS_SLOUCH, "Slouch",
      5, 0, 100, 8, {fail_basis::invo, 60, 4, 25}, abflag::none },
    { ABIL_CHEIBRIADOS_TIME_STEP, "Step From Time",
      10, 0, 200, 10, {fail_basis::invo, 80, 4, 25}, abflag::none },

    // Ashenzari
    { ABIL_ASHENZARI_CURSE, "Curse Item",
      0, 0, 0, 0, {fail_basis::invo}, abflag::none },
    { ABIL_ASHENZARI_SCRYING, "Scrying",
      4, 0, 0, 2, {fail_basis::invo}, abflag::instant },
    { ABIL_ASHENZARI_TRANSFER_KNOWLEDGE, "Transfer Knowledge",
      0, 0, 0, 10, {fail_basis::invo}, abflag::none },
    { ABIL_ASHENZARI_END_TRANSFER, "End Transfer Knowledge",
      0, 0, 0, 0, {fail_basis::invo}, abflag::starve_ok },

    // Dithmenos
    { ABIL_DITHMENOS_SHADOW_STEP, "Shadow Step",
      0, 0, 0, generic_cost::fixed(1),
      {fail_basis::invo, 30, 6, 20}, abflag::exhaustion },
    { ABIL_DITHMENOS_SHADOW_FORM, "Shadow Form",
      9, 0, 0, 12, {fail_basis::invo, 80, 4, 25}, abflag::skill_drain },

    // Ru
    { ABIL_RU_DRAW_OUT_POWER, "Draw Out Power", 0, 0, 0, 0,
      {fail_basis::invo}, abflag::exhaustion|abflag::skill_drain|abflag::conf_ok },
    { ABIL_RU_POWER_LEAP, "Power Leap",
      5, 0, 0, 0, {fail_basis::invo}, abflag::exhaustion },
    { ABIL_RU_APOCALYPSE, "Apocalypse",
      8, 0, 0, 0, {fail_basis::invo}, abflag::exhaustion|abflag::skill_drain },

    { ABIL_RU_SACRIFICE_PURITY, "Sacrifice Purity",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_WORDS, "Sacrifice Words",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_DRINK, "Sacrifice Drink",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_ESSENCE, "Sacrifice Essence",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_HEALTH, "Sacrifice Health",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_STEALTH, "Sacrifice Stealth",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_ARTIFICE, "Sacrifice Artifice",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_LOVE, "Sacrifice Love",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_COURAGE, "Sacrifice Courage",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_ARCANA, "Sacrifice Arcana",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_NIMBLENESS, "Sacrifice Nimbleness",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_DURABILITY, "Sacrifice Durability",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_HAND, "Sacrifice a Hand",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_EXPERIENCE, "Sacrifice Experience",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_SKILL, "Sacrifice Skill",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_EYE, "Sacrifice an Eye",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_SACRIFICE_RESISTANCE, "Sacrifice Resistance",
      0, 0, 0, 0, {fail_basis::invo}, abflag::sacrifice },
    { ABIL_RU_REJECT_SACRIFICES, "Reject Sacrifices",
      0, 0, 0, 0, {fail_basis::invo}, abflag::none },

    // Gozag
    { ABIL_GOZAG_POTION_PETITION, "Potion Petition",
      0, 0, 0, 0, {fail_basis::invo}, abflag::gold },
    { ABIL_GOZAG_CALL_MERCHANT, "Call Merchant",
      0, 0, 0, 0, {fail_basis::invo}, abflag::gold|abflag::starve_ok },
    { ABIL_GOZAG_BRIBE_BRANCH, "Bribe Branch",
      0, 0, 0, 0, {fail_basis::invo}, abflag::gold },

    // Qazlal
    { ABIL_QAZLAL_UPHEAVAL, "Upheaval",
      4, 0, 0, 1, {fail_basis::invo, 40, 5, 20}, abflag::none },
    { ABIL_QAZLAL_ELEMENTAL_FORCE, "Elemental Force",
      6, 0, 0, 6, {fail_basis::invo, 60, 5, 20}, abflag::none },
    { ABIL_QAZLAL_DISASTER_AREA, "Disaster Area",
      7, 0, 0, 10, {fail_basis::invo, 70, 4, 25}, abflag::none },

#if TAG_MAJOR_VERSION == 34
    // Pakellas
    { ABIL_PAKELLAS_DEVICE_SURGE, "Device Surge",
      0, 0, 0, generic_cost::fixed(1),
      {fail_basis::invo, 40, 5, 20}, abflag::variable_mp | abflag::instant },
#endif

    // Uskayaw
    { ABIL_USKAYAW_STOMP, "Stomp",
        3, 0, 100, generic_cost::fixed(20), {fail_basis::invo}, abflag::none },
    { ABIL_USKAYAW_LINE_PASS, "Line Pass",
        4, 0, 200, generic_cost::fixed(20), {fail_basis::invo}, abflag::none},
    { ABIL_USKAYAW_GRAND_FINALE, "Grand Finale",
        8, 0, 500, generic_cost::fixed(0),
        {fail_basis::invo, 120 + piety_breakpoint(4), 5, 1}, abflag::none},

    // Hepliaklqana
    { ABIL_HEPLIAKLQANA_RECALL, "Recall Ancestor",
        2, 0, 0, 0, {fail_basis::invo}, abflag::none },
    { ABIL_HEPLIAKLQANA_TRANSFERENCE, "Transference",
        2, 0, 0, 3, {fail_basis::invo, 40, 5, 20},
        abflag::none },
    { ABIL_HEPLIAKLQANA_IDEALISE, "Idealise",
        4, 0, 0, 4, {fail_basis::invo, 60, 4, 25},
        abflag::none },

    { ABIL_HEPLIAKLQANA_TYPE_KNIGHT,       "Ancestor Life: Knight",
        0, 0, 0, 0, {fail_basis::invo}, abflag::starve_ok },
    { ABIL_HEPLIAKLQANA_TYPE_BATTLEMAGE,   "Ancestor Life: Battlemage",
        0, 0, 0, 0, {fail_basis::invo}, abflag::starve_ok },
    { ABIL_HEPLIAKLQANA_TYPE_HEXER,        "Ancestor Life: Hexer",
        0, 0, 0, 0, {fail_basis::invo}, abflag::starve_ok },

    { ABIL_HEPLIAKLQANA_IDENTITY,  "Ancestor Identity",
        0, 0, 0, 0, {fail_basis::invo}, abflag::instant | abflag::starve_ok },

    // Wu Jian
    { ABIL_WU_JIAN_SERPENTS_LASH, "Serpent's Lash",
        0, 0, 0, 0, {fail_basis::invo}, abflag::exhaustion | abflag::instant
        | abflag::skill_drain },
    { ABIL_WU_JIAN_HEAVENLY_STORM, "Heavenly Storm",
        0, 0, 0, 6, {fail_basis::invo, piety_breakpoint(4), 0, 1}, abflag::none },
    // Lunge and Whirlwind abilities aren't menu abilities but currently need
    // to exist for action counting, hence need enums/entries.
    { ABIL_WU_JIAN_LUNGE, "Lunge", 0, 0, 0, 0, {}, abflag::berserk_ok },
    { ABIL_WU_JIAN_WHIRLWIND, "Whirlwind", 0, 0, 0, 0, {}, abflag::berserk_ok },
    { ABIL_WU_JIAN_WALLJUMP, "Wall Jump",
        0, 0, 0, 0, {}, abflag::starve_ok | abflag::berserk_ok },

    { ABIL_STOP_RECALL, "Stop Recall", 0, 0, 0, 0, {fail_basis::invo}, abflag::starve_ok },
    { ABIL_RENOUNCE_RELIGION, "Renounce Religion",
      0, 0, 0, 0, {fail_basis::invo}, abflag::starve_ok },
    { ABIL_CONVERT_TO_BEOGH, "Convert to Beogh",
      0, 0, 0, 0, {fail_basis::invo}, abflag::starve_ok },
};

static const ability_def& get_ability_def(ability_type abil)
{
    for (const ability_def &ab_def : Ability_List)
        if (ab_def.ability == abil)
            return ab_def;

    return Ability_List[0];
}

unsigned int ability_mp_cost(ability_type abil)
{
    return get_ability_def(abil).mp_cost;
}

/**
 * Is there a valid ability with a name matching that given?
 *
 * @param key   The name in question. (Not case sensitive.)
 * @return      true if such an ability exists; false if not.
 */
bool string_matches_ability_name(const string& key)
{
    return ability_by_name(key) != ABIL_NON_ABILITY;
}

/**
 * Find an ability whose name matches the given key.
 *
 * @param name      The name in question. (Not case sensitive.)
 * @return          The enum of the relevant ability, if there was one; else
 *                  ABIL_NON_ABILITY.
 */
ability_type ability_by_name(const string &key)
{
    for (const auto &abil : Ability_List)
    {
        if (abil.ability == ABIL_NON_ABILITY)
            continue;

        const string name = lowercase_string(ability_name(abil.ability));
        if (name == lowercase_string(key))
            return abil.ability;
    }

    return ABIL_NON_ABILITY;
}

string print_abilities()
{
    string text = "\n<w>a:</w> ";

    const vector<talent> talents = your_talents(false);

    if (talents.empty())
        text += "no special abilities";
    else
    {
        for (unsigned int i = 0; i < talents.size(); ++i)
        {
            if (i)
                text += ", ";
            text += ability_name(talents[i].which);
        }
    }

    return text;
}

int get_gold_cost(ability_type ability)
{
    switch (ability)
    {
    case ABIL_GOZAG_CALL_MERCHANT:
        return gozag_price_for_shop(true);
    case ABIL_GOZAG_POTION_PETITION:
        return gozag_potion_price();
    case ABIL_GOZAG_BRIBE_BRANCH:
        return GOZAG_BRIBE_AMOUNT;
    default:
        return 0;
    }
}

static string _nemelex_card_text(ability_type ability)
{
    int cards = deck_cards(ability_deck(ability));

    if (ability == ABIL_NEMELEX_DRAW_STACK)
        return make_stringf("(next: %s)", stack_top().c_str());
    else
        return make_stringf("(%d in deck)", cards);
}

const string make_cost_description(ability_type ability)
{
    const ability_def& abil = get_ability_def(ability);
    string ret;
    if (abil.mp_cost)
        ret += make_stringf(", %d MP", abil.mp_cost);

    if (abil.flags & abflag::variable_mp)
        ret += ", MP";

    if (abil.hp_cost)
        ret += make_stringf(", %d HP", abil.hp_cost.cost(you.hp_max));

    if (abil.food_cost && !you_foodless()
        && (you.undead_state() != US_SEMI_UNDEAD
            || you.hunger_state > HS_STARVING))
    {
        ret += ", Hunger"; // randomised and exact amount hidden from player
    }

    if (abil.piety_cost || abil.flags & abflag::piety)
        ret += ", Piety"; // randomised and exact amount hidden from player

    if (abil.flags & abflag::breath)
        ret += ", Breath";

    if (abil.flags & abflag::delay)
        ret += ", Delay";

    if (abil.flags & abflag::pain)
        ret += ", Pain";

    if (abil.flags & abflag::exhaustion)
        ret += ", Exhaustion";

    if (abil.flags & abflag::instant)
        ret += ", Instant"; // not really a cost, more of a bonus - bwr

    if (abil.flags & abflag::rations)
        ret += ", 2 Rations per target";

    if (abil.flags & abflag::rations_or_piety)
        ret += ", Piety or 2 Rations";

    if (abil.flags & abflag::skill_drain)
        ret += ", Drain";

    if (abil.flags & abflag::remove_curse_scroll)
        ret += ", Scroll of remove curse";

    if (abil.flags & abflag::gold)
    {
        const int amount = get_gold_cost(ability);
        if (amount)
            ret += make_stringf(", %d Gold", amount);
        else if (ability == ABIL_GOZAG_POTION_PETITION)
            ret += ", Free";
        else
            ret += ", Gold";
    }

    if (abil.flags & abflag::sacrifice)
    {
        ret += ", ";
        const string prefix = "Sacrifice ";
        ret += string(ability_name(ability)).substr(prefix.size());
        ret += ru_sac_text(ability);
    }

    if (abil.flags & abflag::card)
    {
        ret += ", ";
        ret += "A Card ";
        ret += _nemelex_card_text(ability);
    }

    // If we haven't output anything so far, then the effect has no cost
    if (ret.empty())
        return "None";

    ret.erase(0, 2);
    return ret;
}

static string _get_piety_amount_str(int value)
{
    return value <= 2 ? "very small" :
           value <= 4 ? "small" :
           value <= 8 ? "moderate" :
           value <= 12 ? "large" :
                        "very large";
}

static const string _detailed_cost_description(ability_type ability)
{
    const ability_def& abil = get_ability_def(ability);
    ostringstream ret;

    bool have_cost = false;
    ret << "This ability costs: ";

    if (abil.mp_cost > 0)
    {
        have_cost = true;
        ret << "\nMP     : ";
        ret << abil.mp_cost;
    }
    if (abil.hp_cost)
    {
        have_cost = true;
        ret << "\nHP     : ";
        ret << abil.hp_cost.cost(you.hp_max);
    }

    if (abil.food_cost && !you_foodless()
        && (you.undead_state() != US_SEMI_UNDEAD
            || you.hunger_state > HS_STARVING))
    {
        have_cost = true;
        ret << "\nHunger : ";
        ret << hunger_cost_string(abil.food_cost + abil.food_cost / 2);
    }

    if (abil.piety_cost || abil.flags & abflag::piety)
    {
        have_cost = true;
        ret << "\nPiety  : ";
        if (abil.flags & abflag::piety)
            ret << "variable";
        else
        {
            int avgcost = abil.piety_cost.base + abil.piety_cost.add / 2;
            ret << _get_piety_amount_str(avgcost);
        }
    }

    if (abil.flags & abflag::gold)
    {
        have_cost = true;
        ret << "\nGold   : ";
        int gold_amount = get_gold_cost(ability);
        if (gold_amount)
            ret << gold_amount;
        else if (ability == ABIL_GOZAG_POTION_PETITION)
            ret << "free";
        else
            ret << "variable";
    }

    if (abil.flags & abflag::rations_or_piety)
        ret << "\nPiety, or 2 rations";

    if (abil.flags & abflag::rations)
        ret << "\nRations: 2 per target";

    if (abil.flags & abflag::remove_curse_scroll)
    {
        have_cost = true;
        ret << "\nOne scroll of remove curse";
    }

    if (!have_cost)
        ret << "nothing.";

    if (abil.flags & abflag::breath)
        ret << "\nYou must catch your breath between uses of this ability.";

    if (abil.flags & abflag::delay)
        ret << "\nIt takes some time before being effective.";

    if (abil.flags & abflag::pain)
        ret << "\nUsing this ability will hurt you.";

    if (abil.flags & abflag::exhaustion)
        ret << "\nIt causes exhaustion, and cannot be used when exhausted.";

    if (abil.flags & abflag::instant)
        ret << "\nIt is instantaneous.";

    if (abil.flags & abflag::conf_ok)
        ret << "\nYou can use it even if confused.";

    if (abil.flags & abflag::skill_drain)
        ret << "\nIt will temporarily drain your skills when used.";

    return ret.str();
}

ability_type fixup_ability(ability_type ability)
{
    switch (ability)
    {
    case ABIL_YRED_ANIMATE_REMAINS:
        // suppress animate remains once animate dead is unlocked (ugh)
        if (in_good_standing(GOD_YREDELEMNUL, 2))
            return ABIL_NON_ABILITY;
        return ability;

    case ABIL_YRED_RECALL_UNDEAD_SLAVES:
    case ABIL_BEOGH_RECALL_ORCISH_FOLLOWERS:
        if (!you.recall_list.empty())
            return ABIL_STOP_RECALL;
        return ability;

    case ABIL_EVOKE_BERSERK:
    case ABIL_TROG_BERSERK:
        if (you.is_lifeless_undead(false)
            || you.species == SP_FORMICID)
        {
            return ABIL_NON_ABILITY;
        }
        return ability;

    case ABIL_BLINK:
    case ABIL_EVOKE_BLINK:
        if (you.species == SP_FORMICID)
            return ABIL_NON_ABILITY;
        else
            return ability;

    case ABIL_LUGONU_ABYSS_EXIT:
    case ABIL_LUGONU_ABYSS_ENTER:
        if (brdepth[BRANCH_ABYSS] == -1)
            return ABIL_NON_ABILITY;
        else
            return ability;

    case ABIL_TSO_BLESS_WEAPON:
    case ABIL_KIKU_BLESS_WEAPON:
    case ABIL_LUGONU_BLESS_WEAPON:
        if (you.species == SP_FELID)
            return ABIL_NON_ABILITY;
        else
            return ability;

    case ABIL_ELYVILON_HEAL_OTHER:
    case ABIL_TSO_SUMMON_DIVINE_WARRIOR:
    case ABIL_MAKHLEB_LESSER_SERVANT_OF_MAKHLEB:
    case ABIL_MAKHLEB_GREATER_SERVANT_OF_MAKHLEB:
    case ABIL_TROG_BROTHERS_IN_ARMS:
    case ABIL_GOZAG_BRIBE_BRANCH:
    case ABIL_QAZLAL_ELEMENTAL_FORCE:
        if (you.get_mutation_level(MUT_NO_LOVE))
            return ABIL_NON_ABILITY;
        else
            return ability;

    case ABIL_SIF_MUNA_DIVINE_ENERGY:
        if (you.attribute[ATTR_DIVINE_ENERGY])
            return ABIL_SIF_MUNA_STOP_DIVINE_ENERGY;
        return ability;

    case ABIL_ASHENZARI_TRANSFER_KNOWLEDGE:
        if (you.species == SP_GNOLL)
            return ABIL_NON_ABILITY;
        else
            return ability;

    default:
        return ability;
    }
}

/// Handle special cases for ability failure chances.
static int _adjusted_failure_chance(ability_type ability, int base_chance)
{
    switch (ability)
    {
    case ABIL_BREATHE_FIRE:
    case ABIL_BREATHE_FROST:
    case ABIL_BREATHE_ACID:
    case ABIL_BREATHE_LIGHTNING:
    case ABIL_BREATHE_POWER:
    case ABIL_BREATHE_MEPHITIC:
    case ABIL_BREATHE_STEAM:
        if (you.form == transformation::dragon)
            return base_chance - 20;
        return base_chance;

    case ABIL_BLINK:
        return 48 - (17 * you.get_mutation_level(MUT_BLINK))
                  - you.experience_level / 2;
        break;

    case ABIL_NEMELEX_DEAL_FOUR:
        return 70 - (you.piety * 2 / 45) - you.skill(SK_INVOCATIONS, 9) / 2;

    default:
        return base_chance;
    }
}

talent get_talent(ability_type ability, bool check_confused)
{
    ASSERT(ability != ABIL_NON_ABILITY);

    // Placeholder handling, part 1: The ability we have might be a
    // placeholder, so convert it into its corresponding ability before
    // doing anything else, so that we'll handle its flags properly.
    talent result { fixup_ability(ability), 0, 0, false };
    const ability_def &abil = get_ability_def(result.which);

    if (check_confused && you.confused()
        && !testbits(abil.flags, abflag::conf_ok))
    {
        result.which = ABIL_NON_ABILITY;
        return result;
    }

    // Look through the table to see if there's a preference, else find
    // a new empty slot for this ability. - bwr
    const int index = find_ability_slot(abil.ability);
    result.hotkey = index >= 0 ? index_to_letter(index) : 0;

    const int base_chance = abil.failure.chance();
    const int failure = _adjusted_failure_chance(ability, base_chance);
    result.fail = max(0, min(100, failure));

    result.is_invocation = abil.failure.basis == fail_basis::invo;

    return result;
}

const char* ability_name(ability_type ability)
{
    return get_ability_def(ability).name;
}

vector<const char*> get_ability_names()
{
    vector<const char*> result;
    for (const talent &tal : your_talents(false))
        result.push_back(ability_name(tal.which));
    return result;
}

static string _desc_sac_mut(const CrawlStoreValue &mut_store)
{
    return mut_upgrade_summary(static_cast<mutation_type>(mut_store.get_int()));
}

static string _sacrifice_desc(const ability_type ability)
{
    const string boilerplate =
        "\nIf you make this sacrifice, your powers granted by Ru "
        "will become stronger in proportion to the value of the "
        "sacrifice, and you may gain new powers as well.\n\n"
        "Sacrifices cannot be taken back.\n";
    const string piety_info = ru_sacrifice_description(ability);
    const string desc = boilerplate + piety_info;

    if (!you_worship(GOD_RU))
        return desc;

    const string sac_vec_key = ru_sacrifice_vector(ability);
    if (sac_vec_key.empty())
        return desc;

    ASSERT(you.props.exists(sac_vec_key));
    const CrawlVector &sacrifice_muts = you.props[sac_vec_key].get_vector();
    return "\nAfter this sacrifice, you will find that "
            + comma_separated_fn(sacrifice_muts.begin(), sacrifice_muts.end(),
                                 _desc_sac_mut)
            + ".\n" + desc;
}

static string _nemelex_desc(ability_type ability)
{
    ostringstream desc;
    deck_type deck = ability_deck(ability);

    desc << "Draw a card from " << (deck == DECK_STACK ? "your " : "the ");
    desc << deck_name(deck) << "; " << lowercase_first(deck_description(deck));

    return desc.str();
}

// XXX: should this be in describe.cc?
string get_ability_desc(const ability_type ability, bool need_title)
{
    const string& name = ability_name(ability);

    string lookup;

    if (testbits(get_ability_def(ability).flags, abflag::card))
        lookup = _nemelex_desc(ability);
    else
        lookup = getLongDescription(name + " ability");

    if (lookup.empty()) // Nothing found?
        lookup = "No description found.\n";

    if (testbits(get_ability_def(ability).flags, abflag::sacrifice))
        lookup += _sacrifice_desc(ability);

    if (god_hates_ability(ability, you.religion))
    {
        lookup += uppercase_first(god_name(you.religion))
                  + " frowns upon the use of this ability.\n";
    }

    ostringstream res;
    if (need_title)
        res << name << "\n\n";
    res << lookup << "\n" << _detailed_cost_description(ability);

    const string quote = getQuoteString(name + " ability");
    if (!quote.empty())
        res << "\n\n" << quote;

    return res.str();
}

static void _print_talent_description(const talent& tal)
{
    describe_ability(tal.which);
}

void no_ability_msg()
{
    // Give messages if the character cannot use innate talents right now.
    // * Vampires can't turn into bats when full of blood.
    // * Tengu can't start to fly if already flying.
    if (you.species == SP_VAMPIRE && you.experience_level >= 3)
    {
        if (you.transform_uncancellable)
            mpr("You can't untransform!");
        else
        {
            ASSERT(you.hunger_state > HS_SATIATED);
            mpr("Sorry, you're too full to transform right now.");
        }
    }
    else if (you.get_mutation_level(MUT_TENGU_FLIGHT)
             || you.get_mutation_level(MUT_BIG_WINGS)
             || you.get_mutation_level(MUT_FAIRY_FLIGHT))
    {
        if (you.airborne())
            mpr("You're already flying!");
    }
    else
        mpr("Sorry, you're not good enough to have a special ability.");
}

bool activate_ability()
{
    vector<talent> talents = your_talents(false);

    if (talents.empty())
    {
        no_ability_msg();
        crawl_state.zero_turns_taken();
        return false;
    }

    int selected = -1;
#ifndef TOUCH_UI
    if (Options.ability_menu)
#endif
    {
        selected = choose_ability_menu(talents);
        if (selected == -1)
        {
            canned_msg(MSG_OK);
            crawl_state.zero_turns_taken();
            return false;
        }
    }
#ifndef TOUCH_UI
    else
    {
        while (selected < 0)
        {
            msg::streams(MSGCH_PROMPT) << "Use which ability? (? or * to list) "
                                       << endl;

            const int keyin = get_ch();

            if (keyin == '?' || keyin == '*')
            {
                selected = choose_ability_menu(talents);
                if (selected == -1)
                {
                    canned_msg(MSG_OK);
                    crawl_state.zero_turns_taken();
                    return false;
                }
            }
            else if (key_is_escape(keyin) || keyin == ' ' || keyin == '\r'
                     || keyin == '\n')
            {
                canned_msg(MSG_OK);
                crawl_state.zero_turns_taken();
                return false;
            }
            else if (isaalpha(keyin))
            {
                // Try to find the hotkey.
                for (unsigned int i = 0; i < talents.size(); ++i)
                {
                    if (talents[i].hotkey == keyin)
                    {
                        selected = static_cast<int>(i);
                        break;
                    }
                }

                // If we can't, cancel out.
                if (selected < 0)
                {
                    mpr("You can't do that.");
                    crawl_state.zero_turns_taken();
                    return false;
                }
            }
        }
    }
#endif
    return activate_talent(talents[selected]);
}

static bool _can_hop(bool quiet)
{
    if (!you.duration[DUR_NO_HOP])
        return true;
    if (!quiet)
        mpr("Your legs are too worn out to hop.");
    return false;
}

// Check prerequisites for a number of abilities.
// Abort any attempt if these cannot be met, without losing the turn.
// TODO: Many more cases need to be added!
static bool _check_ability_possible(const ability_def& abil, bool quiet = false)
{
    if (you.berserk() && !testbits(abil.flags, abflag::berserk_ok))
    {
        if (!quiet)
            canned_msg(MSG_TOO_BERSERK);
        return false;
    }

    // Doing these would outright kill the player.
    // (or, in the case of the stat-zeros, they'd at least be extremely
    // dangerous.)
    if (abil.ability == ABIL_STOP_FLYING)
    {
        if (is_feat_dangerous(grd(you.pos()), false, true))
        {
            if (!quiet)
                mpr("Stopping flight right now would be fatal!");
            return false;
        }
    }
    else if (abil.ability == ABIL_END_TRANSFORMATION)
    {
        if (feat_dangerous_for_form(transformation::none, env.grid(you.pos())))
        {
            if (!quiet)
            {
                mprf("Turning back right now would cause you to %s!",
                    env.grid(you.pos()) == DNGN_LAVA ? "burn" : "drown");
            }

            return false;
        }
    }

    if ((abil.ability == ABIL_EVOKE_BERSERK
         || abil.ability == ABIL_TROG_BERSERK)
        && !you.can_go_berserk(true, false, quiet))
    {
        return false;
    }

    if ((abil.ability == ABIL_EVOKE_FLIGHT
         || abil.ability == ABIL_TRAN_BAT
         || abil.ability == ABIL_FLY)
        && !flight_allowed())
    {
        return false;
    }


    if (you.confused() && !testbits(abil.flags, abflag::conf_ok))
    {
        if (!quiet)
            canned_msg(MSG_TOO_CONFUSED);
        return false;
    }

    // Silence and water elementals
    if (silenced(you.pos())
        || you.duration[DUR_WATER_HOLD] && !you.res_water_drowning())
    {
        talent tal = get_talent(abil.ability, false);
        if (tal.is_invocation)
        {
            if (!quiet)
            {
                mprf("You cannot call out to %s while %s.",
                     god_name(you.religion).c_str(),
                     you.duration[DUR_WATER_HOLD] ? "unable to breathe"
                                                  : "silenced");
            }
            return false;
        }
    }

    if (!testbits(abil.flags, abflag::starve_ok)
        && apply_starvation_penalties())
    {
        if (!quiet)
            canned_msg(MSG_TOO_HUNGRY);
        return false;
    }

    // Don't insta-starve the player.
    // (Losing consciousness possible from 400 downward.)
    if (!testbits(abil.flags, abflag::starve_ok) && !you.undead_state())
    {
        const hunger_state_t state =
            static_cast<hunger_state_t>(max(0, you.hunger_state - 1));
        const int expected_hunger = hunger_threshold[state]
                                    - abil.food_cost * 2;
        if (!quiet)
        {
            dprf("hunger: %d, max. food_cost: %d, expected hunger: %d",
                 you.hunger, abil.food_cost * 2, expected_hunger);
        }
        // Safety margin for natural hunger, mutations etc.
        if (expected_hunger <= 50)
        {
            if (!quiet)
                canned_msg(MSG_TOO_HUNGRY);
            return false;
        }
    }

    const god_power* god_power = god_power_from_ability(abil.ability);
    if (god_power && !god_power_usable(*god_power))
    {
        if (!quiet)
            canned_msg(MSG_GOD_DECLINES);
        return false;
    }

    if (testbits(abil.flags, abflag::card) && !deck_cards(ability_deck(abil.ability)))
    {
        if (!quiet)
            mpr("That deck is empty!");
        return false;
    }

    vector<text_pattern> &actions = Options.confirm_action;
    if (!actions.empty())
    {
        const char* name = ability_name(abil.ability);
        for (const text_pattern &action : actions)
        {
            if (action.matches(name))
            {
                string prompt = "Really use " + string(name) + "?";
                if (!yesno(prompt.c_str(), false, 'n'))
                {
                    canned_msg(MSG_OK);
                    return false;
                }
                break;
            }
        }
    }

    // Check that we can afford to pay the costs.
    // Note that mutation shenanigans might leave us with negative MP,
    // so don't fail in that case if there's no MP cost.
    if (abil.mp_cost > 0 && !enough_mp(abil.mp_cost, quiet, true))
        return false;

    const int hpcost = abil.hp_cost.cost(you.hp_max);
    if (hpcost > 0 && !enough_hp(hpcost, quiet))
        return false;

    switch (abil.ability)
    {
    case ABIL_ZIN_RECITE:
    {
        if (!zin_check_able_to_recite(quiet))
            return false;

        int result = zin_check_recite_to_monsters(quiet);
        if (result != 1)
        {
            if (!quiet)
            {
                if (result == 0)
                    mpr("There's no appreciative audience!");
                else if (result == -1)
                    mpr("You are not zealous enough to affect this audience!");
            }
            return false;
        }
        return true;
    }

    case ABIL_ZIN_SANCTUARY:
        if (env.sanctuary_time)
        {
            if (!quiet)
                mpr("There's already a sanctuary in place on this level.");
            return false;
        }
        return true;

    case ABIL_ZIN_DONATE_GOLD:
        if (!you.gold)
        {
            if (!quiet)
                mpr("You have nothing to donate!");
            return false;
        }
        return true;

    case ABIL_ELYVILON_PURIFICATION:
        if (!you.disease && !you.duration[DUR_POISONING]
            && !you.duration[DUR_CONF] && !you.duration[DUR_SLOW]
            && !you.petrifying()
            && you.strength(false) == you.max_strength()
            && you.intel(false) == you.max_intel()
            && you.dex(false) == you.max_dex()
            && !player_rotted()
            && !you.duration[DUR_WEAK])
        {
            if (!quiet)
                mpr("Nothing ails you!");
            return false;
        }
        return true;

    case ABIL_LUGONU_ABYSS_EXIT:
        if (!player_in_branch(BRANCH_ABYSS))
        {
            if (!quiet)
                mpr("You aren't in the Abyss!");
            return false;
        }
        return true;

    case ABIL_LUGONU_CORRUPT:
        return !is_level_incorruptible(quiet);

    case ABIL_LUGONU_ABYSS_ENTER:
        if (player_in_branch(BRANCH_ABYSS))
        {
            if (!quiet)
                mpr("You're already here!");
            return false;
        }
        return true;

    case ABIL_SIF_MUNA_FORGET_SPELL:
        if (you.spell_no == 0)
        {
            if (!quiet)
                canned_msg(MSG_NO_SPELLS);
            return false;
        }
        return true;

    case ABIL_ASHENZARI_TRANSFER_KNOWLEDGE:
        if (!trainable_skills(true))
        {
            if (!quiet)
                mpr("You have nothing more to learn.");
            return false;
        }
        return true;

    case ABIL_FEDHAS_EVOLUTION:
        return fedhas_check_evolve_flora(quiet);

    case ABIL_FEDHAS_SPAWN_SPORES:
    {
        const int retval = fedhas_check_corpse_spores(quiet);
        if (retval <= 0)
        {
            if (!quiet)
            {
                if (retval == 0)
                    mpr("No corpses are in range.");
                else
                    canned_msg(MSG_OK);
            }
            return false;
        }
        return true;
    }

    case ABIL_SPIT_POISON:
    case ABIL_BREATHE_FIRE:
    case ABIL_BREATHE_FROST:
    case ABIL_BREATHE_POISON:
    case ABIL_BREATHE_LIGHTNING:
    case ABIL_BREATHE_ACID:
    case ABIL_BREATHE_POWER:
    case ABIL_BREATHE_STEAM:
    case ABIL_BREATHE_MEPHITIC:
        if (you.duration[DUR_BREATH_WEAPON])
        {
            if (!quiet)
                canned_msg(MSG_CANNOT_DO_YET);
            return false;
        }
        return true;

    case ABIL_SHAFT_SELF:
        return you.can_do_shaft_ability(quiet);

    case ABIL_HOP:
        return _can_hop(quiet);

    case ABIL_BLINK:
    case ABIL_EVOKE_BLINK:
    {
        const string no_tele_reason = you.no_tele_reason(false, true);
        if (no_tele_reason.empty())
            return true;

        if (!quiet)
             mpr(no_tele_reason);
        return false;
    }

    case ABIL_EVOKE_BERSERK:
    case ABIL_TROG_BERSERK:
        return you.can_go_berserk(true, false, true)
               && (quiet || berserk_check_wielded_weapon());

    case ABIL_EVOKE_FOG:
        if (cloud_at(you.pos()))
        {
            if (!quiet)
                mpr("It's too cloudy to do that here.");
            return false;
        }
        if (env.level_state & LSTATE_STILL_WINDS)
        {
            if (!quiet)
                mpr("The air is too still for clouds to form.");
            return false;
        }
        return true;

    case ABIL_GOZAG_POTION_PETITION:
        return gozag_setup_potion_petition(quiet);

    case ABIL_GOZAG_CALL_MERCHANT:
        return gozag_setup_call_merchant(quiet);

    case ABIL_GOZAG_BRIBE_BRANCH:
        return gozag_check_bribe_branch(quiet);

    case ABIL_RU_SACRIFICE_EXPERIENCE:
        if (you.experience_level <= RU_SAC_XP_LEVELS)
        {
            if (!quiet)
                mpr("You don't have enough experience to sacrifice.");
            return false;
        }
        return true;

#if TAG_MAJOR_VERSION == 34
    case ABIL_PAKELLAS_DEVICE_SURGE:
        if (you.magic_points == 0)
        {
            if (!quiet)
                mpr("You have no magic power.");
            return false;
        }
        return true;
#endif

        // only available while your ancestor is alive.
    case ABIL_HEPLIAKLQANA_IDEALISE:
    case ABIL_HEPLIAKLQANA_RECALL:
    case ABIL_HEPLIAKLQANA_TRANSFERENCE:
        if (hepliaklqana_ancestor() == MID_NOBODY)
        {
            if (!quiet)
            {
                mprf("%s is still trapped in memory!",
                     hepliaklqana_ally_name().c_str());
            }
            return false;
        }
        return true;

    case ABIL_WU_JIAN_WALLJUMP:
    {
        // TODO: Add check for whether there is any valid landing spot
        if (you.is_nervous())
        {
            if (!quiet)
                mpr("You are too terrified to wall jump!");
            return false;
        }
        if (you.attribute[ATTR_HELD])
        {
            if (!quiet)
            {
                mprf("You cannot wall jump while caught in a %s.",
                     get_trapping_net(you.pos()) == NON_ITEM ? "web" : "net");
            }
            return false;
        }
        // Is there a valid place to wall jump?
        bool has_targets = false;
        for (adjacent_iterator ai(you.pos()); ai; ++ai)
            if (feat_can_wall_jump_against(grd(*ai)))
            {
                has_targets = true;
                break;
            }

        if (!has_targets)
        {
            if (!quiet)
                mpr("There is nothing to wall jump against here.");
            return false;
        }
        return true;
    }

    default:
        return true;
    }
}

static bool _check_ability_dangerous(const ability_type ability,
                                     bool quiet = false)
{
    if (ability == ABIL_TRAN_BAT)
        return !check_form_stat_safety(transformation::bat, quiet);
    else if (ability == ABIL_END_TRANSFORMATION
             && !feat_dangerous_for_form(transformation::none,
                                         env.grid(you.pos())))
    {
        return !check_form_stat_safety(transformation::bat, quiet);
    }
    else
        return false;
}

bool check_ability_possible(const ability_type ability, bool quiet)
{
    return _check_ability_possible(get_ability_def(ability), quiet);
}

bool activate_talent(const talent& tal)
{
    const ability_def& abil = get_ability_def(tal.which);

    if (_check_ability_dangerous(abil.ability) || !_check_ability_possible(abil))
    {
        crawl_state.zero_turns_taken();
        return false;
    }

    bool fail = random2avg(100, 3) < tal.fail;

    const spret_type ability_result = _do_ability(abil, fail);
    switch (ability_result)
    {
        case SPRET_SUCCESS:
            ASSERT(!fail || testbits(abil.flags, abflag::hostile));
            practise_using_ability(abil.ability);
            _pay_ability_costs(abil);
            count_action(tal.is_invocation ? CACT_INVOKE : CACT_ABIL, abil.ability);
            return true;
        case SPRET_FAIL:
            mpr("You fail to use your ability.");
            you.turn_is_over = true;
            return false;
        case SPRET_ABORT:
            crawl_state.zero_turns_taken();
            return false;
        case SPRET_NONE:
        default:
            die("Weird ability return type");
            return false;
    }
}

static int _calc_breath_ability_range(ability_type ability)
{
    int range = 0;

    switch (ability)
    {
    case ABIL_BREATHE_ACID:
        range = 3;
        break;
    case ABIL_BREATHE_FIRE:
    case ABIL_BREATHE_FROST:
    case ABIL_SPIT_POISON:
        range = 5;
        break;
    case ABIL_BREATHE_MEPHITIC:
    case ABIL_BREATHE_STEAM:
    case ABIL_BREATHE_POISON:
        range = 6;
        break;
    case ABIL_BREATHE_LIGHTNING:
    case ABIL_BREATHE_POWER:
        range = LOS_MAX_RANGE;
        break;
    default:
        die("Bad breath type!");
        break;
    }

    return min((int)you.current_vision, range);
}

static bool _acid_breath_can_hit(const actor *act)
{
    if (act->is_monster())
    {
        const monster* mons = act->as_monster();
        bolt testbeam;
        testbeam.thrower = KILL_YOU;
        zappy(ZAP_BREATHE_ACID, 100, false, testbeam);

        return !testbeam.ignores_monster(mons);
    }
    else
        return false;
}

/// If the player is stationary, print 'You cannot move.' and return true.
static bool _abort_if_stationary()
{
    if (!you.is_stationary())
        return false;

    canned_msg(MSG_CANNOT_MOVE);
    return true;
}

static bool _cleansing_flame_affects(const actor *act)
{
    return act->res_holy_energy() < 3;
}

/*
 * Use an ability.
 *
 * @param abil The actual ability used.
 * @param fail If true, the ability is doomed to fail, and SPRET_FAIL will
 * be returned if the ability is not SPRET_ABORTed.
 * @returns Whether the spell succeeded (SPRET_SUCCESS), failed (SPRET_FAIL),
 *  or was canceled (SPRET_ABORT). Never returns SPRET_NONE.
 */
static spret_type _do_ability(const ability_def& abil, bool fail)
{
    dist abild;
    bolt beam;
    dist spd;

    // Note: the costs will not be applied until after this switch
    // statement... it's assumed that only failures have returned! - bwr
    switch (abil.ability)
    {
    case ABIL_DIG:
        fail_check();
        if (!you.digging)
        {
            you.digging = true;
            mpr("You extend your mandibles.");
        }
        else
        {
            you.digging = false;
            mpr("You retract your mandibles.");
        }
        break;

    case ABIL_SHAFT_SELF:
        fail_check();
        if (you.can_do_shaft_ability(false))
        {
            if (yesno("Are you sure you want to shaft yourself?", true, 'n'))
                start_delay<ShaftSelfDelay>(1);
            else
                return SPRET_ABORT;
        }
        else
            return SPRET_ABORT;
        break;

    case ABIL_HOP:
        if (_can_hop(false))
            return frog_hop(fail);
        else
            return SPRET_ABORT;

    case ABIL_SPIT_POISON:      // Naga poison spit
    {
        int power = 10 + you.experience_level;
        beam.range = _calc_breath_ability_range(abil.ability);

        if (!spell_direction(abild, beam)
            || !player_tracer(ZAP_SPIT_POISON, power, beam))
        {
            return SPRET_ABORT;
        }
        else
        {
            fail_check();
            zapping(ZAP_SPIT_POISON, power, beam);
            you.set_duration(DUR_BREATH_WEAPON, 3 + random2(5));
        }
        break;
    }

    case ABIL_BREATHE_ACID:       // Draconian acid splash
    {
        beam.range = _calc_breath_ability_range(abil.ability);
        targeter_splash hitfunc(&you, beam.range);
        direction_chooser_args args;
        args.mode = TARG_HOSTILE;
        args.hitfunc = &hitfunc;
        if (!spell_direction(abild, beam, &args))
          return SPRET_ABORT;

        if (stop_attack_prompt(hitfunc, "spit at", _acid_breath_can_hit))
          return SPRET_ABORT;

        fail_check();
        zapping(ZAP_BREATHE_ACID, (you.form == transformation::dragon) ?
                2 * you.experience_level : you.experience_level,
                beam, false, "You spit a glob of acid.");

        you.increase_duration(DUR_BREATH_WEAPON,
                          3 + random2(10) + random2(30 - you.experience_level));
        break;
    }

    case ABIL_BREATHE_FIRE:
    case ABIL_BREATHE_FROST:
    case ABIL_BREATHE_POISON:
    case ABIL_BREATHE_POWER:
    case ABIL_BREATHE_STEAM:
    case ABIL_BREATHE_MEPHITIC:
        beam.range = _calc_breath_ability_range(abil.ability);
        if (!spell_direction(abild, beam))
            return SPRET_ABORT;

        // fallthrough to ABIL_BREATHE_LIGHTNING

    case ABIL_BREATHE_LIGHTNING: // not targeted
        fail_check();

        // TODO: refactor this to use only one call to zapping(), don't
        // duplicate its fail_check(), split out breathe_lightning, etc

        switch (abil.ability)
        {
        case ABIL_BREATHE_FIRE:
        {
            int power = you.experience_level;

            if (you.form == transformation::dragon)
                power += 12;

            string msg = "You breathe a blast of fire";
            msg += (power < 15) ? '.' : '!';

            if (!zapping(ZAP_BREATHE_FIRE, power, beam, true, msg.c_str()))
                return SPRET_ABORT;
            break;
        }

        case ABIL_BREATHE_FROST:
            if (!zapping(ZAP_BREATHE_FROST,
                 (you.form == transformation::dragon) ?
                     2 * you.experience_level : you.experience_level,
                 beam, true,
                         "You exhale a wave of freezing cold."))
            {
                return SPRET_ABORT;
            }
            break;

        case ABIL_BREATHE_POISON:
            if (!zapping(ZAP_BREATHE_POISON, you.experience_level, beam, true,
                         "You exhale a blast of poison gas."))
            {
                return SPRET_ABORT;
            }
            break;

        case ABIL_BREATHE_LIGHTNING:
            mpr("You breathe a wild blast of lightning!");
            black_drac_breath();
            break;

        case ABIL_BREATHE_ACID:
            if (!zapping(ZAP_BREATHE_ACID,
                (you.form == transformation::dragon) ?
                    2 * you.experience_level : you.experience_level,
                beam, true, "You spit a glob of acid."))
            {
                return SPRET_ABORT;
            }
            break;

        case ABIL_BREATHE_POWER:
            if (!zapping(ZAP_BREATHE_POWER,
                (you.form == transformation::dragon) ?
                    2 * you.experience_level : you.experience_level,
                beam, true,
                         "You breathe a bolt of dispelling energy."))
            {
                return SPRET_ABORT;
            }
            break;

        case ABIL_BREATHE_STEAM:
            if (!zapping(ZAP_BREATHE_STEAM,
                (you.form == transformation::dragon) ?
                    2 * you.experience_level : you.experience_level,
                beam, true,
                         "You exhale a blast of scalding steam."))
            {
                return SPRET_ABORT;
            }
            break;

        case ABIL_BREATHE_MEPHITIC:
            if (!zapping(ZAP_BREATHE_MEPHITIC,
                (you.form == transformation::dragon) ?
                    2 * you.experience_level : you.experience_level,
                beam, true,
                         "You exhale a blast of noxious fumes."))
            {
                return SPRET_ABORT;
            }
            break;

        default:
            break;
        }

        you.increase_duration(DUR_BREATH_WEAPON,
                      3 + random2(10) + random2(30 - you.experience_level));

        if (abil.ability == ABIL_BREATHE_STEAM)
            you.duration[DUR_BREATH_WEAPON] /= 2;

        break;

    case ABIL_EVOKE_BLINK:      // randarts
        fail_check();
        // deliberate fall-through
    case ABIL_BLINK:            // mutation
        return cast_blink(fail);
        break;

    case ABIL_EVOKE_BERSERK:    // amulet of rage, randarts
        fail_check();
        you.go_berserk(true);
        break;

    case ABIL_FLY:
        fail_check();
        // FD, Te, or Dr/Gr wings
        if (you.racial_permanent_flight())
        {
            you.attribute[ATTR_PERM_FLIGHT] = 1;
            float_player();
        }
        // low level Te
        if (you.species == SP_TENGU)
            mpr("You feel very comfortable in the air.");
        break;

    // DEMONIC POWERS:
    case ABIL_DAMNATION:
        fail_check();
        if (your_spells(SPELL_HURL_DAMNATION,
                        you.experience_level * 10,
                        false) == SPRET_ABORT)
        {
            return SPRET_ABORT;
        }
        break;

    case ABIL_EVOKE_TURN_INVISIBLE:     // cloaks, randarts
        if (!invis_allowed())
            return SPRET_ABORT;
        fail_check();
        surge_power(you.spec_evoke());
        potionlike_effect(POT_INVISIBILITY,
                          player_adjust_evoc_power(
                              you.skill(SK_EVOCATIONS, 2) + 5));
        contaminate_player(1000 + random2(2000), true);
        break;

#if TAG_MAJOR_VERSION == 34
    case ABIL_EVOKE_TURN_VISIBLE:
        fail_check();
        ASSERT(!you.attribute[ATTR_INVIS_UNCANCELLABLE]);
        mpr("You feel less transparent.");
        you.duration[DUR_INVIS] = 1;
        break;
#endif

    case ABIL_EVOKE_FLIGHT:             // ring, boots, randarts
        fail_check();
        ASSERT(!get_form()->forbids_flight());
        if (you.wearing_ego(EQ_ALL_ARMOUR, SPARM_FLYING))
        {
            bool standing = !you.airborne();
            you.attribute[ATTR_PERM_FLIGHT] = 1;
            if (standing)
                float_player();
            else
                mpr("You feel more buoyant.");
        }
        else
        {
            surge_power(you.spec_evoke());
            fly_player(
                player_adjust_evoc_power(you.skill(SK_EVOCATIONS, 2) + 30));
        }
        break;

    case ABIL_EVOKE_FOG:     // cloak of the Thief
        fail_check();
        mpr("With a swish of your cloak, you release a cloud of fog.");
        big_cloud(random_smoke_type(), &you, you.pos(), 50, 8 + random2(8));
        break;

    case ABIL_EVOKE_RATSKIN: // ratskin cloak
        fail_check();
        mpr("The rats of the Dungeon answer your call.");

        for (int i = 0; i < (coinflip() + 1); ++i)
        {
            monster_type mon = coinflip() ? MONS_HELL_RAT : MONS_RIVER_RAT;

            mgen_data mg(mon, BEH_FRIENDLY, you.pos(), MHITYOU);
            if (monster *m = create_monster(mg))
                m->add_ench(mon_enchant(ENCH_FAKE_ABJURATION, 3));
        }

        break;

    case ABIL_EVOKE_THUNDER: // robe of Clouds
        fail_check();
        mpr("The folds of your robe billow into a mighty storm.");

        for (radius_iterator ri(you.pos(), 2, C_SQUARE); ri; ++ri)
            if (!cell_is_solid(*ri))
                place_cloud(CLOUD_STORM, *ri, 8 + random2avg(8,2), &you);

        break;

    case ABIL_CANCEL_PPROJ:
        fail_check();
        you.duration[DUR_PORTAL_PROJECTILE] = 0;
        you.attribute[ATTR_PORTAL_PROJECTILE] = 0;
        mpr("You are no longer teleporting projectiles to their destination.");
        break;

    case ABIL_CANCEL_PIERCE:
        fail_check();
        you.duration[DUR_PIERCING_SHOT] = 0;
        mpr("Your projectiles no longer penetrate their targets.");
        break;

    case ABIL_STOP_FLYING:
        fail_check();
        you.duration[DUR_FLIGHT] = 0;
        you.attribute[ATTR_PERM_FLIGHT] = 0;
        land_player();
        break;

    case ABIL_END_TRANSFORMATION:
        fail_check();
        untransform();
        break;

    // INVOCATIONS:
    case ABIL_ZIN_RECITE:
    {
        fail_check();
        if (zin_check_recite_to_monsters() == 1)
        {
            you.attribute[ATTR_RECITE_TYPE] = (recite_type) random2(NUM_RECITE_TYPES); // This is just flavor
            you.attribute[ATTR_RECITE_SEED] = random2(2187); // 3^7
            you.duration[DUR_RECITE] = 3 * BASELINE_DELAY;
            mprf("You clear your throat and prepare to recite.");
            you.increase_duration(DUR_RECITE_COOLDOWN,
                                  3 + random2(10) + random2(30));
        }
        else
        {
            canned_msg(MSG_OK);
            return SPRET_ABORT;
        }
        break;
    }
    case ABIL_ZIN_VITALISATION:
        fail_check();
        zin_vitalisation();
        break;

    case ABIL_ZIN_IMPRISON:
    {
        beam.range = LOS_MAX_RANGE;
        direction_chooser_args args;
        args.restricts = DIR_TARGET;
        args.mode = TARG_HOSTILE;
        args.needs_path = false;
        if (!spell_direction(spd, beam, &args))
            return SPRET_ABORT;

        if (beam.target == you.pos())
        {
            mpr("You cannot imprison yourself!");
            return SPRET_ABORT;
        }

        monster* mons = monster_at(beam.target);

        if (mons == nullptr || !you.can_see(*mons))
        {
            mpr("There is no monster there to imprison!");
            return SPRET_ABORT;
        }

        if (mons_is_firewood(*mons) || mons_is_conjured(mons->type))
        {
            mpr("You cannot imprison that!");
            return SPRET_ABORT;
        }

        if (mons->friendly() || mons->good_neutral())
        {
            mpr("You cannot imprison a law-abiding creature!");
            return SPRET_ABORT;
        }

        fail_check();

        int power = 3 + (roll_dice(5, you.skill(SK_INVOCATIONS, 5) + 12) / 26);

        if (!cast_imprison(power, mons, -GOD_ZIN))
            return SPRET_ABORT;
        break;
    }

    case ABIL_ZIN_SANCTUARY:
        fail_check();
        zin_sanctuary();
        break;

    case ABIL_ZIN_DONATE_GOLD:
        fail_check();
        zin_donate_gold();
        break;

    case ABIL_TSO_DIVINE_SHIELD:
        fail_check();
        tso_divine_shield();
        break;

    case ABIL_TSO_CLEANSING_FLAME:
    {
        targeter_los hitfunc(&you, LOS_SOLID, 2);
        {
            if (stop_attack_prompt(hitfunc, "harm", _cleansing_flame_affects))
                return SPRET_ABORT;
        }
        fail_check();
        cleansing_flame(10 + you.skill_rdiv(SK_INVOCATIONS, 7, 9),
                        CLEANSING_FLAME_INVOCATION, you.pos(), &you);
        break;
    }

    case ABIL_TSO_SUMMON_DIVINE_WARRIOR:
        fail_check();
        summon_holy_warrior(you.skill(SK_INVOCATIONS, 4), false);
        break;

    case ABIL_TSO_BLESS_WEAPON:
        fail_check();
        simple_god_message(" will bless one of your weapons.");
        // included in default force_more_message
        if (!bless_weapon(GOD_SHINING_ONE, SPWPN_HOLY_WRATH, YELLOW))
            return SPRET_ABORT;
        break;

    case ABIL_KIKU_RECEIVE_CORPSES:
        fail_check();
        kiku_receive_corpses(you.skill(SK_NECROMANCY, 4));
        break;

    case ABIL_KIKU_TORMENT:
        fail_check();
        if (!kiku_take_corpse())
        {
            mpr("There are no corpses to sacrifice!");
            return SPRET_ABORT;
        }
        simple_god_message(" torments the living!");
        torment(&you, TORMENT_KIKUBAAQUDGHA, you.pos());
        break;

    case ABIL_KIKU_BLESS_WEAPON:
        fail_check();
        simple_god_message(" will bloody one of your weapons with pain.");
        // included in default force_more_message
        if (!bless_weapon(GOD_KIKUBAAQUDGHA, SPWPN_PAIN, RED))
            return SPRET_ABORT;
        break;

    case ABIL_KIKU_GIFT_NECRONOMICON:
    {
        fail_check();
        if (!kiku_gift_necronomicon())
            return SPRET_ABORT;
        break;
    }

    case ABIL_YRED_INJURY_MIRROR:
        fail_check();
        if (yred_injury_mirror())
            mpr("Another wave of unholy energy enters you.");
        else
        {
            mprf("You offer yourself to %s, and are filled with unholy energy.",
                 god_name(you.religion).c_str());
        }
        you.duration[DUR_MIRROR_DAMAGE] = 9 * BASELINE_DELAY
                     + random2avg(you.piety * BASELINE_DELAY, 2) / 10;
        break;

    case ABIL_YRED_ANIMATE_REMAINS:
        fail_check();
        canned_msg(MSG_ANIMATE_REMAINS);
        if (animate_remains(you.pos(), CORPSE_BODY, BEH_FRIENDLY,
                            MHITYOU, &you, "", GOD_YREDELEMNUL) < 0)
        {
            mpr("There are no remains here to animate!");
            return SPRET_ABORT;
        }
        break;

    case ABIL_YRED_ANIMATE_DEAD:
        fail_check();
        canned_msg(MSG_CALL_DEAD);
        animate_dead(&you, you.skill_rdiv(SK_INVOCATIONS) + 1,
                     BEH_FRIENDLY, MHITYOU, &you, "", GOD_YREDELEMNUL);
        break;

    case ABIL_YRED_RECALL_UNDEAD_SLAVES:
        fail_check();
        start_recall(RECALL_YRED);
        break;

    case ABIL_YRED_DRAIN_LIFE:
    {
        int damage = 0;
        const spret_type result =
            fire_los_attack_spell(SPELL_DRAIN_LIFE,
                                  you.skill_rdiv(SK_INVOCATIONS),
                                  &you, nullptr, fail, &damage);
        if (result != SPRET_SUCCESS)
            return result;

        if (damage > 0)
        {
            mpr("You feel life flooding into your body.");
            inc_hp(damage);
        }
        break;
    }

    case ABIL_YRED_ENSLAVE_SOUL:
    {
        god_acting gdact;
        beam.range = LOS_MAX_RANGE;
        direction_chooser_args args;
        args.restricts = DIR_TARGET;
        args.mode = TARG_HOSTILE;
        args.needs_path = false;

        if (!spell_direction(spd, beam, &args))
            return SPRET_ABORT;

        if (beam.target == you.pos())
        {
            mpr("Your soul already belongs to Yredelemnul.");
            return SPRET_ABORT;
        }

        monster* mons = monster_at(beam.target);
        if (mons == nullptr || !you.can_see(*mons)
            || !yred_can_enslave_soul(mons))
        {
            mpr("You see nothing there you can enslave the soul of!");
            return SPRET_ABORT;
        }

        // The monster can be no more than lightly wounded/damaged.
        if (mons_get_damage_level(*mons) > MDAM_LIGHTLY_DAMAGED)
        {
            simple_monster_message(*mons, "'s soul is too badly injured.");
            return SPRET_ABORT;
        }
        fail_check();

        const int duration = you.skill_rdiv(SK_INVOCATIONS, 3, 4) + 2;
        mons->add_ench(mon_enchant(ENCH_SOUL_RIPE, 0, &you,
                                   duration * BASELINE_DELAY));
        simple_monster_message(*mons, "'s soul is now ripe for the taking.");
        break;
    }

    case ABIL_OKAWARU_HEROISM:
        fail_check();
        mprf(MSGCH_DURATION, you.duration[DUR_HEROISM]
             ? "You feel more confident with your borrowed prowess."
             : "You gain the combat prowess of a mighty hero.");

        you.increase_duration(DUR_HEROISM,
                              10 + random2avg(you.skill(SK_INVOCATIONS, 6), 2),
                              100);
        you.redraw_evasion      = true;
        you.redraw_armour_class = true;
        break;

    case ABIL_OKAWARU_FINESSE:
        fail_check();
        if (you.duration[DUR_FINESSE])
        {
            // "Your [hand(s)] get{s} new energy."
            mprf(MSGCH_DURATION, "%s",
                 you.hands_act("get", "new energy.").c_str());
        }
        else
            mprf(MSGCH_DURATION, "You can now deal lightning-fast blows.");

        you.increase_duration(DUR_FINESSE,
                              10 + random2avg(you.skill(SK_INVOCATIONS, 6), 2),
                              100);

        did_god_conduct(DID_HASTY, 8); // Currently irrelevant.
        break;

    case ABIL_MAKHLEB_MINOR_DESTRUCTION:
    {
        beam.range = min((int)you.current_vision, 5);

        if (!spell_direction(spd, beam))
            return SPRET_ABORT;

        int power = you.skill(SK_INVOCATIONS, 1)
                    + random2(1 + you.skill(SK_INVOCATIONS, 1))
                    + random2(1 + you.skill(SK_INVOCATIONS, 1));

        // Since the actual beam is random, check with BEAM_MMISSILE.
        if (!player_tracer(ZAP_DEBUGGING_RAY, power, beam, beam.range))
            return SPRET_ABORT;

        fail_check();
        beam.origin_spell = SPELL_NO_SPELL; // let zapping reset this

        switch (random2(5))
        {
        case 0: zapping(ZAP_THROW_FLAME, power, beam); break;
        case 1: zapping(ZAP_PAIN, power, beam); break;
        case 2: zapping(ZAP_STONE_ARROW, power, beam); break;
        case 3: zapping(ZAP_SHOCK, power, beam); break;
        case 4: zapping(ZAP_BREATHE_ACID, power / 7, beam); break;
        }
        break;
    }

    case ABIL_MAKHLEB_LESSER_SERVANT_OF_MAKHLEB:
        summon_demon_type(random_choose(MONS_HELLWING, MONS_NEQOXEC,
                                        MONS_ORANGE_DEMON, MONS_SMOKE_DEMON,
                                        MONS_YNOXINUL),
                          20 + you.skill(SK_INVOCATIONS, 3),
                          GOD_MAKHLEB, 0, !fail);
        break;

    case ABIL_MAKHLEB_MAJOR_DESTRUCTION:
    {
        beam.range = you.current_vision;

        if (!spell_direction(spd, beam))
            return SPRET_ABORT;

        int power = 3 + you.skill(SK_INVOCATIONS, 1)
                    + random2(1 + you.skill(SK_INVOCATIONS, 2))
                    + random2(1 + you.skill(SK_INVOCATIONS, 2));

        // Since the actual beam is random, check with BEAM_MMISSILE.
        if (!player_tracer(ZAP_DEBUGGING_RAY, power, beam, beam.range))
            return SPRET_ABORT;

        fail_check();
        {
            beam.origin_spell = SPELL_NO_SPELL; // let zapping reset this
            zap_type ztype =
                random_choose(ZAP_BOLT_OF_FIRE,
                              ZAP_FIREBALL,
                              ZAP_LIGHTNING_BOLT,
                              ZAP_STICKY_FLAME,
                              ZAP_IRON_SHOT,
                              ZAP_BOLT_OF_DRAINING,
                              ZAP_ORB_OF_ELECTRICITY);
            zapping(ztype, power, beam);
        }
        break;
    }

    case ABIL_MAKHLEB_GREATER_SERVANT_OF_MAKHLEB:
        summon_demon_type(random_choose(MONS_EXECUTIONER, MONS_GREEN_DEATH,
                                        MONS_BLIZZARD_DEMON, MONS_BALRUG,
                                        MONS_CACODEMON),
                          20 + you.skill(SK_INVOCATIONS, 3),
                          GOD_MAKHLEB, 0, !fail);
        break;

    case ABIL_TROG_BERSERK:
        fail_check();
        // Trog abilities don't use or train invocations.
        you.go_berserk(true);
        break;

    case ABIL_TROG_REGEN_MR:
        fail_check();
        // Trog abilities don't use or train invocations.
        trog_do_trogs_hand(you.piety / 2);
        break;

    case ABIL_TROG_BROTHERS_IN_ARMS:
        fail_check();
        // Trog abilities don't use or train invocations.
        summon_berserker(you.piety +
                         random2(you.piety/4) - random2(you.piety/4),
                         &you);
        break;

    case ABIL_SIF_MUNA_DIVINE_ENERGY:
        simple_god_message(" will now grant you divine energy when your "
                           "reserves of magic are depleted.");
        mpr("You will briefly lose access to your magic after casting a "
            "spell in this manner.");
        you.attribute[ATTR_DIVINE_ENERGY] = 1;
        break;

    case ABIL_SIF_MUNA_STOP_DIVINE_ENERGY:
        simple_god_message(" stops granting you divine energy.");
        you.attribute[ATTR_DIVINE_ENERGY] = 0;
        break;

    case ABIL_SIF_MUNA_FORGET_SPELL:
        fail_check();
        if (cast_selective_amnesia() <= 0)
            return SPRET_ABORT;
        break;

    case ABIL_SIF_MUNA_CHANNEL_ENERGY:
    {
        fail_check();
        you.increase_duration(DUR_CHANNEL_ENERGY,
            4 + random2avg(you.skill_rdiv(SK_INVOCATIONS, 2, 3), 2), 100);
        break;
    }

    case ABIL_ELYVILON_LIFESAVING:
        fail_check();
        if (you.duration[DUR_LIFESAVING])
            mpr("You renew your call for help.");
        else
        {
            mprf("You beseech %s to protect your life.",
                 god_name(you.religion).c_str());
        }
        // Might be a decrease, this is intentional (like Yred).
        you.duration[DUR_LIFESAVING] = 9 * BASELINE_DELAY
                     + random2avg(you.piety * BASELINE_DELAY, 2) / 10;
        break;

    case ABIL_ELYVILON_LESSER_HEALING:
    case ABIL_ELYVILON_GREATER_HEALING:
    {
        fail_check();
        int pow = 0;
        if (abil.ability == ABIL_ELYVILON_LESSER_HEALING)
            pow = 5 + you.skill_rdiv(SK_INVOCATIONS, 1, 3);
        else
            pow = 10 + you.skill_rdiv(SK_INVOCATIONS, 2, 3);
        const int healed = pow + roll_dice(2, pow);
        mpr("You are healed.");
        inc_hp(healed);
        break;
    }

    case ABIL_ELYVILON_PURIFICATION:
        fail_check();
        elyvilon_purification();
        break;

    case ABIL_ELYVILON_HEAL_OTHER:
    {
        int pow = 30 + you.skill(SK_INVOCATIONS, 1);
        return cast_healing(pow, fail);
    }

    case ABIL_ELYVILON_DIVINE_VIGOUR:
        fail_check();
        if (!elyvilon_divine_vigour())
            return SPRET_ABORT;
        break;

    case ABIL_LUGONU_ABYSS_EXIT:
        fail_check();
        down_stairs(DNGN_EXIT_ABYSS);
        break;

    case ABIL_LUGONU_BEND_SPACE:
        fail_check();
        lugonu_bend_space();
        break;

    case ABIL_LUGONU_BANISH:
    {
        beam.range = you.current_vision;
        const int pow = 68 + you.skill(SK_INVOCATIONS, 3);

        direction_chooser_args args;
        args.mode = TARG_HOSTILE;
        args.get_desc_func = bind(desc_success_chance, placeholders::_1,
                                  zap_ench_power(ZAP_BANISHMENT, pow, false),
                                  false, nullptr);
        if (!spell_direction(spd, beam, &args))
            return SPRET_ABORT;

        if (beam.target == you.pos())
        {
            mpr("You cannot banish yourself!");
            return SPRET_ABORT;
        }

        fail_check();

        return zapping(ZAP_BANISHMENT, pow, beam, true, nullptr, fail);
    }

    case ABIL_LUGONU_CORRUPT:
        fail_check();
        if (!lugonu_corrupt_level(300 + you.skill(SK_INVOCATIONS, 15)))
            return SPRET_ABORT;
        break;

    case ABIL_LUGONU_ABYSS_ENTER:
    {
        fail_check();
        // Deflate HP.
        dec_hp(random2avg(you.hp, 2), false);

        no_notes nx; // This banishment shouldn't be noted.
        banished();
        break;
    }

    case ABIL_LUGONU_BLESS_WEAPON:
        fail_check();
        simple_god_message(" will brand one of your weapons with the "
                           "corruption of the Abyss.");
        // included in default force_more_message
        if (!bless_weapon(GOD_LUGONU, SPWPN_DISTORTION, MAGENTA))
            return SPRET_ABORT;
        break;

    case ABIL_NEMELEX_DRAW_DESTRUCTION:
        fail_check();
        if (!deck_draw(DECK_OF_DESTRUCTION))
            return SPRET_ABORT;
        break;
    case ABIL_NEMELEX_DRAW_ESCAPE:
        fail_check();
        if (!deck_draw(DECK_OF_ESCAPE))
            return SPRET_ABORT;
        break;
    case ABIL_NEMELEX_DRAW_SUMMONING:
        fail_check();
        if (!deck_draw(DECK_OF_SUMMONING))
            return SPRET_ABORT;
        break;
    case ABIL_NEMELEX_DRAW_STACK:
        fail_check();
        if (!deck_draw(DECK_STACK))
            return SPRET_ABORT;
        break;

    case ABIL_NEMELEX_TRIPLE_DRAW:
        fail_check();
        if (!deck_triple_draw())
            return SPRET_ABORT;
        break;

    case ABIL_NEMELEX_DEAL_FOUR:
        fail_check();
        if (!deck_deal())
            return SPRET_ABORT;
        break;

    case ABIL_NEMELEX_STACK_FIVE:
        fail_check();
        if (!deck_stack())
            return SPRET_ABORT;
        break;

    case ABIL_BEOGH_SMITING:
        fail_check();
        if (your_spells(SPELL_SMITING,
                        12 + skill_bump(SK_INVOCATIONS, 6),
                        false, nullptr) == SPRET_ABORT)
        {
            return SPRET_ABORT;
        }
        break;

    case ABIL_BEOGH_GIFT_ITEM:
        if (!beogh_gift_item())
            return SPRET_ABORT;
        break;

    case ABIL_BEOGH_RESURRECTION:
        if (!beogh_resurrect())
            return SPRET_ABORT;
        break;

    case ABIL_BEOGH_RECALL_ORCISH_FOLLOWERS:
        fail_check();
        start_recall(RECALL_BEOGH);
        break;

    case ABIL_STOP_RECALL:
        fail_check();
        mpr("You stop recalling your allies.");
        end_recall();
        break;

    case ABIL_FEDHAS_FUNGAL_BLOOM:
        fedhas_fungal_bloom();
        return SPRET_SUCCESS;

    case ABIL_FEDHAS_SUNLIGHT:
        return fedhas_sunlight(fail);

    case ABIL_FEDHAS_PLANT_RING:
        fail_check();
        if (!fedhas_plant_ring_from_rations())
            return SPRET_ABORT;
        break;

    case ABIL_FEDHAS_RAIN:
        fail_check();
        if (!fedhas_rain(you.pos()))
        {
            canned_msg(MSG_NOTHING_HAPPENS);
            return SPRET_ABORT;
        }
        break;

    case ABIL_FEDHAS_SPAWN_SPORES:
    {
        fail_check();
        const int num = fedhas_corpse_spores();
        ASSERT(num > 0);
        break;
    }

    case ABIL_FEDHAS_EVOLUTION:
        return fedhas_evolve_flora(fail);

    case ABIL_TRAN_BAT:
        fail_check();
        if (!transform(100, transformation::bat))
        {
            crawl_state.zero_turns_taken();
            return SPRET_ABORT;
        }
        break;

    case ABIL_JIYVA_CALL_JELLY:
    {
        fail_check();
        mgen_data mg(MONS_JELLY, BEH_STRICT_NEUTRAL, you.pos(),
                     MHITNOT, MG_NONE, GOD_JIYVA);

        mg.non_actor_summoner = "Jiyva";

        if (!create_monster(mg))
            return SPRET_ABORT;
        break;
    }

    case ABIL_JIYVA_SLIMIFY:
    {
        fail_check();
        const item_def* const weapon = you.weapon();
        const string msg = weapon ? weapon->name(DESC_YOUR)
                                  : ("your " + you.hand_name(true));
        mprf(MSGCH_DURATION, "A thick mucus forms on %s.", msg.c_str());
        you.increase_duration(DUR_SLIMIFY,
                              random2avg(you.piety / 4, 2) + 3, 100);
        break;
    }

    case ABIL_JIYVA_CURE_BAD_MUTATION:
        fail_check();
        jiyva_remove_bad_mutation();
        break;

    case ABIL_CHEIBRIADOS_TIME_STEP:
        fail_check();
        cheibriados_time_step(max(1, you.skill(SK_INVOCATIONS, 10)
                                     * you.piety / 100));
        break;

    case ABIL_CHEIBRIADOS_TIME_BEND:
        fail_check();
        cheibriados_time_bend(16 + you.skill(SK_INVOCATIONS, 8));
        break;

    case ABIL_CHEIBRIADOS_DISTORTION:
        fail_check();
        cheibriados_temporal_distortion();
        break;

    case ABIL_CHEIBRIADOS_SLOUCH:
        fail_check();
        if (!cheibriados_slouch())
            return SPRET_ABORT;
        break;

    case ABIL_ASHENZARI_CURSE:
        fail_check();
        if (!ashenzari_curse_item())
            return SPRET_ABORT;
        break;

    case ABIL_ASHENZARI_SCRYING:
        fail_check();
        if (you.duration[DUR_SCRYING])
            mpr("You extend your astral sight.");
        else
            mpr("You gain astral sight.");
        you.duration[DUR_SCRYING] = 100 + random2avg(you.piety * 2, 2);
        you.xray_vision = true;
        viewwindow(true);
        break;

    case ABIL_ASHENZARI_TRANSFER_KNOWLEDGE:
        fail_check();
        if (!ashenzari_transfer_knowledge())
        {
            canned_msg(MSG_OK);
            return SPRET_ABORT;
        }
        break;

    case ABIL_ASHENZARI_END_TRANSFER:
        fail_check();
        if (!ashenzari_end_transfer())
        {
            canned_msg(MSG_OK);
            return SPRET_ABORT;
        }
        break;

    case ABIL_DITHMENOS_SHADOW_STEP:
        if (_abort_if_stationary())
            return SPRET_ABORT;
        if (you.duration[DUR_EXHAUSTED])
        {
            mpr("You are too exhausted.");
            return SPRET_ABORT;
        }
        fail_check();
        
        if (!dithmenos_shadow_step())
        {
            canned_msg(MSG_OK);
            return SPRET_ABORT;
        }
        
        you.increase_duration(DUR_EXHAUSTED, 4);
        break;

    case ABIL_DITHMENOS_SHADOW_FORM:
        fail_check();
        if (!transform(you.skill(SK_INVOCATIONS, 2), transformation::shadow))
        {
            crawl_state.zero_turns_taken();
            return SPRET_ABORT;
        }
        break;

    case ABIL_GOZAG_POTION_PETITION:
        fail_check();
        run_uncancel(UNC_POTION_PETITION, 0);
        break;

    case ABIL_GOZAG_CALL_MERCHANT:
        fail_check();
        run_uncancel(UNC_CALL_MERCHANT, 0);
        break;

    case ABIL_GOZAG_BRIBE_BRANCH:
        fail_check();
        if (!gozag_bribe_branch())
            return SPRET_ABORT;
        break;

    case ABIL_QAZLAL_UPHEAVAL:
        return qazlal_upheaval(coord_def(), false, fail);

    case ABIL_QAZLAL_ELEMENTAL_FORCE:
        return qazlal_elemental_force(fail);

    case ABIL_QAZLAL_DISASTER_AREA:
        fail_check();
        if (!qazlal_disaster_area())
            return SPRET_ABORT;
        break;

    case ABIL_RU_SACRIFICE_PURITY:
    case ABIL_RU_SACRIFICE_WORDS:
    case ABIL_RU_SACRIFICE_DRINK:
    case ABIL_RU_SACRIFICE_ESSENCE:
    case ABIL_RU_SACRIFICE_HEALTH:
    case ABIL_RU_SACRIFICE_STEALTH:
    case ABIL_RU_SACRIFICE_ARTIFICE:
    case ABIL_RU_SACRIFICE_LOVE:
    case ABIL_RU_SACRIFICE_COURAGE:
    case ABIL_RU_SACRIFICE_ARCANA:
    case ABIL_RU_SACRIFICE_NIMBLENESS:
    case ABIL_RU_SACRIFICE_DURABILITY:
    case ABIL_RU_SACRIFICE_HAND:
    case ABIL_RU_SACRIFICE_EXPERIENCE:
    case ABIL_RU_SACRIFICE_SKILL:
    case ABIL_RU_SACRIFICE_EYE:
    case ABIL_RU_SACRIFICE_RESISTANCE:
        fail_check();
        if (!ru_do_sacrifice(abil.ability))
            return SPRET_ABORT;
        break;

    case ABIL_RU_REJECT_SACRIFICES:
        fail_check();
        if (!ru_reject_sacrifices())
            return SPRET_ABORT;
        break;

    case ABIL_RU_DRAW_OUT_POWER:
        fail_check();
        if (you.duration[DUR_EXHAUSTED])
        {
            mpr("You're too exhausted to draw out your power.");
            return SPRET_ABORT;
        }
        if (you.hp == you.hp_max && you.magic_points == you.max_magic_points
            && !you.duration[DUR_CONF]
            && !you.duration[DUR_SLOW]
            && !you.attribute[ATTR_HELD]
            && !you.petrifying()
            && !you.is_constricted())
        {
            mpr("You have no need to draw out power.");
            return SPRET_ABORT;
        }
        ru_draw_out_power();
        you.increase_duration(DUR_EXHAUSTED, 12 + random2(5));
        break;

    case ABIL_RU_POWER_LEAP:
        if (you.duration[DUR_EXHAUSTED])
        {
            mpr("You're too exhausted to power leap.");
            return SPRET_ABORT;
        }

        if (_abort_if_stationary())
            return SPRET_ABORT;

        fail_check();

        if (!ru_power_leap())
        {
            canned_msg(MSG_OK);
            return SPRET_ABORT;
        }
        you.increase_duration(DUR_EXHAUSTED, 18 + random2(8));
        break;

    case ABIL_RU_APOCALYPSE:
        if (you.duration[DUR_EXHAUSTED])
        {
            mpr("You're too exhausted to unleash your apocalyptic power.");
            return SPRET_ABORT;
        }

        fail_check();

        if (!ru_apocalypse())
            return SPRET_ABORT;
        you.increase_duration(DUR_EXHAUSTED, 30 + random2(20));
        break;

#if TAG_MAJOR_VERSION == 34
    case ABIL_PAKELLAS_DEVICE_SURGE:
    {
        fail_check();

        mprf(MSGCH_DURATION, "You feel a buildup of energy.");
        you.increase_duration(DUR_DEVICE_SURGE,
                              random2avg(you.piety / 4, 2) + 3, 100);
        break;
    }
#endif

    case ABIL_USKAYAW_STOMP:
        fail_check();
        if (!uskayaw_stomp())
            return SPRET_ABORT;
        break;

    case ABIL_USKAYAW_LINE_PASS:
        if (_abort_if_stationary())
            return SPRET_ABORT;
        fail_check();
        if (!uskayaw_line_pass())
            return SPRET_ABORT;
        break;

    case ABIL_USKAYAW_GRAND_FINALE:
        return uskayaw_grand_finale(fail);

    case ABIL_HEPLIAKLQANA_IDEALISE:
        return hepliaklqana_idealise(fail);

    case ABIL_HEPLIAKLQANA_RECALL:
        fail_check();
        if (try_recall(hepliaklqana_ancestor()))
            upgrade_hepliaklqana_ancestor(true);
        break;

    case ABIL_HEPLIAKLQANA_TRANSFERENCE:
        return hepliaklqana_transference(fail);

    case ABIL_HEPLIAKLQANA_TYPE_KNIGHT:
    case ABIL_HEPLIAKLQANA_TYPE_BATTLEMAGE:
    case ABIL_HEPLIAKLQANA_TYPE_HEXER:
        if (!hepliaklqana_choose_ancestor_type(abil.ability))
            return SPRET_ABORT;
        break;

    case ABIL_HEPLIAKLQANA_IDENTITY:
        hepliaklqana_choose_identity();
        break;

    case ABIL_WU_JIAN_SERPENTS_LASH:
        if (you.attribute[ATTR_SERPENTS_LASH])
        {
            mpr("You are already lashing out.");
            return SPRET_ABORT;
        }
        if (you.duration[DUR_EXHAUSTED])
        {
            mpr("You are too exhausted to lash out.");
            return SPRET_ABORT;
        }
        fail_check();
        mprf(MSGCH_GOD, "Your muscles tense, ready for explosive movement...");
        you.attribute[ATTR_SERPENTS_LASH] = 2;
        you.redraw_status_lights = true;
        return SPRET_SUCCESS;

    case ABIL_WU_JIAN_HEAVENLY_STORM:
        fail_check();
        mprf(MSGCH_GOD, "The air is filled with shimmering golden clouds!");
        wu_jian_sifu_message(" says: The storm will not cease as long as you "
                             "keep fighting, disciple!");

        for (radius_iterator ai(you.pos(), 2, C_SQUARE, LOS_SOLID); ai; ++ai)
        {
            if (!cell_is_solid(*ai))
                place_cloud(CLOUD_GOLD_DUST, *ai, 5 + random2(5), &you);
        }

        you.attribute[ATTR_HEAVENLY_STORM] = 6;
        you.duration[DUR_HEAVENLY_STORM] = WU_JIAN_HEAVEN_TICK_TIME;
        invalidate_agrid(true);
        break;

    case ABIL_WU_JIAN_WALLJUMP:
        fail_check();
        return wu_jian_wall_jump_ability();

    case ABIL_RENOUNCE_RELIGION:
        fail_check();
        if (yesno("Really renounce your faith, foregoing its fabulous benefits?",
                  false, 'n')
            && yesno("Are you sure you won't change your mind later?",
                     false, 'n'))
        {
            excommunication(true);
        }
        else
        {
            canned_msg(MSG_OK);
            return SPRET_ABORT;
        }
        break;

    case ABIL_CONVERT_TO_BEOGH:
        fail_check();
        god_pitch(GOD_BEOGH);
        if (you_worship(GOD_BEOGH))
        {
            spare_beogh_convert();
            break;
        }
        return SPRET_ABORT;

    case ABIL_NON_ABILITY:
        fail_check();
        mpr("Sorry, you can't do that.");
        break;

    default:
        die("invalid ability");
    }

    return SPRET_SUCCESS;
}

// [ds] Increase piety cost for god abilities that are particularly
// overpowered in Sprint. Yes, this is a hack. No, I don't care.
static int _scale_piety_cost(ability_type abil, int original_cost)
{
    // Abilities that have aroused our ire earn 2.5x their classic
    // Crawl piety cost.
    return (crawl_state.game_is_sprint()
            && (abil == ABIL_TROG_BROTHERS_IN_ARMS
                || abil == ABIL_MAKHLEB_GREATER_SERVANT_OF_MAKHLEB))
           ? div_rand_round(original_cost * 5, 2)
           : original_cost;
}

static void _pay_ability_costs(const ability_def& abil)
{
    // wall jump handles its own timing, because it can be instant if
    // serpent's lash is activated.
    if (abil.flags & abflag::instant)
    {
        you.turn_is_over = false;
        you.elapsed_time_at_last_input = you.elapsed_time;
        update_turn_count();
    }
    else if (abil.ability != ABIL_WU_JIAN_WALLJUMP)
        you.turn_is_over = true;

    const int food_cost  = abil.food_cost + random2avg(abil.food_cost, 2);
    const int piety_cost =
        _scale_piety_cost(abil.ability, abil.piety_cost.cost());
    const int hp_cost    = abil.hp_cost.cost(you.hp_max);

    dprf("Cost: mp=%d; hp=%d; food=%d; piety=%d",
         abil.mp_cost, hp_cost, food_cost, piety_cost);

    if (abil.mp_cost)
        dec_mp(abil.mp_cost);

    if (abil.hp_cost)
        dec_hp(hp_cost, false);

    if (food_cost)
        make_hungry(food_cost, false, true);

    if (piety_cost)
        lose_piety(piety_cost);
}

int choose_ability_menu(const vector<talent>& talents)
{
    ToggleableMenu abil_menu(MF_SINGLESELECT | MF_ANYPRINTABLE
            | MF_NO_WRAP_ROWS | MF_TOGGLE_ACTION | MF_ALWAYS_SHOW_MORE);

    abil_menu.set_highlighter(nullptr);
#ifdef USE_TILE_LOCAL
    {
        // Hack like the one in spl-cast.cc:list_spells() to align the title.
        ToggleableMenuEntry* me =
            new ToggleableMenuEntry("Ability - do what?                  "
                                    "Cost                          Failure",
                                    "Ability - describe what?            "
                                    "Cost                          Failure",
                                    MEL_ITEM);
        me->colour = BLUE;
        abil_menu.set_title(me, true, true);
    }
#else
    abil_menu.set_title(
        new ToggleableMenuEntry("Ability - do what?                  "
                                "Cost                          Failure",
                                "Ability - describe what?            "
                                "Cost                          Failure",
                                MEL_TITLE), true, true);
#endif
    abil_menu.set_tag("ability");
    abil_menu.add_toggle_key('!');
    abil_menu.add_toggle_key('?');
    abil_menu.menu_action = Menu::ACT_EXECUTE;

    if (crawl_state.game_is_hints())
    {
        // XXX: This could be buggy if you manage to pick up lots and
        // lots of abilities during hints mode.
        abil_menu.set_more(hints_abilities_info());
    }
    else
    {
        abil_menu.set_more(formatted_string::parse_string(
                           "Press '<w>!</w>' or '<w>?</w>' to toggle "
                           "between ability selection and description."));
    }

    int numbers[52];
    for (int i = 0; i < 52; ++i)
        numbers[i] = i;

    bool found_invocations = false;

    // First add all non-invocation abilities.
    for (unsigned int i = 0; i < talents.size(); ++i)
    {
        if (talents[i].is_invocation)
            found_invocations = true;
        else
        {
            ToggleableMenuEntry* me =
                new ToggleableMenuEntry(describe_talent(talents[i]),
                                        describe_talent(talents[i]),
                                        MEL_ITEM, 1, talents[i].hotkey);
            me->data = &numbers[i];
#ifdef USE_TILE
            me->add_tile(tile_def(tileidx_ability(talents[i].which), TEX_GUI));
#endif
            if (!check_ability_possible(talents[i].which, true))
            {
                me->colour = COL_INAPPLICABLE;
#ifdef USE_TILE
                me->add_tile(tile_def(TILEI_MESH, TEX_ICONS));
#endif
            }
            else if (_check_ability_dangerous(talents[i].which, true))
                me->colour = COL_DANGEROUS;
            // Only check this here, since your god can't hate its own abilities
            else if (god_hates_ability(talents[i].which, you.religion))
                me->colour = COL_FORBIDDEN;
            abil_menu.add_entry(me);
        }
    }

    if (found_invocations)
    {
#ifdef USE_TILE_LOCAL
        MenuEntry* subtitle = new MenuEntry(" Invocations -    ", MEL_ITEM);
        subtitle->colour = BLUE;
        abil_menu.add_entry(subtitle);
#else
        abil_menu.add_entry(new MenuEntry(" Invocations -    ", MEL_SUBTITLE));
#endif
        for (unsigned int i = 0; i < talents.size(); ++i)
        {
            if (talents[i].is_invocation)
            {
                ToggleableMenuEntry* me =
                    new ToggleableMenuEntry(describe_talent(talents[i]),
                                            describe_talent(talents[i]),
                                            MEL_ITEM, 1, talents[i].hotkey);
                me->data = &numbers[i];
#ifdef USE_TILE
                me->add_tile(tile_def(tileidx_ability(talents[i].which),
                                      TEX_GUI));
#endif
                if (!check_ability_possible(talents[i].which, true))
                {
                    me->colour = COL_INAPPLICABLE;
#ifdef USE_TILE
                    me->add_tile(tile_def(TILEI_MESH, TEX_ICONS));
#endif
                }
                else if (_check_ability_dangerous(talents[i].which, true))
                    me->colour = COL_DANGEROUS;
                abil_menu.add_entry(me);
            }
        }
    }

    int ret = -1;
    abil_menu.on_single_selection = [&abil_menu, &talents, &ret](const MenuEntry& sel)
    {
        ASSERT(sel.hotkeys.size() == 1);
        int selected = *(static_cast<int*>(sel.data));

        if (abil_menu.menu_action == Menu::ACT_EXAMINE)
            _print_talent_description(talents[selected]);
        else
            ret = *(static_cast<int*>(sel.data));
        return abil_menu.menu_action == Menu::ACT_EXAMINE;
    };
    abil_menu.show(false);
    if (!crawl_state.doing_prev_cmd_again)
        redraw_screen();
    return ret;
}

string describe_talent(const talent& tal)
{
    ASSERT(tal.which != ABIL_NON_ABILITY);

    const string failure = failure_rate_to_string(tal.fail)
        + (testbits(get_ability_def(tal.which).flags, abflag::hostile)
           ? " hostile" : "");

    ostringstream desc;
    desc << left
         << chop_string(ability_name(tal.which), 32)
         << chop_string(make_cost_description(tal.which), 30)
         << chop_string(failure, 12);
    return trimmed_string(desc.str());
}

static void _add_talent(vector<talent>& vec, const ability_type ability,
                        bool check_confused)
{
    const talent t = get_talent(ability, check_confused);
    if (t.which != ABIL_NON_ABILITY)
        vec.push_back(t);
}

/**
 * Return all relevant talents that the player has.
 *
 * Currently the only abilities that are affected by include_unusable are god
 * abilities (affect by e.g. penance or silence).
 * @param check_confused If true, abilities that don't work when confused will
 *                       be excluded.
 * @param include_unusable If true, abilities that are currently unusable will
 *                         be excluded.
 * @return  A vector of talent structs.
 */
vector<talent> your_talents(bool check_confused, bool include_unusable)
{
    vector<talent> talents;

    // Species-based abilities.
    if (you.species == SP_FORMICID
        && (form_keeps_mutations() || include_unusable))
    {
        _add_talent(talents, ABIL_DIG, check_confused);
        if (!crawl_state.game_is_sprint() || brdepth[you.where_are_you] > 1)
            _add_talent(talents, ABIL_SHAFT_SELF, check_confused);
    }

    if (you.get_mutation_level(MUT_HOP))
        _add_talent(talents, ABIL_HOP, check_confused);

    // Spit Poison, possibly upgraded to Breathe Poison.
    if (you.get_mutation_level(MUT_SPIT_POISON) == 2)
        _add_talent(talents, ABIL_BREATHE_POISON, check_confused);
    else if (you.get_mutation_level(MUT_SPIT_POISON))
        _add_talent(talents, ABIL_SPIT_POISON, check_confused);

    if (species_is_draconian(you.species)
        // Draconians don't maintain their original breath weapons
        // if shapechanged into a non-dragon form.
        && (!form_changed_physiology() || you.form == transformation::dragon)
        && draconian_breath(you.species) != ABIL_NON_ABILITY)
    {
        _add_talent(talents, draconian_breath(you.species), check_confused);
    }

    if (you.species == SP_VAMPIRE
        && you.hunger_state > HS_SATIATED
        && you.form != transformation::bat)
    {
        _add_talent(talents, ABIL_TRAN_BAT, check_confused);
    }

    if (you.racial_permanent_flight() && !you.attribute[ATTR_PERM_FLIGHT])
    {
        // Avariel, black draconians and gargoyles get permaflight at XL 9/14/14.
        // Other dracs can mutate big wings whenever as well.
        _add_talent(talents, ABIL_FLY, check_confused);
    }

    if (you.attribute[ATTR_PERM_FLIGHT] && you.racial_permanent_flight())
        _add_talent(talents, ABIL_STOP_FLYING, check_confused);

    // Mutations
    if (you.get_mutation_level(MUT_HURL_DAMNATION))
        _add_talent(talents, ABIL_DAMNATION, check_confused);

    if (you.duration[DUR_TRANSFORMATION] && !you.transform_uncancellable)
        _add_talent(talents, ABIL_END_TRANSFORMATION, check_confused);

    if (you.get_mutation_level(MUT_BLINK))
        _add_talent(talents, ABIL_BLINK, check_confused);

    // Religious abilities.
    for (ability_type abil : get_god_abilities(include_unusable, false,
                                               include_unusable))
    {
        _add_talent(talents, abil, check_confused);
    }

    // And finally, the ability to opt-out of your faith {dlb}:
    if (!you_worship(GOD_NO_GOD))
        _add_talent(talents, ABIL_RENOUNCE_RELIGION, check_confused);

    if (env.level_state & LSTATE_BEOGH && can_convert_to_beogh())
        _add_talent(talents, ABIL_CONVERT_TO_BEOGH, check_confused);

    //jmf: Check for breath weapons - they're exclusive of each other, I hope!
    //     Make better ones come first.
    if (you.species != SP_RED_DRACONIAN && you.form == transformation::dragon
         && dragon_form_dragon_type() == MONS_FIRE_DRAGON)
    {
        _add_talent(talents, ABIL_BREATHE_FIRE, check_confused);
    }

    if (you.duration[DUR_PORTAL_PROJECTILE])
        _add_talent(talents, ABIL_CANCEL_PPROJ, check_confused);

    if (you.duration[DUR_PIERCING_SHOT])
        _add_talent(talents, ABIL_CANCEL_PIERCE, check_confused);

    // Evocations from items.
    if (you.scan_artefacts(ARTP_BLINK)
        && !you.get_mutation_level(MUT_NO_ARTIFICE))
    {
        _add_talent(talents, ABIL_EVOKE_BLINK, check_confused);
    }

    if (player_equip_unrand(UNRAND_THIEF)
        && !you.get_mutation_level(MUT_NO_ARTIFICE))
    {
        _add_talent(talents, ABIL_EVOKE_FOG, check_confused);
    }

    if (player_equip_unrand(UNRAND_RATSKIN_CLOAK)
        && !you.get_mutation_level(MUT_NO_ARTIFICE)
        && !you.get_mutation_level(MUT_NO_LOVE))
    {
        _add_talent(talents, ABIL_EVOKE_RATSKIN, check_confused);
    }

    if (player_equip_unrand(UNRAND_RCLOUDS)
        && !you.get_mutation_level(MUT_NO_ARTIFICE))
    {
        _add_talent(talents, ABIL_EVOKE_THUNDER, check_confused);
    }

    if (you.evokable_berserk() && !you.get_mutation_level(MUT_NO_ARTIFICE))
        _add_talent(talents, ABIL_EVOKE_BERSERK, check_confused);

    if (you.evokable_invis() > 0
        && !you.get_mutation_level(MUT_NO_ARTIFICE)
        && !you.duration[DUR_INVIS])
    {
        _add_talent(talents, ABIL_EVOKE_TURN_INVISIBLE, check_confused);
    }

    if (you.evokable_flight() && !you.get_mutation_level(MUT_NO_ARTIFICE))
    {
        // Has no effect on permanently flying Tengu.
        if (!you.permanent_flight() || !you.racial_permanent_flight())
        {
            // You can still evoke perm flight if you have temporary one.
            if (!you.airborne()
                || !you.attribute[ATTR_PERM_FLIGHT]
                   && you.wearing_ego(EQ_ALL_ARMOUR, SPARM_FLYING))
            {
                _add_talent(talents, ABIL_EVOKE_FLIGHT, check_confused);
            }
            // Now you can only turn flight off if you have an
            // activatable item. Potions and spells will have to time
            // out.
            if (you.airborne() && !you.attribute[ATTR_FLIGHT_UNCANCELLABLE])
                _add_talent(talents, ABIL_STOP_FLYING, check_confused);
        }
    }

    // Find hotkeys for the non-hotkeyed talents.
    for (talent &tal : talents)
    {
        const int index = _lookup_ability_slot(tal.which);
        if (index > -1)
        {
            tal.hotkey = index_to_letter(index);
            continue;
        }

        // Try to find a free hotkey for i, starting from Z.
        for (int k = 51; k >= 0; ++k)
        {
            const int kkey = index_to_letter(k);
            bool good_key = true;

            // Check that it doesn't conflict with other hotkeys.
            for (const talent &other : talents)
                if (other.hotkey == kkey)
                {
                    good_key = false;
                    break;
                }

            if (good_key)
            {
                tal.hotkey = k;
                you.ability_letter_table[k] = tal.which;
                break;
            }
        }
        // In theory, we could be left with an unreachable ability
        // here (if you have 53 or more abilities simultaneously).
    }

    return talents;
}

/**
 * Maybe move an ability to the slot given by the ability_slot option.
 *
 * @param[in] slot current slot of the ability
 * @returns the new slot of the ability; may still be slot, if the ability
 *          was not reassigned.
 */
int auto_assign_ability_slot(int slot)
{
    const ability_type abil_type = you.ability_letter_table[slot];
    const string abilname = lowercase_string(ability_name(abil_type));
    bool overwrite = false;
    // check to see whether we've chosen an automatic label:
    for (auto& mapping : Options.auto_ability_letters)
    {
        if (!mapping.first.matches(abilname))
            continue;
        for (char i : mapping.second)
        {
            if (i == '+')
                overwrite = true;
            else if (i == '-')
                overwrite = false;
            else if (isaalpha(i))
            {
                const int index = letter_to_index(i);
                ability_type existing_ability = you.ability_letter_table[index];

                if (existing_ability == ABIL_NON_ABILITY
                    || existing_ability == abil_type)
                {
                    // Unassigned or already assigned to this ability.
                    you.ability_letter_table[index] = abil_type;
                    if (slot != index)
                        you.ability_letter_table[slot] = ABIL_NON_ABILITY;
                    return index;
                }
                else if (overwrite)
                {
                    const string str = lowercase_string(ability_name(existing_ability));
                    // Don't overwrite an ability matched by the same rule.
                    if (mapping.first.matches(str))
                        continue;
                    you.ability_letter_table[slot] = abil_type;
                    swap_ability_slots(slot, index, true);
                    return index;
                }
                // else occupied, continue to the next mapping.
            }
        }
    }
    return slot;
}

// Returns an index (0-51) if already assigned, -1 if not.
static int _lookup_ability_slot(const ability_type abil)
{
    // Placeholder handling, part 2: The ability we have might
    // correspond to a placeholder, in which case the ability letter
    // table will contain that placeholder. Convert the latter to
    // its corresponding ability before comparing the two, so that
    // we'll find the placeholder's index properly.
    for (int slot = 0; slot < 52; slot++)
        if (fixup_ability(you.ability_letter_table[slot]) == abil)
            return slot;
    return -1;
}

// Assign a new ability slot if necessary. Returns an index (0-51) if
// successful, -1 if you should just use the next one.
int find_ability_slot(const ability_type abil, char firstletter)
{
    // If we were already assigned a slot, use it.
    int had_slot = _lookup_ability_slot(abil);
    if (had_slot > -1)
        return had_slot;

    // No requested slot, find new one and make it preferred.

    // firstletter defaults to 'f', because a-e is for invocations
    int first_slot = letter_to_index(firstletter);

    // Reserve the first non-god ability slot (f) for Draconian breath
    if (you.species == SP_BASE_DRACONIAN && first_slot >= letter_to_index('f'))
        first_slot += 1;

    ASSERT(first_slot < 52);

    switch (abil)
    {
    case ABIL_ELYVILON_LIFESAVING:
        first_slot = letter_to_index('p');
        break;
    case ABIL_KIKU_GIFT_NECRONOMICON:
        first_slot = letter_to_index('N');
        break;
    case ABIL_TSO_BLESS_WEAPON:
    case ABIL_KIKU_BLESS_WEAPON:
    case ABIL_LUGONU_BLESS_WEAPON:
        first_slot = letter_to_index('W');
        break;
    case ABIL_CONVERT_TO_BEOGH:
        first_slot = letter_to_index('Y');
        break;
    case ABIL_RU_SACRIFICE_PURITY:
    case ABIL_RU_SACRIFICE_WORDS:
    case ABIL_RU_SACRIFICE_DRINK:
    case ABIL_RU_SACRIFICE_ESSENCE:
    case ABIL_RU_SACRIFICE_HEALTH:
    case ABIL_RU_SACRIFICE_STEALTH:
    case ABIL_RU_SACRIFICE_ARTIFICE:
    case ABIL_RU_SACRIFICE_LOVE:
    case ABIL_RU_SACRIFICE_COURAGE:
    case ABIL_RU_SACRIFICE_ARCANA:
    case ABIL_RU_SACRIFICE_NIMBLENESS:
    case ABIL_RU_SACRIFICE_DURABILITY:
    case ABIL_RU_SACRIFICE_HAND:
    case ABIL_RU_SACRIFICE_EXPERIENCE:
    case ABIL_RU_SACRIFICE_SKILL:
    case ABIL_RU_SACRIFICE_EYE:
    case ABIL_RU_SACRIFICE_RESISTANCE:
    case ABIL_RU_REJECT_SACRIFICES:
    case ABIL_HEPLIAKLQANA_TYPE_KNIGHT:
    case ABIL_HEPLIAKLQANA_TYPE_BATTLEMAGE:
    case ABIL_HEPLIAKLQANA_TYPE_HEXER:
    case ABIL_HEPLIAKLQANA_IDENTITY: // move this?
        first_slot = letter_to_index('G');
        break;
    default:
        break;
    }

    for (int slot = first_slot; slot < 52; ++slot)
    {
        if (you.ability_letter_table[slot] == ABIL_NON_ABILITY)
        {
            you.ability_letter_table[slot] = abil;
            return auto_assign_ability_slot(slot);
        }
    }

    // If we can't find anything else, try a-e.
    for (int slot = first_slot - 1; slot >= 0; --slot)
    {
        if (you.ability_letter_table[slot] == ABIL_NON_ABILITY)
        {
            you.ability_letter_table[slot] = abil;
            return auto_assign_ability_slot(slot);
        }
    }

    // All letters are assigned.
    return -1;
}


vector<ability_type> get_god_abilities(bool ignore_silence, bool ignore_piety,
                                       bool ignore_penance)
{
    vector<ability_type> abilities;
    if (you_worship(GOD_RU))
    {
        ASSERT(you.props.exists(AVAILABLE_SAC_KEY));
        bool any_sacrifices = false;
        for (const auto& store : you.props[AVAILABLE_SAC_KEY].get_vector())
        {
            any_sacrifices = true;
            abilities.push_back(static_cast<ability_type>(store.get_int()));
        }
        if (any_sacrifices)
            abilities.push_back(ABIL_RU_REJECT_SACRIFICES);
    }
    // XXX: should we check ignore_piety?
    if (you_worship(GOD_HEPLIAKLQANA)
        && piety_rank() >= 2 && !you.props.exists(HEPLIAKLQANA_ALLY_TYPE_KEY))
    {
        for (int anc_type = ABIL_HEPLIAKLQANA_FIRST_TYPE;
             anc_type <= ABIL_HEPLIAKLQANA_LAST_TYPE;
             ++anc_type)
        {
            abilities.push_back(static_cast<ability_type>(anc_type));
        }
    }
    if (you_worship(GOD_NEMELEX_XOBEH))
    {
        for (int deck = ABIL_NEMELEX_FIRST_DECK;
             deck <= ABIL_NEMELEX_LAST_DECK;
             ++deck)
        {
            abilities.push_back(static_cast<ability_type>(deck));
        }
        if (!you.props[NEMELEX_STACK_KEY].get_vector().empty())
            abilities.push_back(ABIL_NEMELEX_DRAW_STACK);
    }
    if (you.transfer_skill_points > 0)
        abilities.push_back(ABIL_ASHENZARI_END_TRANSFER);
    if (silenced(you.pos()) && you_worship(GOD_WU_JIAN) && piety_rank() >= 2)
        abilities.push_back(ABIL_WU_JIAN_WALLJUMP);

    if (!ignore_silence && silenced(you.pos()))
        return abilities;
    // Remaining abilities are unusable if silenced.
    for (const auto& power : get_god_powers(you.religion))
    {
        if (god_power_usable(power, ignore_piety, ignore_penance))
        {
            const ability_type abil = fixup_ability(power.abil);
            ASSERT(abil != ABIL_NON_ABILITY);
            abilities.push_back(abil);
        }
    }

    return abilities;
}

void swap_ability_slots(int index1, int index2, bool silent)
{
    // Swap references in the letter table.
    ability_type tmp = you.ability_letter_table[index2];
    you.ability_letter_table[index2] = you.ability_letter_table[index1];
    you.ability_letter_table[index1] = tmp;

    if (!silent)
    {
        mprf_nocap("%c - %s", index_to_letter(index2),
                   ability_name(you.ability_letter_table[index2]));
    }

}

/**
 * What skill affects the success chance/power of a given skill, if any?
 *
 * @param ability       The ability in question.
 * @return              The skill that governs the ability, or SK_NONE.
 */
skill_type abil_skill(ability_type ability)
{
    ASSERT(ability != ABIL_NON_ABILITY);
    return get_ability_def(ability).failure.skill();
}

/**
 * How valuable is it to train the skill that governs this ability? (What
 * 'magnitude' does the ability have?)
 *
 * @param ability       The ability in question.
 * @return              A 'magnitude' for the ability, probably < 10.
 */
int abil_skill_weight(ability_type ability)
{
    ASSERT(ability != ABIL_NON_ABILITY);
    // This is very loosely modelled on a legacy model; fairly arbitrary.
    const int base_fail = get_ability_def(ability).failure.base_chance;
    const int floor = base_fail ? 1 : 0;
    return max(floor, div_rand_round(base_fail, 8) - 3);
}


////////////////////////////////////////////////////////////////////////
// generic_cost

int generic_cost::cost() const
{
    return base + (add > 0 ? random2avg(add, rolls) : 0);
}

int scaling_cost::cost(int max) const
{
    return (value < 0) ? (-value) : ((value * max + 500) / 1000);
}
