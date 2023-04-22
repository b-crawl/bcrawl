#pragma once

#include "spl-cast.h"

spret cast_time_stop(bool fail);

spret cast_blink(bool fail = false);
spret cast_controlled_blink(bool fail = false, bool safe = true, int power = 200);
void uncontrolled_blink(bool override_stasis = false);
spret controlled_blink(bool fail, bool safe_cancel = true, int power = 200);
spret frog_hop(bool fail);
void wizard_blink();

void you_teleport();
void you_teleport_now(bool wizard_tele = false, bool teleportitis = false);
bool you_teleport_to(const coord_def where,
                     bool move_monsters = false);

spret cast_portal_projectile(int pow, bool fail);

struct bolt;
spret cast_apportation(int pow, bolt& beam, bool fail);
spret cast_golubrias_passage(const coord_def& where, bool fail);

spret cast_dispersal(int pow, bool fail);

bool beckon(actor &beckoned, const bolt &path);
spret blinkbolt(int power, bolt &beam, bool fail);
