/**
 * @file
 * @brief Monster abilities.
**/

#pragma once

class monster;
struct bolt;

bool mon_special_ability(monster* mons);

void tiamat_change_colour(monster* drac);

bool ugly_thing_mutate(monster& ugly, bool force = true);
bool slime_creature_polymorph(monster& slime);
void merge_ench_durations(monster& initial, monster& merge_to, bool usehd = false);

bool lost_soul_revive(monster& mons, killer_type killer);

void treant_release_fauna(monster& mons);
void check_grasping_roots(actor& act, bool quiet = false);

void guardian_golem_bond(monster& mons);
