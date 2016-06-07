function gauntlet_portal(e, ptype)
  local timeout_turns = crawl.random_range(600, 800)

  local messager =
    timed_msg {
      initmsg = { "You hear the whispers of an unnatural audience.",
                  "There is an entrance to a demonic gauntlet on this level. "
                  .. "Hurry and find it before they move on!" },
      finalmsg = "$F{The}'s impatience is almost overwhelming!",

      verb = 'restlessness',
      noisemaker = 'crowd',
        ranges = {
          { 5000, 'growing ' },  { 4000, 'spreading ' },
          { 2500, 'anxious ' }, { 1500, 'fierce ' },
          { 0, 'frothing ' }
      }
    }

  e.lua_marker('O',
      timed_marker {
        disappear = "The demonic chitterings cease.",
        entity = 'crowd',
        bailey_type = ptype,
        turns = timeout_turns,
        single_timed = true,
        floor = "stone_arch",
        feat_tile = "dngn_portal_expired",
        msg = messager })
  e.tags("uniq_gauntlet chance_gauntlet")
  e.chance(400)

  e.kfeat("O = enter_gauntlet")
  e.kitem("$ = $ no_pickup")
  e.tile("b = dngn_crystal_red")
  e.tile("n = dngn_transparent_stone_yellow")
end

function audience_members(e)
   aw = "generate_awake"
   audience = aw .. " lesser demon w:25 / " .. aw .. " common demon w:15 / " ..
              aw .. " greater demon w:2 / " .. aw .. " corrupter w:2 / " ..
              aw .. " rakshasa w:2 / " .. aw .. " efreet w:2 / " ..
              aw .. " boggart w:2 / " .. aw .. " worldbinder w:2"
end

-- Deliberately excluded from the below groupings:
-- * Yaktaur captains, mid-tier dragons / giants (too common)
-- * Convokers, Wardens, deep elf elementalists (bad layout fits)
-- * Non-draconian Zot enemies (dilution, special status)
-- * Deep elf sorcerers and demonologists (banishment)
-- At any rate, most layouts should feel free to use this super long list,
-- with some combinations of tiers 1/2 and 4/5.
function gauntlet_tiers(e)
   tier_one = "catoblepas w:5 / torpor snail w:5 / entropy weaver / " ..
               "moth of wrath w:5 / deep elf death mage / flayed ghost / " ..
               "eidolon / phantasmal warrior / giant eyeball w:5 / " ..
               "hellion w:5 / shadow demon w:5 / apocalypse crab w:5"
   tier_two = "emperor scorpion / spark wasp / ghost moth / satyr / " ..
              "greater naga / shambling mangrove / thorn hunter / " ..
              "spriggan berserker / spriggan air mage / azure jelly / " ..
              "very ugly thing w:5 / dancing weapon ; glaive good_item / " ..
              "vampire knight / jiangshi / green death / tentacled starspawn"
   tier_three = "sphinx / storm dragon / shadow dragon / " ..
                "tengu reaver / spriggan defender / deep elf annihilator /" ..
                "deep elf high priest / war gargoyle / crystal guardian / " ..
                "revenant / ancient champion / balrug / blizzard demon / " ..
                "reaper / tentacled monstrosity / acid blob / " ..
                "glowing shapeshifter hd:17 / starcursed mass w:5"
   tier_four = "golden dragon / quicksilver dragon / iron dragon / " ..
               "caustic shrike / titan / juggernaut w:5 / " ..
               "deep elf blademaster / deep elf master archer / " ..
               "green draconian monk w:5 / black draconian knight w:5 / " ..
               "yellow draconian scorcher w:5 / purple draconian zealot w:5 / " ..
               "white draconian annihilator w:5 / ancient lich / " ..
               "curse skull w:5 / executioner w:5 / hellephant / " ..
               "warmonger / blood saint / corrupter"
   tier_five = "pearl dragon / shard shrike / doom hound / " ..
               "brimstone fiend / ice fiend / shadow fiend"
end

function gauntlet_outside_clouds(e, decor_glyph, weird_glyph, decor_size, weird_size)
   -- First cloud type shouldn't harm potential decorative plants.
   local cloud_type_a = {"sparse dust", "sparse dust",
                         "magical condensation", "magical condensation",
                         "thin mist", "thin mist", "translocational energy"}
   local cloud_type_b = {"seething chaos", "negative energy", "acidic fog",
                         "mutagenic fog", "foul pestilence"}
   brume = util.random_from(cloud_type_a)
   smaze = util.random_from(cloud_type_b)
   e.lua_marker(decor_glyph, fog_machine { cloud_type = brume,
                pow_min = 10, pow_max = 30, delay_min = 10, delay_max = 30,
                size = decor_size, walk_dist = 4, spread_rate = 8 })
   e.lua_marker(weird_glyph, fog_machine { cloud_type = smaze,
                pow_min = 20, pow_max = 40, delay_min = 10, delay_max = 30,
                size = weird_size, walk_dist = 4, spread_rate = 8, excl_rad = -1 })
end

function gauntlet_corpse_mound(e, victim_glyph, venatio_glyph)
   -- Weigh down the corpses of races that don't appear in the dungeon,
   -- weigh up the divine and the demonic.
   victims = "centaur corpse / deep dwarf corpse / elf corpse w:20 / " ..
             "demigod corpse w:30 / draconian corpse / " ..
             "demonspawn corpse w:30 / felid corpse w:5 / " ..
             "formicid corpse w:5 / ghoul corpse / " ..
             "halfling corpse w:5 / orc corpse / human corpse / " ..
             "kobold corpse / merfolk corpse / minotaur corpse / " ..
             "naga corpse / octopode corpse w:5 / spriggan corpse / " ..
             "tengu corpse / troll corpse"
   venatio = "catoblepas corpse / hydra corpse / kraken corpse / " ..
             "ghost moth corpse / giant orange brain corpse / " ..
             "caustic shrike corpse / iron dragon corpse / " ..
             "golden dragon corpse / quicksilver dragon corpse / " ..
             "juggernaut corpse / iron giant corpse / " ..
             "apocalypse crab corpse / hellephant corpse"
   dgn.delayed_decay(e, victim_glyph, victims)
   dgn.delayed_decay(e, venatio_glyph, venatio)
end

function teleporter_gauntlet_switch_fn(data, triggerable, triggerer, marker, ev)
  local position = dgn.point(marker:pos())
  my_slaves = dgn.find_marker_positions_by_prop("teleport_spot",
                                                data.teleport_spot)

  if you.teleport_to(my_slaves[1].x, my_slaves[1].y) then
    crawl.mpr("Your surroundings suddenly seem different!")
  else
    crawl.mpr("There is a strange hissing noise.")
  end
end

function gauntlet_teleporters(e, teleporter_glyphs, teleport_spot_glyphs)
  -- Decorative indicators.
  e.kfeat(teleporter_glyphs .. " = teleporter")
  e.kfeat(teleport_spot_glyphs .. " = floor")
  e.ftile(teleporter_glyphs .. " = floor_marble")
  e.ftile(teleport_spot_glyphs .. " = floor_marble")
  e.colour(teleport_spot_glyphs .. " = lightmagenta")

  local teleporters = teleporter_glyphs
  local teleport_spots = teleport_spot_glyphs
  local tele_marker = {}

  for i = 1, #teleporters do
      tele_marker[i] = TriggerableFunction:new {
                       func="teleporter_gauntlet_switch_fn",
                       data = {teleport_spot=i},
                       repeated=true }
      tele_marker[i]:add_triggerer(DgnTriggerer:new { type="player_move" })
      e.lua_marker(teleporters:sub(i,i), tele_marker[i])
      e.lua_marker(teleport_spots:sub(i,i), portal_desc { teleport_spot=i})
  end
end

function gauntlet_setup(e)
  e.tags('no_monster_gen')
  e.orient('encompass')
  e.tile('X = wall_permarock_red')
  e.tile('o = wall_permarock_clear_yellow')
  e.tile('c = dngn_stone_wall_red')
  e.tile('b = dngn_crystal_red')
  e.kfeat('< = exit_gauntlet')
  e.ftile('A< = floor_marble')
  e.ftile('" = floor_w_marble')
  e.colour('" = red')
end

function gauntlet_place_grate_trap_pair(e, plate_glyphs, wall_glyphs,
                                        trigger_values)
  for i = 1, 2 do
    pair_index = i == 1 and 2 or 1
    local tm = TriggerableFunction:new{
      func="callback.gauntlet_lower_wall", repeated=true,
      data={triggered=false, trigger_value = trigger_values[i],
            pair_value = trigger_values[pair_index]} }
    tm:add_triggerer(DgnTriggerer:new{type="pressure_plate"})
    e.lua_marker(plate_glyphs[i], tm)
    e.lua_marker(plate_glyphs[i], props_marker { plate = trigger_values[i] })
    e.kfeat(plate_glyphs[i] .. " = known pressure plate trap")

    e.lua_marker(wall_glyphs[i],
                 props_marker { trap_wall = trigger_values[i]})
    e.kfeat(wall_glyphs[i] .. " = clear_permarock_wall")
  end
end

function callback.gauntlet_lower_wall(data, triggerable, triggerer, marker, ev)
    if data.triggered == true then
        return
    end

    local x, y = marker:pos()
    local you_x, you_y = you.pos()
    local seen = false
    for slave in iter.slave_iterator("trap_wall", data.trigger_value) do
      dgn.terrain_changed(slave.x, slave.y, "floor", false, false, false)
      if (you.see_cell(slave.x, slave.y)) then
        seen = true
      end
    end

    if seen then
      crawl.mpr("The pressure plate clicks, and a section of wall lowers!")
    else
      crawl.mpr("The pressure plate clicks, and you hear a wall moving!")
    end

    -- Remove pressure plates.
    for slave in iter.slave_iterator("plate", data.trigger_value) do
      dgn.grid(slave.x, slave.y, "floor")
    end
    for slave in iter.slave_iterator("plate", data.pair_value) do
      dgn.grid(slave.x, slave.y, "floor")
    end
    data.triggered = true
end
