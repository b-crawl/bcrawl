#pragma once
// Used only in world_reacts()
void player_reacts();
void player_reacts_to_monsters();

void regenerate_hp_and_mp(int delay, bool apply_bonuses = true);

// Only function other than decrement_duratons() which uses decrement_a_duration()
void extract_manticore_spikes(const char* endmsg);
