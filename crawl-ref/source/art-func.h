/**
 * @file
 * @brief Functions non-standard unrandarts uses.
**/

/*
 * util/art-data.pl scans through this file to grab the functions
 * non-standard unrandarts use and put them into the unranddata structs
 * in art-data.h, so the function names must have the form of
 * _UNRAND_ENUM_func_name() in order to be recognised.
 */

#ifdef ART_FUNC_H
#error "art-func.h included twice!"
#endif

#ifdef ART_DATA_H
#error "art-func.h must be included before art-data.h"
#endif

#define ART_FUNC_H

#include "act-iter.h"      // For monster_near_iterator
#include "areas.h"         // For silenced() and invalidate_agrid()
#include "attack.h"        // For attack_strength_punctuation()
#include "beam.h"          // For Lajatang of Order's silver damage
#include "cloud.h"         // For storm bow's and robe of clouds' rain
#include "coordit.h"       // For distance_iterator()
#include "english.h"       // For apostrophise
#include "exercise.h"      // For practise_evoking
#include "fight.h"
#include "food.h"          // For evokes
#include "ghost.h"         // For is_dragonkind ghost_demon datas
#include "god-conduct.h"    // did_god_conduct
#include "god-passive.h"    // passive_t::want_curses
#include "mgen-data.h"     // For Sceptre of Asmodeus evoke
#include "mon-death.h"     // For demon axe's SAME_ATTITUDE
#include "mon-place.h"     // For Sceptre of Asmodeus evoke
#include "mon-poly.h"      // For Plutonium Sword
#include "nearby-danger.h" // For Staff of Battle
#include "player.h"
#include "player-stats.h"
#include "spl-cast.h"      // For evokes
#include "spl-damage.h"
#include "spl-goditem.h"   // For Sceptre of Torment tormenting
#include "spl-miscast.h"
#include "spl-summoning.h" // For Zonguldrok animating dead and Wucad abjuration
#include "terrain.h"       // For storm bow
#include "transform.h"     // For glaive of prune
#include "view.h"          // For arc blade's discharge effect

// prop recording whether the singing sword has said hello yet
#define SS_WELCOME_KEY "ss_welcome"
// similarly, for the majin-bo
#define MB_WELCOME_KEY "mb_welcome"

/*******************
 * Helper functions.
 *******************/

static void _equip_mpr(bool* show_msgs, const char* msg,
                       msg_channel_type chan = MSGCH_PLAIN)
{
    bool do_show = true;
    if (show_msgs == nullptr)
        show_msgs = &do_show;

    if (*show_msgs)
        mprf(chan, "%s", msg);

    // Caller shouldn't give any more messages.
    *show_msgs = false;
}

/*******************
 * Unrand functions.
 *******************/

static bool _evoke_sceptre_of_asmodeus()
{
    if (!x_chance_in_y(you.skill(SK_EVOCATIONS, 100), 3000))
        return false;

    const monster_type mon = random_choose_weighted(
                                   3, MONS_EFREET,
                                   3, MONS_SUN_DEMON,
                                   3, MONS_BALRUG,
                                   2, MONS_HELLION,
                                   1, MONS_BRIMSTONE_FIEND);

    mgen_data mg(mon, BEH_CHARMED, you.pos(), MHITYOU,
                 MG_FORCE_BEH, you.religion);
    mg.set_summoned(&you, 0, 0);
    mg.extra_flags |= (MF_NO_REWARD | MF_HARD_RESET);

    monster *m = create_monster(mg);

    if (m)
    {
        mpr("The sceptre summons one of its servants.");
        did_god_conduct(DID_EVIL, 3);

        m->add_ench(mon_enchant(ENCH_FAKE_ABJURATION, 6));

        if (!player_angers_monster(m))
            mpr("You don't feel so good about this...");
    }
    else
        mpr("The air shimmers briefly.");

    return true;
}

static bool _ASMODEUS_evoke(item_def *item, bool* did_work, bool* unevokable)
{
    if (_evoke_sceptre_of_asmodeus())
    {
        make_hungry(200, false, true);
        *did_work = true;
        practise_evoking(1);
    }

    return false;
}

////////////////////////////////////////////////////
static void _CEREBOV_melee_effects(item_def* weapon, actor* attacker,
                                   actor* defender, bool mondied, int dam)
{
    if (dam)
    {
        if (defender->is_player()
            && defender->res_fire() <= 3
            && !you.duration[DUR_FIRE_VULN])
        {
            mpr("The sword of Cerebov burns away your fire resistance.");
            you.increase_duration(DUR_FIRE_VULN, 3 + random2(dam), 50);
        }
        if (defender->is_monster()
            && !mondied
            && !defender->as_monster()->has_ench(ENCH_FIRE_VULN))
        {
            if (you.can_see(*attacker))
            {
                mprf("The sword of Cerebov burns away %s fire resistance.",
                     defender->name(DESC_ITS).c_str());
            }
            defender->as_monster()->add_ench(
                mon_enchant(ENCH_FIRE_VULN, 1, attacker,
                            (3 + random2(dam)) * BASELINE_DELAY));
        }
    }
}

////////////////////////////////////////////////////

static void _CURSES_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "A shiver runs down your spine.");
    if (!unmeld)
    {
        const int pow = random2(9);
        MiscastEffect(&you, nullptr, WIELD_MISCAST,
                      SPTYP_NECROMANCY, pow, random2(70),
                      "the scythe of Curses", NH_NEVER);
    }
}

static void _CURSES_world_reacts(item_def *item)
{
    if (one_chance_in(30))
        curse_an_item(true);
}

static void _CURSES_melee_effects(item_def* weapon, actor* attacker,
                                  actor* defender, bool mondied, int dam)
{
    if (attacker->is_player())
        did_god_conduct(DID_EVIL, 3);
    if (!mondied && !defender->is_player())
    {
        monster* mons = defender->as_monster();
        switch (random2(8))
        {
        case 0:
            mons->add_ench(mon_enchant(ENCH_CORONA, 1, attacker, random2(150)));
            break;
        case 1:
            if(mons_can_be_blinded(mons->type))
                mons->add_ench(mon_enchant(ENCH_BLIND, 1, attacker, random2(100)));
            break;
        case 2:
            mons->add_ench(mon_enchant(ENCH_SLOW, 1, attacker, random2(100)));
            break;
        case 3:
            mons->add_ench(mon_enchant(ENCH_CONFUSION, 1, attacker, random2(60)));
            break;
        case 4:
            if (mons->antimagic_susceptible())
                mons->add_ench(mon_enchant(ENCH_ANTIMAGIC, 1, attacker, random2(320)));
            break;
        default: break;
        }
    }
}

/////////////////////////////////////////////////////

static bool _DISPATER_evoke(item_def *item, bool* did_work, bool* unevokable)
{
    if (!enough_hp(14, true))
    {
        mpr("You're too close to death to use this item.");
        *unevokable = true;
        return true;
    }

    if (!enough_mp(4, false))
    {
        *unevokable = true;
        return true;
    }

    *did_work = true;
    int power = you.skill(SK_EVOCATIONS, 8);

    if (your_spells(SPELL_HURL_DAMNATION, power, false) == spret::abort)
    {
        *unevokable = true;
        return false;
    }

    mpr("You feel the staff feeding on your energy!");
    dec_hp(14, false);
    dec_mp(4);
    make_hungry(100, false, true);
    practise_evoking(random_range(1, 2));

    return false;
}

////////////////////////////////////////////////////

static void _FINISHER_melee_effects(item_def* weapon, actor* attacker,
                                  actor* defender, bool mondied, int dam)
{
    // Can't kill a monster that's already dead.
    // Can't kill a monster if we don't do damage.
    // Don't insta-kill the player
    if (mondied || dam == 0 || defender->is_player())
        return;

    // Chance to insta-kill based on HD. From 1/4 for small HD popcorn down to
    // 1/10 for an Orb of Fire (compare to the 3/20 chance for banish or
    // instant teleport on distortion).
    if (x_chance_in_y(50 - defender->get_hit_dice(), 200))
    {
        monster* mons = defender->as_monster();
        mons->flags |= MF_EXPLODE_KILL;
        mons->hurt(attacker, INSTANT_DEATH);
    }
}

////////////////////////////////////////////////////

// XXX: Staff giving a boost to poison spells is hardcoded in
// player_spec_poison()

static void _OLGREB_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    if (you.can_smell())
        _equip_mpr(show_msgs, "You smell chlorine.");
    else
        _equip_mpr(show_msgs, "The staff glows a sickly green.");
}

static void _OLGREB_unequip(item_def *item, bool *show_msgs)
{
    if (you.can_smell())
        _equip_mpr(show_msgs, "The smell of chlorine vanishes.");
    else
        _equip_mpr(show_msgs, "The staff's sickly green glow vanishes.");
}

static bool _OLGREB_evoke(item_def *item, bool* did_work, bool* unevokable)
{
    if (!enough_mp(4, false))
    {
        *unevokable = true;
        return true;
    }

    if (!x_chance_in_y(you.skill(SK_EVOCATIONS, 100) + 100, 600))
        return false;

    *did_work = true;

    int power = div_rand_round(20 + you.skill(SK_EVOCATIONS, 20), 4);

    // Allow aborting (for example if friendlies are nearby).
    if (your_spells(SPELL_OLGREBS_TOXIC_RADIANCE, power, false) == spret::abort)
    {
        *unevokable = true;
        return false;
    }

    if (x_chance_in_y(you.skill(SK_EVOCATIONS, 100) + 100, 2000))
        your_spells(SPELL_VENOM_BOLT, power, false);

    dec_mp(4);
    make_hungry(50, false, true);
    practise_evoking(1);

    return false;
}

static void _OLGREB_melee_effects(item_def* weapon, actor* attacker,
                                  actor* defender, bool mondied, int dam)
{
    if (defender->alive())
        defender->poison(attacker, 2);
}

////////////////////////////////////////////////////

static void _power_pluses(item_def *item)
{
    item->plus = you.strength() - 10;
}

static void _POWER_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "You sense an aura of extreme power.");
    _power_pluses(item);
}

static void _POWER_world_reacts(item_def *item)
{
    _power_pluses(item);
}

////////////////////////////////////////////////////

static void _SINGING_SWORD_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    bool def_show = true;

    if (show_msgs == nullptr)
        show_msgs = &def_show;

    if (!*show_msgs)
        return;

    if (!item->props.exists(SS_WELCOME_KEY))
    {
        mprf(MSGCH_TALK, "The sword says, \"Hi! I'm the Singing Sword!\"");
        item->props[SS_WELCOME_KEY].get_bool() = true;
    }
    else
        mprf(MSGCH_TALK, "The Singing Sword hums in delight!");

    *show_msgs = false;
}

static void _SINGING_SWORD_unequip(item_def *item, bool *show_msgs)
{
    set_artefact_name(*item, "Singing Sword");
    if (you.duration[DUR_SONG_OF_SLAYING] > 9000)
    {
        _equip_mpr(show_msgs, "The Singing Sword stops its song.", MSGCH_TALK);
        you.props[SONG_OF_SLAYING_KEY] = 0;
        you.duration[DUR_SONG_OF_SLAYING] = 0;
    }
    else
        _equip_mpr(show_msgs, "The Singing Sword sighs.", MSGCH_TALK);
}

static void _SINGING_SWORD_world_reacts(item_def *item)
{
    bool silent = silenced(you.pos());
    string old_name = get_artefact_name(*item);
    string new_name;
    if (silent)
        new_name = "Sulking Sword";
    else
        new_name = "Singing Sword";
    if (old_name != new_name)
    {
        set_artefact_name(*item, new_name);
        you.wield_change = true;
    }
    
    if (there_are_monsters_nearby(true, true, false) && !silent)
    {
        if (you.duration[DUR_SONG_OF_SLAYING] < 9000)
        {
            mprf(MSGCH_TALK, "The Singing Sword starts singing loudly!");
            if (!you.duration[DUR_SONG_OF_SLAYING])
                you.props[SONG_OF_SLAYING_KEY] = 0;
        }
        you.duration[DUR_SONG_OF_SLAYING] = 10000;
        noisy(15, you.pos());
        you.duration[DUR_INVIS] = max(20, you.duration[DUR_INVIS]);
    }
    else if (you.duration[DUR_SONG_OF_SLAYING] > 9000)
    {
        mprf(MSGCH_TALK, "The Singing Sword stops its song.");
        you.props[SONG_OF_SLAYING_KEY] = 0;
        you.duration[DUR_SONG_OF_SLAYING] = 0;
    }
}
////////////////////////////////////////////////////
// Glaive of Prune

static void _PRUNE_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "You feel pruney.");
}

static void _PRUNE_world_reacts(item_def *item)
{
    if (one_chance_in(10))
        did_god_conduct(DID_CHAOS, 1);
}

static void _PRUNE_melee_effects(item_def* weapon, actor* attacker,
                                    actor* defender, bool mondied, int dam)
{
    if(attacker->is_player() && you.form != transformation::tree)
    {
        transform(0, transformation::tree, true, false, nullptr, true);
        you.transform_uncancellable = true;
    }
}

////////////////////////////////////////////////////

static void _TORMENT_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "A terrible, searing pain shoots up your arm!");
}

static void _TORMENT_melee_effects(item_def* weapon, actor* attacker,
                                   actor* defender, bool mondied, int dam)
{
    if (one_chance_in(5))
        torment(attacker, TORMENT_SCEPTRE, attacker->pos());
}

/////////////////////////////////////////////////////

static void _TROG_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "You feel bloodthirsty!");
}

static void _TROG_unequip(item_def *item, bool *show_msgs)
{
    _equip_mpr(show_msgs, "You feel less violent.");
}

////////////////////////////////////////////////////

static bool _WUCAD_MU_evoke(item_def *item, bool* did_work, bool* unevokable)
{
    if (you.magic_points == you.max_magic_points)
    {
        mpr("Your reserves of magic are full.");
        *unevokable = true;
        return true;
    }

    int evo_skill = you.skill(SK_EVOCATIONS, 10);
    int power = evo_skill + 50;
    int limit = you.max_magic_points - you.magic_points;
    int mp_gained = 0;
    
    mpr("You gaze into the orb.");
    for (monster_near_iterator mi(you.pos(), LOS_NO_TRANS); mi; ++mi)
    {
        if (abjuration(power / 2, *mi))
            if (mi->get_experience_level() > (mp_gained + 1)
                    && x_chance_in_y(evo_skill, 270))
                mp_gained++;
        if (mp_gained >= limit)
            break;
    }
    
    inc_mp(mp_gained);
    *did_work = true;
    practise_evoking(1);
    did_god_conduct(DID_CHANNEL, 10, true);

    return false;
}

///////////////////////////////////////////////////

// XXX: Always getting maximal vampiric drain is hardcoded in
// attack::apply_damage_brand()

static void _VAMPIRES_TOOTH_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    if (you.undead_state() == US_ALIVE && !you_foodless())
    {
        _equip_mpr(show_msgs,
                   "You feel a strange hunger, and smell blood in the air...");
    }
    else if (you.species != SP_VAMPIRE)
        _equip_mpr(show_msgs, "You feel strangely empty.");
    // else let player-equip.cc handle message
}

///////////////////////////////////////////////////

static void _VARIABILITY_melee_effects(item_def* weapon, actor* attacker,
                                       actor* defender, bool mondied,
                                       int dam)
{
    if (!mondied && one_chance_in(5))
    {
        const int pow = 75 + random2avg(75, 2);
        if (you.can_see(*attacker))
            mpr("The mace of Variability scintillates.");
        cast_chain_spell(SPELL_CHAIN_OF_CHAOS, pow, attacker);
    }
}

///////////////////////////////////////////////////

static void _ZONGULDROK_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "You sense an extremely unholy aura.");
}

static void _ZONGULDROK_melee_effects(item_def* weapon, actor* attacker,
                                      actor* defender, bool mondied, int dam)
{
    if (attacker->is_player())
    {
        did_god_conduct(DID_EVIL, 3);
        did_god_conduct(DID_CORPSE_VIOLATION, 3);
    }
}

///////////////////////////////////////////////////

static void _GONG_melee_effects(item_def* item, actor* wearer,
                                actor* attacker, bool dummy, int dam)
{
    if (silenced(wearer->pos()))
        return;

    string msg = getSpeakString("shield of the gong");
    if (msg.empty())
        msg = "You hear a strange loud sound.";
    mprf(MSGCH_SOUND, "%s", msg.c_str());

    noisy(40, wearer->pos());
}

///////////////////////////////////////////////////

static void _DEMON_AXE_melee_effects(item_def* item, actor* attacker,
                                     actor* defender, bool mondied, int dam)
{
    if (one_chance_in(10))
    {
        if (monster* mons = attacker->as_monster())
        {
            create_monster(
                mgen_data(summon_any_demon(RANDOM_DEMON_COMMON),
                          SAME_ATTITUDE(mons), mons->pos(), mons->foe)
                .set_summoned(mons, 6, SPELL_SUMMON_DEMON));
        }
        else
            cast_summon_demon(50+random2(100));
    }
}

static monster* _find_nearest_possible_beholder()
{
    for (distance_iterator di(you.pos(), true, true, LOS_RADIUS); di; ++di)
    {
        monster *mon = monster_at(*di);
        if (mon && you.can_see(*mon)
            && you.possible_beholder(mon)
            && mons_is_threatening(*mon))
        {
            return mon;
        }
    }

    return nullptr;
}

static void _DEMON_AXE_world_reacts(item_def *item)
{

    monster* mon = _find_nearest_possible_beholder();

    if (!mon)
        return;

    monster& closest = *mon;

    if (!you.beheld_by(closest))
    {
        mprf("Visions of slaying %s flood into your mind.",
             closest.name(DESC_THE).c_str());

        // The monsters (if any) currently mesmerising the player do not include
        // this monster. To avoid trapping the player, all other beholders
        // are removed.

        you.clear_beholders();
    }

    if (you.confused())
    {
        mpr("Your confusion fades away as the thirst for blood takes over your mind.");
        you.duration[DUR_CONF] = 0;
    }

    you.add_beholder(closest, true);
}

static void _DEMON_AXE_unequip(item_def *item, bool *show_msgs)
{
    if (you.beheld())
    {
        // This shouldn't clear sirens and merfolk avatars, but we lack the
        // information why they behold us -- usually, it's due to the axe.
        // Since unwielding it costs scrolls of rem curse, we might say getting
        // the demon away is enough of a shock to get you back to senses.
        you.clear_beholders();
        mpr("Your thirst for blood fades away.");
    }
}

///////////////////////////////////////////////////

static void _WYRMBANE_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs,
               species_is_draconian(you.species)
                || you.form == transformation::dragon
                   ? "You feel an overwhelming desire to commit suicide."
                   : "You feel an overwhelming desire to slay dragons!");
}

static bool is_dragonkind(const actor *act)
{
    if (mons_genus(act->mons_species()) == MONS_DRAGON
        || mons_genus(act->mons_species()) == MONS_DRAKE
        || mons_genus(act->mons_species()) == MONS_DRACONIAN)
    {
        return true;
    }

    if (act->is_player())
        return you.form == transformation::dragon;

    // Else the actor is a monster.
    const monster* mon = act->as_monster();

    if (mons_is_zombified(*mon)
        && (mons_genus(mon->base_monster) == MONS_DRAGON
            || mons_genus(mon->base_monster) == MONS_DRAKE
            || mons_genus(mon->base_monster) == MONS_DRACONIAN))
    {
        return true;
    }

    if (mons_is_ghost_demon(mon->type)
        && species_is_draconian(mon->ghost->species))
    {
        return true;
    }

    return false;
}

static void _WYRMBANE_melee_effects(item_def* weapon, actor* attacker,
                                    actor* defender, bool mondied, int dam)
{
    if (!defender || !is_dragonkind(defender))
        return;

    // Since the target will become a DEAD MONSTER if it dies due to the extra
    // damage to dragons, we need to grab this information now.
    int hd = min(defender->get_experience_level(), 18);
    string name = defender->name(DESC_THE);

    if (!mondied)
    {
        int bonus_dam = 1 + random2(3 * dam / 2);
        mprf("%s %s%s",
            defender->name(DESC_THE).c_str(),
            defender->conj_verb("convulse").c_str(),
            attack_strength_punctuation(bonus_dam).c_str());

        defender->hurt(attacker, bonus_dam);

        // Allow the lance to charge when killing dragonform felid players.
        mondied = defender->is_player() ? defender->as_player()->pending_revival
                                        : !defender->alive();
    }

    if (!mondied || defender->is_summoned()
        || (defender->is_monster()
            && testbits(defender->as_monster()->flags, MF_NO_REWARD)))
    {
        return;
    }

    // The cap can be reached by:
    // * iron dragon, golden dragon, pearl dragon (18)
    // * Xtahua (19)
    // * bone dragon, Serpent of Hell (20)
    // * Tiamat (22)
    // * pghosts (up to 27)
    dprf("Killed a drac with hd %d.", hd);

    if (weapon->plus < hd)
    {
        weapon->plus++;

        // Including you, if you were a dragonform felid with lives left.
        if (weapon->plus == 18)
        {
            mprf("<white>The lance glows brightly as it skewers %s. You feel "
                 "that it has reached its full power.</white>",
                 name.c_str());
        }
        else
        {
            mprf("<green>The lance glows as it skewers %s.</green>",
                 name.c_str());
        }

        you.wield_change = true;
    }
}

///////////////////////////////////////////////////

static void _UNDEADHUNTER_melee_effects(item_def* item, actor* attacker,
                                        actor* defender, bool mondied, int dam)
{
    if (defender->holiness() & MH_UNDEAD && !one_chance_in(3)
        && !mondied && dam)
    {
        int bonus_dam = random2avg((1 + (dam * 3)), 3);
        mprf("%s %s blasted by disruptive energy%s",
              defender->name(DESC_THE).c_str(),
              defender->conj_verb("be").c_str(),
              attack_strength_punctuation(bonus_dam).c_str());
        defender->hurt(attacker, bonus_dam);
    }
}

///////////////////////////////////////////////////
static void _EOS_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    invalidate_agrid(true);
}

static void _EOS_unequip(item_def *item, bool *show_msgs)
{
    invalidate_agrid(true);
}

static void _EOS_melee_effects(item_def* weapon, actor* attacker,
                                  actor* defender, bool mondied, int dam)
{
    if (!mondied && !defender->is_player())
    {
        monster* mons = defender->as_monster();
        if (x_chance_in_y(1, 3))
            mons->add_ench(mon_enchant(ENCH_CORONA, 1, attacker, random2(80)));
        if (x_chance_in_y(1, 4) && mons_can_be_blinded(mons->type))
            mons->add_ench(mon_enchant(ENCH_BLIND, 1, attacker, random2(50)));
    }
}

///////////////////////////////////////////////////
static void _SHADOWS_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    invalidate_agrid(true);
}

static void _SHADOWS_unequip(item_def *item, bool *show_msgs)
{
    invalidate_agrid(true);
}

///////////////////////////////////////////////////
static void _shillelagh_pluses(item_def *item)
{
    item->plus = you.skill(SK_MACES_FLAILS, 1);
}

static void _DEVASTATOR_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "Time to lay down the shillelagh law.");
    _shillelagh_pluses(item);
}

static void _DEVASTATOR_world_reacts(item_def *item)
{
    _shillelagh_pluses(item);
}

static void _DEVASTATOR_melee_effects(item_def* item, actor* attacker,
                                      actor* defender, bool mondied, int dam)
{
    if (dam && x_chance_in_y(you.skill(SK_MACES_FLAILS, 10) - 50, 360))
        shillelagh(attacker, defender->pos(), dam*2);
}

///////////////////////////////////////////////////
static void _DRAGONSKIN_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "You feel oddly protected from the elements.");
}

static void _DRAGONSKIN_unequip(item_def *item, bool *show_msgs)
{
    _equip_mpr(show_msgs, "You no longer feel protected from the elements.");
}

///////////////////////////////////////////////////
static void _BLACK_KNIGHT_HORSE_world_reacts(item_def *item)
{
    if (one_chance_in(10))
        did_god_conduct(DID_EVIL, 1);
}

///////////////////////////////////////////////////
static void _NIGHT_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    update_vision_range();
    _equip_mpr(show_msgs, "The light fades from your surroundings.");
}

static void _NIGHT_unequip(item_def *item, bool *show_msgs)
{
    update_vision_range();
    _equip_mpr(show_msgs, "The light returns to your surroundings.");
}

///////////////////////////////////////////////////

static void _PLUTONIUM_SWORD_melee_effects(item_def* weapon, actor* attacker,
                                           actor* defender, bool mondied,
                                           int dam)
{
    if (attacker->is_player() && !mondied)
    {
        monster* mons = defender->as_monster();
        if (you.magic_points >= 4)
        {
            const int base_dam = calc_dice(6, 36).roll();
            const int dmg = mons->apply_ac(base_dam);
            if (dmg)
            {
                mprf("%s is blasted with magical radiation%s",
                     mons->name(DESC_THE).c_str(),
                     attack_strength_punctuation(dmg).c_str());
                
                mons->hurt(&you, dmg, BEAM_MMISSILE, KILLED_BY_BEAM, "", "", false);
                
                if (mons->alive() && mons->can_polymorph())
                {
                    if (one_chance_in(12))
                        monster_polymorph(mons, RANDOM_MONSTER, PPT_LESS);
                    
                    mons->malmutate("");
                }
                
                dec_mp(4);
                did_god_conduct(DID_CHAOS, 4);
            }
        }
    }
}

static bool _PLUTONIUM_SWORD_evoke(item_def *item, bool* did_work, bool* unevokable)
{
    if (you.magic_points >= you.max_magic_points)
    {
        mpr("Your reserves of magic are already full.");
        *unevokable = true;
        return true;
    }
    mpr("You channel some magical energy.");
    contaminate_player(1050 + random2(400));
    inc_mp(3 + div_rand_round(you.skill(SK_EVOCATIONS, 10), 40));
    *did_work = true;
    practise_evoking(1);
    did_god_conduct(DID_CHANNEL, 1, true);
    did_god_conduct(DID_CHAOS, 1);

    return false;
}

///////////////////////////////////////////////////

static void _SNAKEBITE_melee_effects(item_def* weapon, actor* attacker,
                                     actor* defender, bool mondied, int dam)
{
    if (!mondied && x_chance_in_y(2, 5))
    {
        curare_actor(attacker, defender, 2, "curare",
                     attacker->name(DESC_PLAIN));
    }
}

///////////////////////////////////////////////////

static void _WOE_melee_effects(item_def* weapon, actor* attacker,
                               actor* defender, bool mondied, int dam)
{
    const char *verb = "bugger", *adv = "";
    switch (random2(8))
    {
    case 0: verb = "cleave", adv = " in twain"; break;
    case 1: verb = "pulverise", adv = " into a thin bloody mist"; break;
    case 2: verb = "hew", adv = " savagely"; break;
    case 3: verb = "fatally mangle", adv = ""; break;
    case 4: verb = "dissect", adv = " like a pig carcass"; break;
    case 5: verb = "chop", adv = " into pieces"; break;
    case 6: verb = "butcher", adv = " messily"; break;
    case 7: verb = "slaughter", adv = " joyfully"; break;
    }
    if (you.see_cell(attacker->pos()) || you.see_cell(defender->pos()))
    {
        mprf("%s %s %s%s.", attacker->name(DESC_THE).c_str(),
             attacker->conj_verb(verb).c_str(),
             (attacker == defender ? defender->pronoun(PRONOUN_REFLEXIVE)
                                   : defender->name(DESC_THE)).c_str(),
             adv);
    }

    if (!mondied)
        defender->hurt(attacker, defender->stat_hp());
}

///////////////////////////////////////////////////

static setup_missile_type _DAMNATION_launch(item_def* item, bolt* beam,
                                           string* ammo_name, bool* returning)
{
    item_def *player_weapon = you.weapon();
    if (player_weapon && is_unrandom_artefact(*player_weapon, UNRAND_DAMNATION))
        you.god_conduct(DID_EVIL, 3);

    ASSERT(beam->item
           && beam->item->base_type == OBJ_MISSILES
           && !is_artefact(*(beam->item)));
    beam->item->special = SPMSL_EXPLODING; // so that it mulches
    beam->item->props[DAMNATION_BOLT_KEY].get_bool() = true;

    beam->name    = "hellfire bolt";
    *ammo_name    = "a hellfire bolt";
    beam->colour  = LIGHTRED;
    beam->glyph   = DCHAR_FIRED_ZAP;

    bolt *expl   = new bolt(*beam);
    expl->flavour = BEAM_DAMNATION;
    expl->is_explosion = true;
    expl->damage = dice_def(3, 10);
    expl->name   = "hellfire";

    beam->special_explosion = expl;
    return SM_FINISHED;
}

///////////////////////////////////////////////////

/**
 * Calculate the bonus damage that the Elemental Staff does with an attack of
 * the given flavour.
 *
 * @param flavour   The elemental flavour of attack; may be BEAM_NONE for earth
 *                  (physical) attacks.
 * @param defender  The victim of the attack. (Not const because checking res
 *                  may end up IDing items on monsters... )
 * @return          The amount of damage that the defender will receive.
 */
static int _calc_elemental_staff_damage(beam_type flavour,
                                        actor* defender)
{
    const int base_bonus_dam = 10 + random2(15);

    if (flavour == BEAM_NONE) // earth
        return defender->apply_ac(base_bonus_dam);

    return resist_adjust_damage(defender, flavour, base_bonus_dam);
}

static void _ELEMENTAL_STAFF_melee_effects(item_def*, actor* attacker,
                                           actor* defender, bool mondied,
                                           int)
{
    const int evoc = attacker->skill(SK_EVOCATIONS, 27);
    if (mondied || !(x_chance_in_y(evoc, 27*27) || x_chance_in_y(evoc, 27*27)))
        return;

    const char *verb = nullptr;
    beam_type flavour = BEAM_NONE;

    switch (random2(4))
    {
    case 0:
        verb = "burn";
        flavour = BEAM_FIRE;
        break;
    case 1:
        verb = "freeze";
        flavour = BEAM_COLD;
        break;
    case 2:
        verb = "electrocute";
        flavour = BEAM_ELECTRICITY;
        break;
    default:
        dprf("Bad damage type for elemental staff; defaulting to earth");
        // fallthrough to earth
    case 3:
        verb = "crush";
        break;
    }

    const int bonus_dam = _calc_elemental_staff_damage(flavour, defender);

    if (bonus_dam <= 0)
        return;

    mprf("%s %s %s%s",
         attacker->name(DESC_THE).c_str(),
         attacker->conj_verb(verb).c_str(),
         (attacker == defender ? defender->pronoun(PRONOUN_REFLEXIVE)
                               : defender->name(DESC_THE)).c_str(),
         attack_strength_punctuation(bonus_dam).c_str());

    defender->hurt(attacker, bonus_dam, flavour);

    if (defender->alive() && flavour != BEAM_NONE)
        defender->expose_to_element(flavour, 2);
}

///////////////////////////////////////////////////

static void _ARC_BLADE_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "The arc blade crackles to life.");
}

static void _ARC_BLADE_unequip(item_def *item, bool *show_msgs)
{
    _equip_mpr(show_msgs, "The arc blade stops crackling.");
}

static void _ARC_BLADE_melee_effects(item_def* weapon, actor* attacker,
                                     actor* defender, bool mondied,
                                     int dam)
{
    if (one_chance_in(3))
    {
        const int pow = 100 + random2avg(100, 2);

        if (you.can_see(*attacker))
            mpr("The arc blade crackles.");
        else
            mpr("You hear the crackle of electricity.");

        cast_discharge(pow, *attacker, false, false);
    }
}

///////////////////////////////////////////////////

static void _SPELLBINDER_melee_effects(item_def* weapon, actor* attacker,
                                       actor* defender, bool mondied,
                                       int dam)
{
    // Only cause miscasts if the target has magic to disrupt.
    if (defender->antimagic_susceptible()
        && !mondied)
    {
        const int pow = random2(9);
        MiscastEffect(defender, defender, MELEE_MISCAST,
                      SPTYP_RANDOM, pow, random2(70),
                      "the demon whip \"Spellbinder\"", NH_NEVER);
    }
}

///////////////////////////////////////////////////

static void _ORDER_melee_effects(item_def* item, actor* attacker,
                                         actor* defender, bool mondied, int dam)
{
    if (!mondied)
    {
        string msg = "";
        int silver_dam = silver_damages_victim(defender, dam, msg);
        if (silver_dam)
        {
            if (you.can_see(*defender))
                mpr(msg);
            defender->hurt(attacker, silver_dam);
        }
    }
}

///////////////////////////////////////////////////

static void _FIRESTARTER_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "You are filled with an inner flame.");
}

static void _FIRESTARTER_unequip(item_def *item, bool *show_msgs)
{
    _equip_mpr(show_msgs, "Your inner flame fades away.");
}

static void _FIRESTARTER_melee_effects(item_def* weapon, actor* attacker,
                                   actor* defender, bool mondied, int dam)
{
    if (dam)
    {
        if (defender->is_monster()
            && !mondied
            && !defender->as_monster()->has_ench(ENCH_INNER_FLAME))
        {
            mprf("%s is filled with an inner flame.",
                 defender->name(DESC_THE).c_str());
            defender->as_monster()->add_ench(
                mon_enchant(ENCH_INNER_FLAME, 0, attacker,
                            (3 + random2(dam)) * BASELINE_DELAY));
        }
    }
}

///////////////////////////////////////////////////

static void _SAPPHIRE_DAGGER_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "The dagger glows with an icy blue light!");
}

static void _SAPPHIRE_DAGGER_unequip(item_def *item, bool *show_msgs)
{
    _equip_mpr(show_msgs, "The dagger stops glowing.");
}

static void _SAPPHIRE_DAGGER_melee_effects(item_def* weapon, actor* attacker,
                                   actor* defender, bool mondied, int dam)
{
    if (dam)
    {
        if (defender->is_monster() && !mondied)
        {
            monster* mons = defender->as_monster();
            
            if(!mons->has_ench(ENCH_FROZEN))
            {
                mprf("%s is encased in ice.",
                     defender->name(DESC_THE).c_str());
                mons->add_ench(mon_enchant(
                        ENCH_FROZEN, 0, attacker, (5 + random2(dam)) * BASELINE_DELAY));
            }
            
            if (!(defender->res_cold() > 0 || defender->is_stationary()))
            {
                mons->add_ench(mon_enchant(ENCH_SLOW, 0, attacker, 11));
                mprf("%s is slowed by the cold.",
                     defender->name(DESC_THE).c_str());
            }

        }
        else if (defender->is_player()
            && !you.duration[DUR_FROZEN])
        {
            mprf(MSGCH_WARN, "You are encased in ice.");
            you.increase_duration(DUR_FROZEN, 5 + random2(dam));
        }
    }
}

///////////////////////////////////////////////////

static void _FLAMING_DEATH_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "The scimitar bursts into red hot flame!");
}

static void _FLAMING_DEATH_unequip(item_def *item, bool *show_msgs)
{
    _equip_mpr(show_msgs, "The scimitar stops flaming.");
}

static void _FLAMING_DEATH_melee_effects(item_def* weapon, actor* attacker,
                                   actor* defender, bool mondied, int dam)
{
    if (!mondied && (dam > 2 && one_chance_in(3)))
    {
        if (defender->is_player())
            napalm_player(random2avg(7, 3) + 1, attacker->name(DESC_A, true));
        else
        {
            napalm_monster(
                defender->as_monster(),
                attacker,
                min(4, 1 + random2(attacker->get_hit_dice())/2));
        }
    }
}

///////////////////////////////////////////////////

static void _MAJIN_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    if (!you.max_magic_points)
        return;

    const bool should_msg = !show_msgs || *show_msgs;
    _equip_mpr(show_msgs, "You feel a darkness envelop your magic.");

    if (!item->props.exists(MB_WELCOME_KEY) && should_msg)
    {
        const string msg = "A voice whispers, \"" +
                           getSpeakString("majin-bo greeting") + "\"";
        mprf(MSGCH_TALK, "%s", msg.c_str());
        item->props[MB_WELCOME_KEY].get_bool() = true;
    }
}

static void _MAJIN_unequip(item_def *item, bool *show_msgs)
{
    if (you.max_magic_points)
    {
        _equip_mpr(show_msgs,
                   "The darkness slowly releases its grasp on your magic.");
    }
}

///////////////////////////////////////////////////

static int _octorings_worn()
{
    int worn = 0;

    for (int i = EQ_LEFT_RING; i < NUM_EQUIP; ++i)
    {
        if (you.melded[i] || you.equip[i] == -1)
            continue;

        item_def& ring = you.inv[you.equip[i]];
        if (is_unrandom_artefact(ring, UNRAND_OCTOPUS_KING_RING))
            worn++;
    }

    return worn;
}

static void _OCTOPUS_KING_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    int rings = _octorings_worn();

    if (rings == 8)
        _equip_mpr(show_msgs, "You feel like a king!");
    else if (rings)
        _equip_mpr(show_msgs, "You feel regal.");
    item->plus = 8 + 6 * rings;
}

static void _OCTOPUS_KING_world_reacts(item_def *item)
{
    item->plus = 8 + 6 * _octorings_worn();
}

///////////////////////////////////////////////////

static void _CAPTAIN_melee_effects(item_def* weapon, actor* attacker,
                                actor* defender, bool mondied, int dam)
{
    // Player disarming sounds like a bad idea; monster-on-monster might
    // work but would be complicated.
    if (coinflip()
        && dam >= (3 + random2(defender->get_hit_dice()))
        && !x_chance_in_y(defender->get_hit_dice(), random2(20) + dam*4)
        && attacker->is_player()
        && defender->is_monster()
        && !mondied)
    {
        item_def *wpn = defender->as_monster()->disarm();
        if (wpn)
        {
            mprf("The captain's cutlass flashes! You lacerate %s!!",
                defender->name(DESC_THE).c_str());
            mprf("%s %s falls to the floor!",
                apostrophise(defender->name(DESC_THE)).c_str(),
                wpn->name(DESC_PLAIN).c_str());
            defender->hurt(attacker, 18 + random2(18));
        }
    }
}

///////////////////////////////////////////////////

static void _FENCERS_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "En garde!");
}

///////////////////////////////////////////////////

static void _ETHERIC_CAGE_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "You sense a greater flux of ambient magical fields.");
}

///////////////////////////////////////////////////

static void _ETERNAL_TORMENT_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    calc_hp();
}

static void _ETERNAL_TORMENT_world_reacts(item_def *item)
{
    if (one_chance_in(10))
        did_god_conduct(DID_EVIL, 1);
}


static void _ETERNAL_TORMENT_unequip(item_def *item, bool *show_msgs)
{
    calc_hp();
}

///////////////////////////////////////////////////

static void _VINES_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "The vines latch onto your body!");
}

static void _VINES_unequip(item_def *item, bool *show_msgs)
{
    _equip_mpr(show_msgs, "The vines fall away from your body!");
}

///////////////////////////////////////////////////

static void _KRYIAS_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "Your attunement to healing potions increases.");
}

static void _KRYIAS_unequip(item_def *item, bool *show_msgs)
{
    _equip_mpr(show_msgs, "Your attunement to healing potions decreases.");
}

///////////////////////////////////////////////////

static void _FROSTBITE_melee_effects(item_def* weapon, actor* attacker,
                                    actor* defender, bool mondied, int dam)
{
    coord_def spot = defender->pos();
    if (!cell_is_solid(spot)
        && !cloud_at(spot)
        && one_chance_in(5))
    {
         place_cloud(CLOUD_COLD, spot, random_range(4, 8), attacker, 0);
    }
}

///////////////////////////////////////////////////

static void _LEECH_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "Your kris thirsts for magical power.");
}

static void _LEECH_melee_effects(item_def* item, actor* attacker,
                                        actor* defender, bool mondied, int dam)
{
    if (attacker->is_player())
    {
        monster* mons = defender->as_monster();
        int mana_gain = min(dam, mons->max_hit_points);
        mana_gain = random2(mana_gain);
        
        if (!mana_gain)
            return;
        
        // bonus dmg to summons instead of mana gain
        if (mons->has_ench(ENCH_ABJ)
                || mons->has_ench(ENCH_FAKE_ABJURATION)
                || mons->has_ench(ENCH_SHORT_LIVED)
                || mons->type == MONS_FULMINANT_PRISM)
        {
            if (!mondied)
            {
                mprf("%s %s%s",
                    defender->name(DESC_THE).c_str(),
                    defender->conj_verb("shudders").c_str(),
                    attack_strength_punctuation(mana_gain).c_str());
                defender->hurt(attacker, mana_gain);
            }
            return;
        }
        
        mprf("You draw magical power from %s.",
            (attacker == defender ? defender->pronoun(PRONOUN_REFLEXIVE)
                                : defender->name(DESC_THE)).c_str());
        inc_mp(dam);
    }
}

///////////////////////////////////////////////////

static void _THERMIC_ENGINE_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "The engine hums to life!");
    item->plus = 2;
}

static void _THERMIC_ENGINE_unequip(item_def *item, bool *show_msgs)
{
    _equip_mpr(show_msgs, "The engine shudders to a halt.");
    item->plus = 2;
}

static void _THERMIC_ENGINE_melee_effects(item_def* weapon, actor* attacker,
                                   actor* defender, bool mondied, int dam)
{
    if (weapon->plus < 14)
    {
        weapon->plus += 2;

        if (weapon->plus > 14)
            weapon->plus = 14;

        you.wield_change = true;
    }

    if (mondied)
        return;

    // the flaming brand has already been applied at this point
    const int bonus_dam = resist_adjust_damage(defender, BEAM_COLD,
                                               random2(dam) / 2 + 1);
    if (bonus_dam > 0)
    {
        mprf("%s %s %s.",
            attacker->name(DESC_THE).c_str(),
            attacker->conj_verb("freeze").c_str(),
            (attacker == defender ? defender->pronoun(PRONOUN_REFLEXIVE)
                                : defender->name(DESC_THE)).c_str());

        defender->hurt(attacker, bonus_dam, BEAM_COLD);
        if (defender->alive())
            defender->expose_to_element(BEAM_COLD, 2);
    }
}

static void _THERMIC_ENGINE_world_reacts(item_def *item)
{
    if (item->plus > 2)
    {
        item->plus -= div_rand_round(you.time_taken, BASELINE_DELAY);

        if (item->plus < 2)
            item->plus = 2;

        you.wield_change = true;
    }
}

///////////////////////////////////////////////////

static void _SERPENTINE_SLING_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "The sling writhes as if it's alive!");
}

///////////////////////////////////////////////////

static setup_missile_type _SCORPIO_launch(item_def* item, bolt* beam,
                                           string* ammo_name, bool* returning)
{
    beam->origin_spell = SPELL_BECKONING;
    return SM_CONTINUE;
}

///////////////////////////////////////////////////

static void _STORM_BOW_equip(item_def *item, bool *show_msgs, bool unmeld)
{
    _equip_mpr(show_msgs, "Electricity crackles across the bow's limbs.");
}

static void _STORM_BOW_unequip(item_def *item, bool *show_msgs)
{
    _equip_mpr(show_msgs, "The bow stops crackling with electricity.");
}

static setup_missile_type _STORM_BOW_launch(item_def* item, bolt* beam,
                                           string* ammo_name, bool* returning)
{
    if (one_chance_in(2))
        beam->origin_spell = SPELL_CONJURE_BALL_LIGHTNING;
    return SM_CONTINUE;
}

///////////////////////////////////////////////////

static void _BUTTERFLY_SWORD_melee_effects(item_def* weapon, actor* attacker,
                                    actor* defender, bool mondied, int dam)
{
    create_monster(
        mgen_data(MONS_BUTTERFLY, BEH_NEUTRAL, attacker->pos(), MG_AUTOFOE)
        .set_summoned(&you, 2, SPELL_SUMMON_BUTTERFLIES)
        );
}

////////////////////////////////////////////////////
// Staff of Battle

static void _BATTLE_world_reacts(item_def *item)
{
    if (!find_spectral_weapon(&you) && there_are_monsters_nearby(true, true, false))
        your_spells(SPELL_SPECTRAL_WEAPON, 0, false);
}

////////////////////////////////////////////////////

static void _HENDRICKS_melee_effects(item_def* weapon, actor* attacker,
                                    actor* defender, bool mondied, int dam)
{
    if (coinflip())
    {
        noisy(17 + random2(8), attacker->pos());
        
        if (defender->is_monster() && defender->alive())
        {
            monster* mon = defender->as_monster();
            
            if (coinflip() && mons_can_be_blinded(mon->type))
            {
                if (!mon->has_ench(ENCH_BLIND))
                    mprf("%s vision becomes distorted and hazy.",
                         apostrophise(defender->name(DESC_THE)).c_str());
                mon->add_ench(mon_enchant(ENCH_BLIND, 1, attacker, 10 + random2(60)));
            }
        }
    }
}

static bool _HENDRICKS_evoke(item_def *item, bool* did_work, bool* unevokable)
{
    noisy(17 + random2(8), you.pos());
    int axe_skill = you.skill(SK_AXES, 10);
    if (x_chance_in_y(axe_skill, 100) && enough_mp(1, false))
    {
        int power = div_rand_round(axe_skill, 7);
        mpr("You strum your axe, and make an amazing distorted noise!");
        dec_mp(1);
        mass_enchantment(ENCH_INSANE, power, 0);
    }
    else
        mpr("You ineptly strum your axe, and merely make distorted noise.");
    *did_work = true;

    return false;
}



