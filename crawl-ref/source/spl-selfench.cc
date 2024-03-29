/**
 * @file
 * @brief Self-enchantment spells.
**/

#include "AppHdr.h"

#include "spl-selfench.h"

#include <cmath>

#include "areas.h"
#include "art-enum.h"
#include "cloud.h" // tree form
#include "coordit.h" // radius_iterator
#include "god-conduct.h"
#include "god-passive.h"
#include "hints.h"
#include "items.h" // stack_iterator
#include "libutil.h"
#include "message.h"
#include "output.h"
#include "prompt.h"
#include "religion.h"
#include "spl-util.h"
#include "stringutil.h"
#include "transform.h"
#include "tilepick.h"
#include "view.h"

int allowed_deaths_door_hp()
{
    int hp = calc_spell_power(SPELL_DEATHS_DOOR, true) / 10;

    return max(hp, 1);
}

spret cast_deaths_door(int pow, bool fail)
{
    fail_check();
    mpr("You stand defiantly in death's doorway!");
    mprf(MSGCH_SOUND, "You seem to hear sand running through an hourglass...");

    you.set_duration(DUR_DEATHS_DOOR, 10 + random2avg(13, 3)
                                       + (random2(pow) / 10));

    calc_hp(false, true);

    if (you.duration[DUR_DEATHS_DOOR] > 25 * BASELINE_DELAY)
        you.duration[DUR_DEATHS_DOOR] = (23 + random2(5)) * BASELINE_DELAY;
    return spret::success;
}

void remove_ice_armour()
{
    mprf(MSGCH_DURATION, "Your icy armour melts away.");
    you.redraw_armour_class = true;
    you.duration[DUR_ICY_ARMOUR] = 0;
}

spret ice_armour(int pow, bool fail)
{
    fail_check();

    if (you.duration[DUR_ICY_ARMOUR])
        mpr("Your icy armour thickens.");
    else if (you.form == transformation::ice_beast)
        mpr("Your icy body feels more resilient.");
    else
        mpr("A cloud of ice covers your body!");

    int dur = 2 + div_rand_round(pow, 20);
    you.increase_duration(DUR_ICY_ARMOUR, dur, dur);
    you.props[ICY_ARMOUR_KEY] = pow;
    you.redraw_armour_class = true;

    return spret::success;
}

spret deflection(int pow, bool fail)
{
    bool deflecting = you.attribute[ATTR_DEFLECT_MISSILES];
    if (deflecting)
        if(!yesno("Really dispel your missile deflection?", true, 'n'))
        {
            canned_msg(MSG_OK);
            return spret::abort;
        }
    
    fail_check();
    
    if(!deflecting)
    {
        you.attribute[ATTR_DEFLECT_MISSILES] = 1;
        mpr("You feel very safe from missiles.");
    }
    else
    {
        you.attribute[ATTR_DEFLECT_MISSILES] = 0;
        mpr("You carefully dispel your deflective shield.");
    }

    return spret::success;
}

spret cast_regen(int pow, bool fail)
{
    fail_check();
    you.increase_duration(DUR_REGENERATION, 5 + roll_dice(2, pow / 3 + 1), 100,
                          "Your skin crawls.");

    return spret::success;
}

spret cast_revivification(int pow, bool fail)
{
    fail_check();
    mpr("Your body is healed in an amazingly painful way.");

    const int loss = 6 + binomial(9, 8, pow);
    dec_max_hp(loss * you.hp_max / 100);
    set_hp(you.hp_max);

    if (you.duration[DUR_DEATHS_DOOR])
    {
        mprf(MSGCH_DURATION, "Your life is in your own hands once again.");
        // XXX: better cause name?
        paralyse_player("Death's Door abortion");
        you.duration[DUR_DEATHS_DOOR] = 0;
    }
    return spret::success;
}

spret cast_swiftness(int power, bool fail)
{
    fail_check();

    if (you.in_liquid())
    {
        // Hint that the player won't be faster until they leave the liquid.
        mprf("The %s foams!", you.in_water() ? "water"
                                             : "liquid ground");
    }

    you.set_duration(DUR_SWIFTNESS, 12 + random2(power)/2, 30,
                     "You feel quick.");
    you.attribute[ATTR_SWIFTNESS] = you.duration[DUR_SWIFTNESS];

    return spret::success;
}

int cast_selective_amnesia(const string &pre_msg)
{
    if (you.spell_no == 0)
    {
        canned_msg(MSG_NO_SPELLS);
        return 0;
    }

    int keyin = 0;
    spell_type spell;
    int slot;

    // Pick a spell to forget.
    keyin = list_spells(false, false, false, "Forget which spell?");
    redraw_screen();

    if (isaalpha(keyin))
    {
        spell = get_spell_by_letter(keyin);
        slot = get_spell_slot_by_letter(keyin);

        if (spell != SPELL_NO_SPELL)
        {
            const string prompt = make_stringf(
                    "Forget %s, freeing %d spell level%s for a total of %d?%s",
                    spell_title(spell), spell_levels_required(spell),
                    spell_levels_required(spell) != 1 ? "s" : "",
                    player_spell_levels() + spell_levels_required(spell),
                    you.spell_library[spell] ? "" :
                    " This spell is not in your library!");

            if (yesno(prompt.c_str(), true, 'n', false))
            {
                if (!pre_msg.empty())
                    mpr(pre_msg);
                del_spell_from_memory_by_slot(slot);
                return 1;
            }
        }
    }

    return -1;
}

spret cast_infusion(int pow, bool fail)
{
    fail_check();
    if (!you.duration[DUR_INFUSION])
        mpr("You begin infusing your attacks with magical energy.");
    else
        mpr("You extend your infusion's duration.");

    you.increase_duration(DUR_INFUSION,  8 + roll_dice(2, pow/2), 30);
    you.props["infusion_power"] = pow;

    return spret::success;
}

spret cast_flaming_arrows(int pow, bool fail)
{
    fail_check();
    if (!you.duration[DUR_FIRE_ARROW])
        mpr("You begin infusing your projectiles with flame.");
    else
        mpr("You extend your fiery infusion's duration.");

    you.increase_duration(DUR_FIRE_ARROW,  8 + roll_dice(2, pow/2), 30);
    you.props["fire_arrow_power"] = pow;
    
    if(you.duration[DUR_PIERCING_SHOT])
        you.duration[DUR_PIERCING_SHOT] = 1;

    return spret::success;
}

spret cast_song_of_slaying(int pow, bool fail)
{
    fail_check();

    if (you.duration[DUR_SONG_OF_SLAYING])
        mpr("You start a new song!");
    else
        mpr("You start singing a song of slaying.");

    you.set_duration(DUR_SONG_OF_SLAYING, 20 + random2avg(pow, 2));

    you.props[SONG_OF_SLAYING_KEY] = 1;
    you.redraw_armour_class = true;
    return spret::success;
}

spret cast_silence(int pow, bool fail)
{
    fail_check();
    mpr("A profound silence engulfs you.");

    you.increase_duration(DUR_SILENCE, 10 + pow/4 + random2avg(pow/2, 2), 100);
    invalidate_agrid(true);

    if (you.beheld())
        you.update_beholders();

    learned_something_new(HINT_YOU_SILENCE);
    return spret::success;
}

spret cast_liquefaction(int pow, bool fail)
{
    fail_check();
    flash_view_delay(UA_PLAYER, BROWN, 80);
    flash_view_delay(UA_PLAYER, YELLOW, 80);
    flash_view_delay(UA_PLAYER, BROWN, 140);

    mpr("The ground around you becomes liquefied!");

    you.increase_duration(DUR_LIQUEFYING, 10 + random2avg(pow, 2), 100);
    invalidate_agrid(true);
    return spret::success;
}

spret cast_shroud_of_golubria(int pow, bool fail)
{
    fail_check();
    if (you.duration[DUR_SHROUD_OF_GOLUBRIA])
        mpr("You renew your shroud.");
    else
        mpr("Space distorts slightly along a thin shroud covering your body.");

    you.increase_duration(DUR_SHROUD_OF_GOLUBRIA, 7 + roll_dice(2, pow), 50);
    return spret::success;
}

spret cast_transform(int pow, transformation which_trans, bool fail)
{
    if (!transform(pow, which_trans, false, true)
        || !check_form_stat_safety(which_trans))
    {
        return spret::abort;
    }
    
    if(which_trans == transformation::tree)
    {
        const cloud_type cloud = cloud_type_at(you.pos());
        if (is_damaging_cloud(cloud, false)
            // Tree form is immune to these two.
            && cloud != CLOUD_MEPHITIC && cloud != CLOUD_POISON && cloud != CLOUD_MIASMA
            && !yesno(make_stringf("Really become a tree while standing in a cloud of %s?",
                                   cloud_type_name(cloud).c_str()).c_str(), false, 'n'))
        {
            canned_msg(MSG_OK);
            return spret::abort;
        }
    }

    fail_check();
    transform(pow, which_trans);
    if (which_trans == transformation::tree)
        you.transform_uncancellable = true;
    
    return spret::success;
}

spret cast_piercing_shot(int pow, bool fail)
{
    fail_check();
    if (!you.duration[DUR_PIERCING_SHOT])
        mpr("You begin enchanting your missiles to pierce their targets.");
    else
        mpr("You renew your piercing charm.");

    // Piercing Shot and Portal Projectile are mutually exclusive.
    you.attribute[ATTR_PORTAL_PROJECTILE] = 0;
    you.duration[DUR_PORTAL_PROJECTILE] = 0;
    you.increase_duration(DUR_PIERCING_SHOT, 3 + random2(pow / 2) + random2(pow / 5), 50);
    
    if(you.duration[DUR_FIRE_ARROW])
        you.duration[DUR_FIRE_ARROW] = 1;
    
    return spret::success;
}
