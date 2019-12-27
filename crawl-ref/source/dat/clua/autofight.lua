---------------------------------------------------------------------------
-- autofight.lua:
-- One-key fighting.
--
-- To use this, please bind a key to the following commands:
-- ===hit_closest         (Tab by default)
-- ===hit_closest_nomove  (Shift-Tab by default)
-- ===toggle_autothrow    (not bound by default)
--
-- This uses the very incomplete client monster and view bindings, and
-- is currently very primitive. Improvements welcome!
---------------------------------------------------------------------------

local ATT_HOSTILE = 0
local ATT_NEUTRAL = 1


AUTOFIGHT_STOP = 50
AUTOFIGHT_HUNGER_STOP = 0
AUTOFIGHT_HUNGER_STOP_UNDEAD = false
AUTOFIGHT_CAUGHT = false
AUTOFIGHT_THROW = false
AUTOFIGHT_THROW_NOMOVE = true
AUTOFIGHT_FIRE_STOP = false
AUTOFIGHT_WAIT = false
AUTOFIGHT_PROMPT_RANGE = true
AUTOMAGIC_ACTIVE = false

local function delta_to_cmd(dx, dy)
  local d2v = {
    [-1] = { [-1] = "CMD_MOVE_UP_LEFT",  [0] = "CMD_MOVE_LEFT",  [1] = "CMD_MOVE_DOWN_LEFT"},
    [0]  = { [-1] = "CMD_MOVE_UP",                               [1] = "CMD_MOVE_DOWN"},
    [1]  = { [-1] = "CMD_MOVE_UP_RIGHT", [0] = "CMD_MOVE_RIGHT", [1] = "CMD_MOVE_DOWN_RIGHT"},
  }
  return d2v[dx][dy]
end

local function target_delta_to_cmd(dx, dy)
  local d2v = {
    [-1] = { [-1] = "CMD_TARGET_UP_LEFT",  [0] = "CMD_TARGET_LEFT",  [1] = "CMD_TARGET_DOWN_LEFT"},
    [0]  = { [-1] = "CMD_TARGET_UP",                                 [1] = "CMD_TARGET_DOWN"},
    [1]  = { [-1] = "CMD_TARGET_UP_RIGHT", [0] = "CMD_TARGET_RIGHT", [1] = "CMD_TARGET_DOWN_RIGHT"},
  }
  return d2v[dx][dy]
end

local function sign(a)
  return a > 0 and 1 or a < 0 and -1 or 0
end

local function abs(a)
  return a * sign(a)
end

local function adjacent(dx, dy)
  return abs(dx) <= 1 and abs(dy) <= 1
end

local function vector_move(a, dx, dy)
  for i = 1,abs(dx) do
    a[#a+1] = target_delta_to_cmd(sign(dx), 0)
  end
  for i = 1,abs(dy) do
    a[#a+1] = target_delta_to_cmd(0, sign(dy))
  end
end

local function have_reaching()
  local wp = items.equipped_at("weapon")
  return wp and wp.reach_range == 2 and not wp.is_melded
end

local function have_ranged()
  local wp = items.equipped_at("weapon")
  return wp and wp.is_ranged and not wp.is_melded
end

local function have_throwing(no_move)
  return (AUTOFIGHT_THROW or no_move and AUTOFIGHT_THROW_NOMOVE) and items.fired_item() ~= nil
end

local function is_safe_square(dx, dy)
    if view.feature_at(dx, dy) == "trap_web" then
        return false
    end
    return view.is_safe_square(dx, dy)
end

local function can_move_maybe(dx, dy)
  if view.feature_at(dx,dy) ~= "unseen" and view.is_safe_square(dx,dy) then
    local m = monster.get_monster_at(dx, dy)
    if not m or not m:is_firewood() then
      return true
    end
  end
  return false
end

local function can_move_now(dx, dy)
  local m = monster.get_monster_at(dx, dy)
  -- attitude > ATT_NEUTRAL should mean you can push past the monster
  return (is_safe_square(dx, dy) and (not m or m:attitude() > ATT_NEUTRAL))
end

local function choose_move_towards(ax, ay, bx, by, square_func)
  local los_radius = you.los()
  local move = nil
  local dx = bx - ax
  local dy = by - ay
  local function try_move(mx, my)
    if mx == 0 and my == 0 then
      return nil
    elseif abs(ax+mx) > los_radius or abs(ay+my) > los_radius then
      return nil
    elseif square_func(ax+mx, ay+my) then
      return {mx,my}
    else
      return nil
    end
  end
  if abs(dx) > abs(dy) then
    if abs(dy) == 1 then
      move = try_move(sign(dx), 0)
    end
    if move == nil then move = try_move(sign(dx), sign(dy)) end
    if move == nil then move = try_move(sign(dx), 0) end
    if move == nil and abs(dx) > abs(dy)+1 then
      move = try_move(sign(dx), 1)
    end
    if move == nil and abs(dx) > abs(dy)+1 then
      move = try_move(sign(dx), -1)
    end
    if move == nil then move = try_move(0, sign(dy)) end
  elseif abs(dx) == abs(dy) then
    move = try_move(sign(dx), sign(dy))
    if move == nil then move = try_move(sign(dx), 0) end
    if move == nil then move = try_move(0, sign(dy)) end
  else
    if abs(dx) == 1 then
      move = try_move(0, sign(dy))
    end
    if move == nil then move = try_move(sign(dx), sign(dy)) end
    if move == nil then move = try_move(0, sign(dy)) end
    if move == nil and abs(dy) > abs(dx)+1 then
      move = try_move(1, sign(dy))
    end
    if move == nil and abs(dy) > abs(dx)+1 then
      move = try_move(-1, sign(dy))
    end
    if move == nil then move = try_move(sign(dx), 0) end
  end
  return move
end

local function move_towards(dx, dy)
  local move = choose_move_towards(0, 0, dx, dy, can_move_now)
  if move == nil then
    crawl.mpr("Failed to move towards target.")
  else
    crawl.do_commands({delta_to_cmd(move[1],move[2])})
  end
end

local function will_tab(ax, ay, bx, by)
  if abs(bx-ax) <= 1 and abs(by-ay) <= 1 or
     abs(bx-ax) <= 2 and abs(by-ay) <= 2 and have_reaching() then
    return true
  end
  local move = choose_move_towards(ax, ay, bx, by, can_move_maybe)
  if move == nil then
    return false
  end
  return will_tab(ax+move[1], ay+move[2], bx, by)
end

local function get_monster_info(dx,dy,no_move)
  m = monster.get_monster_at(dx,dy)
  name = m:name()
  if not m then
    return nil
  end
  info = {}
  info.distance = (abs(dx) > abs(dy)) and -abs(dx) or -abs(dy)
  if have_ranged() then
    info.attack_type = you.see_cell_no_trans(dx, dy) and 3 or 0
  elseif not have_reaching() then
    info.attack_type = (-info.distance < 2) and 2 or 0
  else
    if -info.distance > 2 then
      info.attack_type = 0
    elseif -info.distance < 2 then
      info.attack_type = 2
    else
      info.attack_type = view.can_reach(dx, dy) and 1 or 0
    end
  end
  if info.attack_type == 0 and have_throwing(no_move) and you.see_cell_no_trans(dx, dy) then
    -- Melee is better than throwing.
    info.attack_type = 3
  end
  if info.attack_type == 0 and not will_tab(0,0,dx,dy) then
    info.attack_type = -1
  end
  info.can_attack = (info.attack_type > 0) and 1 or info.attack_type
  info.safe = m:is_safe() and -1 or 0
  info.constricting_you = m:is_constricting_you() and 1 or 0
  -- Only prioritize good stabs: sleep and paralysis.
  info.very_stabbable = (m:stabbability() >= 1) and 1 or 0
  info.injury = m:damage_level()
  info.threat = m:threat()
  info.orc_priest_wizard = (name == "orc priest" or name == "orc wizard") and 1 or 0
  return info
end

local function compare_monster_info(m1, m2)
  flag_order = autofight_flag_order
  if flag_order == nil then
    flag_order = {"can_attack", "safe", "distance", "constricting_you", "very_stabbable", "injury", "threat", "orc_priest_wizard"}
  end
  for i,flag in ipairs(flag_order) do
    if m1[flag] > m2[flag] then
      return true
    elseif m1[flag] < m2[flag] then
      return false
    end
  end
  return false
end

local function is_candidate_for_attack(x,y)
  m = monster.get_monster_at(x, y)
  --if m then crawl.mpr("Checking: (" .. x .. "," .. y .. ") " .. m:name()) end
  if not m or m:attitude() ~= ATT_HOSTILE then
    return false
  end
  if m:name() == "butterfly"
      or m:name() == "orb of destruction" then
    return false
  end
  if m:is_firewood() then
  --crawl.mpr("... is firewood.")
    if string.find(m:name(), "ballistomycete") then
      return true
    end
    return false
  end
  return true
end

local function get_target(no_move)
  local los_radius = you.los()
  local x, y, bestx, besty, best_info, new_info
  bestx = 0
  besty = 0
  best_info = nil
  for x = -los_radius,los_radius do
    for y = -los_radius,los_radius do
      if is_candidate_for_attack(x, y) then
        new_info = get_monster_info(x, y, no_move)
        if (not best_info) or compare_monster_info(new_info, best_info) then
          bestx = x
          besty = y
          best_info = new_info
        end
      end
    end
  end
  return bestx, besty, best_info
end

local function attack_fire(x,y)
  local a = {"CMD_FIRE", "CMD_TARGET_FIND_YOU"}
  vector_move(a, x, y)
  a[#a+1] = "CMD_TARGET_SELECT"
  crawl.do_commands(a, true)
end

local function attack_fire_stop(x,y)
  local a = {"CMD_FIRE", "CMD_TARGET_FIND_YOU"}
  vector_move(a, x, y)
  a[#a+1] = "CMD_TARGET_SELECT_ENDPOINT"
  crawl.do_commands(a, true)
end

local function attack_reach(x,y)
  local a = {"CMD_EVOKE_WIELDED", "CMD_TARGET_FIND_YOU"}
  vector_move(a, x, y)
  a[#a+1] = "CMD_TARGET_SELECT_ENDPOINT"
  crawl.do_commands(a, true)
end

local function attack_melee(x,y)
  crawl.do_commands({delta_to_cmd(x, y)})
end

local function set_stop_level(key, value, mode)
  AUTOFIGHT_STOP = tonumber(value)
end

local function set_hunger_stop_level(key, value, mode)
  AUTOFIGHT_HUNGER_STOP = tonumber(value)
end

local function set_hunger_stop_undead(key, value, mode)
  AUTOFIGHT_HUNGER_STOP_UNDEAD = string.lower(value) ~= "false"
end

local function set_af_caught(key, value, mode)
  AUTOFIGHT_CAUGHT = string.lower(value) ~= "false"
end

local function set_af_throw(key, value, mode)
  AUTOFIGHT_THROW = string.lower(value) ~= "false"
end

local function set_af_throw_nomove(key, value, mode)
  AUTOFIGHT_THROW_NOMOVE = string.lower(value) ~= "false"
end

local function set_af_fire_stop(key, value, mode)
  AUTOFIGHT_FIRE_STOP = string.lower(value) ~= "false"
end

local function set_af_wait(key, value, mode)
    AUTOFIGHT_WAIT = string.lower(value) ~= "false"
end

local function set_af_prompt_range(key, value, mode)
    AUTOFIGHT_PROMPT_RANGE = string.lower(value) ~= "false"
end

function set_automagic(key, value, mode)
  AUTOMAGIC_ACTIVE = string.lower(value) ~= "false"
end

function af_hp_is_low()
  local hp, mhp = you.hp()
  return (100*hp <= AUTOFIGHT_STOP*mhp)
end

function af_food_is_low()
  if you.race() == "Mummy" or you.transform() == "lich" then
      return false
  elseif (not AUTOFIGHT_HUNGER_STOP_UNDEAD)
         and (you.race() == "Vampire" or you.race() == "Ghoul") then
      return false
  else
      return (AUTOFIGHT_HUNGER_STOP ~= nil
              and you.hunger() <= AUTOFIGHT_HUNGER_STOP)
  end
end

function attack(allow_movement)
  local x, y, info = get_target(not allow_movement)
  local caught = you.caught()
  if af_hp_is_low() then
    crawl.mpr("You are too injured to fight recklessly!")
  elseif af_food_is_low() then
    crawl.mpr("You are too hungry to fight recklessly!")
  elseif you.confused() then
    crawl.mpr("You are too confused!")
  elseif caught then
    if AUTOFIGHT_CAUGHT then
      crawl.do_commands({delta_to_cmd(1, 0)}) -- Direction doesn't matter.
    else
      crawl.mpr("You are " .. caught .. "!")
    end
  elseif info == nil then
    if AUTOFIGHT_WAIT and not allow_movement then
      crawl.do_commands({"CMD_WAIT"})
    else
      crawl.mpr("No target in view!")
    end
  elseif info.attack_type == 3 then
    if AUTOFIGHT_FIRE_STOP then
      attack_fire_stop(x,y)
    else
      attack_fire(x,y)
    end
  elseif info.attack_type == 2 then
    attack_melee(x,y)
  elseif info.attack_type == 1 then
    attack_reach(x,y)
  elseif info.attack_type == -1 then
    crawl.mpr("No reachable target in view!")
  elseif allow_movement then
    if not AUTOFIGHT_PROMPT_RANGE or crawl.weapon_check() then
      move_towards(x,y)
    end
  elseif AUTOFIGHT_WAIT then
    crawl.do_commands({"CMD_WAIT"})
  else
    crawl.mpr("No target in range!")
  end
end

function hit_closest()
  if AUTOMAGIC_ACTIVE and you.spell_table()[AUTOMAGIC_SPELL_SLOT] then
    mag_attack(true)
  else
    attack(true)
  end
end

function hit_closest_nomove()
  if AUTOMAGIC_ACTIVE and you.spell_table()[AUTOMAGIC_SPELL_SLOT] then
    mag_attack(false)
  else
    attack(false)
  end
end

function hit_nonmagic()
  attack(true)
end

function hit_nonmagic_nomove()
  attack(false)
end

function hit_magic()
  if you.spell_table()[AUTOMAGIC_SPELL_SLOT] then
    mag_attack(true)
  else
    crawl.mpr("No spell in slot " .. AUTOMAGIC_SPELL_SLOT .. "!")
  end
end

function hit_magic_nomove()
  if you.spell_table()[AUTOMAGIC_SPELL_SLOT] then
    mag_attack(false)
  else
    crawl.mpr("No spell in slot " .. AUTOMAGIC_SPELL_SLOT .. "!")
  end
end

function toggle_autothrow()
  AUTOFIGHT_THROW = not AUTOFIGHT_THROW
  crawl.mpr(AUTOFIGHT_THROW and "Enabling autothrow." or "Disabling autothrow.")
end

chk_lua_option.autofight_stop = set_stop_level
chk_lua_option.autofight_hunger_stop = set_hunger_stop_level
chk_lua_option.autofight_hunger_stop_undead = set_hunger_stop_undead
chk_lua_option.autofight_caught = set_af_caught
chk_lua_option.autofight_throw = set_af_throw
chk_lua_option.autofight_throw_nomove = set_af_throw_nomove
chk_lua_option.autofight_fire_stop = set_af_fire_stop
chk_lua_option.autofight_wait = set_af_wait
chk_lua_option.autofight_prompt_range = set_af_prompt_range
chk_lua_option.automagic_enable = set_automagic

auto = false

mk = 1
_pillar = nil
_path = nil
_seqForward = nil
_seqBackward = nil
_seqidx = 0
_dir = true
_pillarDance = false

function inputTogglePillar()
    local px, py = crawl.get_target()
    local wx, wy = travel.waypoint_delta(7)

    if _pillar ~= nil and wx ~= nil then
        for i, xy in ipairs(_pillar) do
            if xy[1] == wx + px and xy[2] == wy + py then
                killPillar()
                crawl.mpr("Path cleared.")
                return
            end
        end
    end

    killPillar()
    doSearch(px, py)
    travel.set_waypoint(7, 0, 0)
end

function inputPillar()
    local x, y = crawl.get_target()
    killPillar()
    doSearch(x, y)
    travel.set_waypoint(7, 0, 0)
end

function dancePillar()
    if (not auto) and _pillarDance then
        crawl.mpr("Stopping manual walk.")
        stopPillarDance()
        if _nextAction ~= nil then
            -- kill the exclude we set after our last step
            local x, y = getOffset(_nextAction)
            travel.del_exclude(x, y, 0)
        end
        return
    end
    if _path == nil then
        crawl.mpr("No path to travel!")
        return
    end
    -- find which tile (if any) of the path we are standing on
    local x, y = travel.waypoint_delta(7)
    local path = _path
    local idx = 0
    for i, xy in ipairs(path) do
        if xy[1] == x and xy[2] == y then
            idx = i
            break
        end
    end
    if idx == 0 then
        crawl.mpr("Step onto a path to travel it.")
        return
    end
    startPillarDance()
    _seqForward = getSeq(path)
    _seqBackward = getSeqBackwards(path)
    _seqidx = idx
    if (not auto) then
        crawl.mpr("Starting manual walk.")
    end
end

do
    local toOff
    function getOffset(cmd)
        toOff = toOff or { CMD_MOVE_UP_LEFT = { -1, -1 },
            CMD_MOVE_UP_RIGHT = { 1, -1 },
            CMD_MOVE_DOWN_LEFT = { -1, 1 },
            CMD_MOVE_DOWN_RIGHT = { 1, 1 },
            CMD_MOVE_UP = { 0, -1 },
            CMD_MOVE_DOWN = { 0, 1 },
            CMD_MOVE_LEFT = { -1, 0 },
            CMD_MOVE_RIGHT = { 1, 0 } }
        return unpack(toOff[cmd])
    end
end


function checkDance()
    -- checks whether we should dance, based on hp, mp, monster positioning, ... and also changes our direction
    -- if necessary (i.e. a monster is blocking our path).
    assert(_seqForward ~= nil)
    assert(_seqBackward ~= nil)
    assert(_seqidx ~= 0)
    local monsters = getAllMonsters()
    -- fast monsters and ranged monsters + casters are bad for health
    -- TODO allow user to override, could be useful for high-regen users vs. low level fast mons
    for i, mon in ipairs(monsters) do
        if string.find(mon:speed_description(), "fast") then
            crawl.mpr("Fast monster in view!")
            stopPillarDance()
            return false
        end
        if #(mon:spells()) ~= 0 or mon:has_known_ranged_attack() then
            crawl.mpr("Ranged enemy in view!")
            stopPillarDance()
            return false
        end
        if mon:status("fast") or mon:status("covering ground quickly") then
            crawl.mpr("Fast monster in view!")
            stopPillarDance()
            return false
        end
    end

    local xdir, ydir, xndir, yndir
    local x, y = getOffset(getNextAction())
    xdir, ydir = x, y
    -- the below pile of ifs checks whether we should keep going in current direction, switch direction, or stop
    -- altogether. It favors moving away from monsters to moving not away, and will never move towards monsters or
    -- within one tile of a monster.
    if not tileIsBetter(x, y, monsters) then
        _dir = not _dir
        x, y = getOffset(getNextAction())
        xndir, yndir = x, y
        if not tileIsBetter(x, y, monsters) then
            _dir = not _dir
            x, y = xdir, ydir
            if not tileIsGood(x, y, monsters) then
                _dir = not _dir
                x, y = xndir, yndir
                if not tileIsGood(x, y, monsters) then
                    crawl.mpr("No good direction to walk!")
                    stopPillarDance()
                    return false
                end
            end
        end
    end
    return true
end

function getNextAction()
    if _dir then
        return _seqForward[_seqidx]
    else
        return _seqBackward[_seqidx]
    end
end

function doSeqAction()
    local nextAction = getNextAction()
    -- actually performs the action and pushes the sequence pointer in the correct direction.
    if _dir then
        _seqidx = _seqidx + 1
        if _seqidx > #_seqForward then
            _seqidx = 1
        end
    else
        _seqidx = _seqidx - 1
        if _seqidx < 1 then
            _seqidx = #_seqBackward
        end
    end
    local x, y = getOffset(nextAction)
    if not travel.feature_traversable(view.feature_at(x, y)) then
        crawl.mpr("Unexpected blockage of path! Did you close a door or move mid-dance?")
        stopPillarDance()
        return
    end
    crawl.do_commands({ nextAction })
end

_stepcount = 0

function showPillar()
    if _path == nil then return end
    local x, y = travel.waypoint_delta(7)
    if x == nil then return end
    local pathTiles = {}
    for i, t in ipairs(_path) do
        pathTiles[t] = true
    end
    showTiles(pathTiles, -x, -y)
end

function hidePillar()
    if _path == nil then return end
    local x, y = travel.waypoint_delta(7)
    if x == nil then return end
    local pathTiles = {}
    for i, t in ipairs(_path) do
        pathTiles[t] = true
    end
    hideTiles(pathTiles, -x, -y)
end

function startPillarDance()
    hidePillar()
    _pillarDance = true
    _nextAction = nil
    _stepcount = 0
end

function stopPillarDance()
    showPillar()
    _pillarDance = false
end

function killPillar()
    hidePillar()
    _pillarDance = false
    _pillar = nil
    _path = nil
    _seqForward = nil
    _seqBackward = nil
    _seqidx = nil
    _dir = true
end

_nextAction = nil
function doStep()
    if (not auto) and _pillarDance then
        if not checkDance() then
            -- dance stopped, delete "next step" exclusion and stop dancing
            if _nextAction ~= nil then
                local x, y = getOffset(_nextAction)
                travel.del_exclude(x, y)
            end
            stopPillarDance()
            return
        end
        if _nextAction ~= nil then
            -- if an enemy came into view, our next action may have changed, do nothing and just switch next action
            local confirmNextAction = getNextAction()
            if _nextAction == confirmNextAction then
                doSeqAction()
                -- after the action happens, the "next action" exclude should be under us
                travel.del_exclude(0, 0)
            else
                crawl.mpr("Direction swapped!")
                local x, y = getOffset(_nextAction)
                travel.del_exclude(x, y)
            end
        end
        _nextAction = getNextAction()
        if _nextAction ~= nil then
            local x, y = getOffset(_nextAction)
            travel.set_exclude(x, y, 0)
        end
    end
end

function c_answer_prompt()
    if (not auto) and _pillarDance then
        return true
    end
end

function can_walk_towards(m)
    -- it's ok to walk towards: neutral monsters, harmless monsters, and firewood.
    -- this is separate from can_walk_through because if they are in our direct path, we can't walk towards them.
    if not m or m:attitude() > 1 then
        return true
    end
    if m:name() == "butterfly" then
        return true
    end
    if m:is_firewood() then
        if string.find(m:name(), "ballistomycete") then
            return false
        end
        return true
    end
    return false
end

function can_walk_through(m)
    -- we can only walk through, and thus completely ignore, friendly non-stationary non-trapped monsters.
    return not m or (m:attitude() == 4 and not (m:is_stationary() or m:is_constricted() or m:is_caught()))
end

function getAllMonsters()
    local monsters = {}
    local los = you.los()
    for x_off = -los, los do
        for y_off = -los, los do
            if view.cell_see_cell(0, 0, x_off, y_off) then
                local mon = monster.get_monster_at(x_off, y_off)
                if not can_walk_through(mon) then
                    monsters[#monsters + 1] = mon
                end
            end
        end
    end
    return monsters
end

function tileIsGood(x, y, monsters)
    -- a tile is "good" if it does not take us closer to any monsters.
    for i, mon in ipairs(monsters) do
        local newDist = getDist(x, y, mon:x_pos(), mon:y_pos())
        if newDist == 0 then return false end
        if (not can_walk_towards(mon)) and (newDist <= 1
                or (getDist(0, 0, mon:x_pos(), mon:y_pos()) > newDist
                and view.cell_see_cell(x, y, mon:x_pos(), mon:y_pos()))) then
            return false
        end
    end
    return true
end

function tileIsBetter(x, y, monsters)
    -- a tile is "better" if it takes us farther away from all monsters.
    for i, mon in ipairs(monsters) do
        local newDist = getDist(x, y, mon:x_pos(), mon:y_pos())
        if newDist == 0 then return false end
        if (not can_walk_towards(mon)) and (newDist <= 1
                or (getDist(0, 0, mon:x_pos(), mon:y_pos()) >= newDist
                and view.cell_see_cell(x, y, mon:x_pos(), mon:y_pos()))) then
            return false
        end
    end
    return true
end

function doSearch(x, y)
    -- first, get the outline (orthogonally adjacent tiles) of the pillar.
    local pillar = create(x, y)
    if pillar == nil then
        return nil
    end
    local xmin, ymin, xmax, ymax = getBbox(pillar.floors)
    -- find two distinct tiles on the outline which are on the outline's axis aligned bounding box.
    local x1, y1, x2, y2 = findTwoTilesOnBorder(pillar.floors, xmin, ymin, xmax, ymax)
    local tileset = getTilesInBox(xmin, ymin, xmax, ymax)
    -- find the shortest path between those two tiles.
    local path = getPath(tileset, x1, y1, x2, y2)
    local next = path[2]
    -- block that path.
    if x1 == xmin then
        mk.put(tileset, x1, next[2], false)
        mk.put(tileset, x1 + 1, next[2], false)
    elseif x1 == xmax then
        mk.put(tileset, x1, next[2], false)
        mk.put(tileset, x1 - 1, next[2], false)
    elseif y1 == ymin then
        mk.put(tileset, next[1], y1, false)
        mk.put(tileset, next[1], y1 + 1, false)
    elseif y1 == ymax then
        mk.put(tileset, next[1], y1, false)
        mk.put(tileset, next[1], y1 - 1, false)
    end
    -- find the shortest path again. This will be forced to be the other path because of the blocked tiles.
    local path2 = getPath(tileset, x1, y1, x2, y2)
    -- flip the second path, concatenate it to the original path. Now we have a full cyclic path around the pillar.
    reverse(path2)
    local prevlen = #path
    for i = 2, (#path2 - 1) do
        path[prevlen + i - 1] = path2[i]
    end
    local pathTiles = {}
    for i, t in ipairs(path) do
        pathTiles[t] = true
    end
    showTiles(pathTiles)
    _path = path
    _pillar = pillar.walls
    crawl.mpr("Pillar chosen. Step on one of the excluded tiles and press space to continue.")
    return path
end

function getSeq(path)
    local seq = {}
    for i, s in ipairs(path) do
        local t = path[(i + 1)]
        if i == #path then
            t = path[1]
        end
        -- this could be a lookup table, but i already wrote it
        local n
        if t[1] == s[1] and t[2] == s[2] + 1 then
            n = "CMD_MOVE_DOWN"
        elseif t[1] == s[1] and t[2] == s[2] - 1 then
            n = "CMD_MOVE_UP"
        elseif t[1] == s[1] + 1 and t[2] == s[2] then
            n = "CMD_MOVE_RIGHT"
        elseif t[1] == s[1] - 1 and t[2] == s[2] then
            n = "CMD_MOVE_LEFT"
        elseif t[1] == s[1] + 1 and t[2] == s[2] + 1 then
            n = "CMD_MOVE_DOWN_RIGHT"
        elseif t[1] == s[1] + 1 and t[2] == s[2] - 1 then
            n = "CMD_MOVE_UP_RIGHT"
        elseif t[1] == s[1] - 1 and t[2] == s[2] + 1 then
            n = "CMD_MOVE_DOWN_LEFT"
        elseif t[1] == s[1] - 1 and t[2] == s[2] - 1 then
            n = "CMD_MOVE_UP_LEFT"
        end
        seq[i] = n
    end
    return seq
end

function getSeqBackwards(path)
    local seq = {}
    for i, s in ipairs(path) do
        local t = path[(i - 1)]
        if i == 1 then
            t = path[#path]
        end
        local n
        -- lol
        if t[1] == s[1] and t[2] == s[2] + 1 then
            n = "CMD_MOVE_DOWN"
        elseif t[1] == s[1] and t[2] == s[2] - 1 then
            n = "CMD_MOVE_UP"
        elseif t[1] == s[1] + 1 and t[2] == s[2] then
            n = "CMD_MOVE_RIGHT"
        elseif t[1] == s[1] - 1 and t[2] == s[2] then
            n = "CMD_MOVE_LEFT"
        elseif t[1] == s[1] + 1 and t[2] == s[2] + 1 then
            n = "CMD_MOVE_DOWN_RIGHT"
        elseif t[1] == s[1] + 1 and t[2] == s[2] - 1 then
            n = "CMD_MOVE_UP_RIGHT"
        elseif t[1] == s[1] - 1 and t[2] == s[2] + 1 then
            n = "CMD_MOVE_DOWN_LEFT"
        elseif t[1] == s[1] - 1 and t[2] == s[2] - 1 then
            n = "CMD_MOVE_UP_LEFT"
        end
        seq[i] = n
    end
    return seq
end


function getTilesInBox(xmin, ymin, xmax, ymax)
    local tiles = mk.new()
    for x = xmin, xmax do
        for y = ymin, ymax do
            mk.put(tiles, x, y, travel.feature_traversable(view.feature_at(x, y)))
        end
    end
    return tiles
end

function create(x, y)
    -- flood fill the wall tiles of the pillar, then get the orthogonally neighboring walkable tiles
    local walls = floodFillWalls(x, y)
    if walls == nil then
        return nil
    end
    return {walls=walls, floors=getNeighboringFloors(walls)}
end

function showTiles(tiles, offx, offy)
    offx = offx or 0
    offy = offy or 0
    for xy, _ in pairs(tiles) do
        local x = xy[1] + offx
        local y = xy[2] + offy
        travel.set_exclude(x, y, 0)
    end
end

function printTiles(tiles)
    -- helper debugging function
    for i, xy in ipairs(tiles) do
        crawl.mpr("(" .. xy[1] .. ", " .. xy[2] .. ")")
    end
end

function hideTiles(tiles, offx, offy)
    offx = offx or 0
    offy = offy or 0
    for xy, _ in pairs(tiles) do
        local x = xy[1] + offx
        local y = xy[2] + offy
        travel.del_exclude(x, y)
    end
end

function extendSet(a, b)
    for k, _ in pairs(b) do
        a[k] = true
    end
end

function has(l, t)
    for k, _ in pairs(l) do
        print(k)
        if k[1] == t[1] and k[2] == t[2] then
            return true
        end
    end
    return false
end

function newTupleSet()
    return {}
end

function floodFillWalls(x, y)
    if travel.feature_traversable(view.feature_at(x, y)) then
        return nil
    end
    local counter = 1
    local used = mk.new()
    used:put(x, y, true)
    local queue = {}
    queue[1] = {x, y}
    local queue_next = 1
    local n_pos = {}
    n_pos[{ 0, 1 }] = true
    n_pos[{ 0, -1 }] = true
    n_pos[{ 1, 0 }] = true
    n_pos[{ -1, 0 }] = true
    while queue_next <= #queue do
        local x, y = unpack(queue[queue_next])
        queue_next = queue_next + 1
        for oxoy, _ in pairs(n_pos) do
            local off_x, off_y = unpack(oxoy)
            if mk.get(used, x + off_x, y + off_y) == nil and not travel.feature_traversable(view.feature_at(x + off_x, y + off_y)) then
                counter = counter + 1
                if counter > 1000 then
                    crawl.mpr("Pillar too large! Did you select a map border?")
                    return nil
                end
                used:put(x + off_x, y + off_y, true)
                queue[#queue + 1] = {x + off_x, y + off_y }
            end
        end
    end
    return queue
end

function getNeighboringFloors(tiles)
    local neighbors = {}
    local n_pos = newTupleSet()
    n_pos[{ 0, 1 }] = true
    n_pos[{ 0, -1 }] = true
    n_pos[{ 1, 0 }] = true
    n_pos[{ -1, 0 }] = true
    for _, xy in ipairs(tiles) do
        local x, y = unpack(xy)
        for oxoy, _ in pairs(n_pos) do
            local off_x, off_y = unpack(oxoy)
            if travel.feature_traversable(view.feature_at(x + off_x, y + off_y)) then
                neighbors[{ x + off_x, y + off_y }] = true
            end
        end
    end
    return neighbors
end

function getBbox(tiles)
    local xmin = math.huge
    local xmax = -math.huge
    local ymin = math.huge
    local ymax = -math.huge
    for tile, _ in pairs(tiles) do
        local x = tile[1]
        local y = tile[2]
        if x > xmax then
            xmax = x
        end
        if x < xmin then
            xmin = x
        end
        if y > ymax then
            ymax = y
        end
        if y < ymin then
            ymin = y
        end
    end
    return xmin, ymin, xmax, ymax
end

function tilesOneApart(x1, y1, x2, y2)
    return math.abs(x1 - x2) <= 1 and math.abs(y1 - y2) <= 1
end

function findTwoTilesOnBorder(tiles, xmin, ymin, xmax, ymax)
    local t1found = false
    local t1 = nil
    for tile, _ in pairs(tiles) do
        local x = tile[1]
        local y = tile[2]
        if (x == xmin or x == xmax or y == ymin or y == ymax) then
            if t1found then
                if not tilesOneApart(t1[1], t1[2], x, y) then
                    return t1[1], t1[2], x, y
                end
            else
                t1found = true
                t1 = { x, y }
            end
        end
    end
    return nil
end

function getDist(x1, y1, x2, y2)
    return math.max(math.abs(x1 - x2), math.abs(y1 - y2))
end

function getNeighboring(tileset, x, y)
    local neighbors = {}
    for ox = -1, 1 do
        for oy = -1, 1 do
            if mk.get(tileset, x + ox, y + oy) ~= nil and not (ox == 0 and oy == 0) then
                neighbors[{ x + ox, y + oy }] = true
            end
        end
    end
    return neighbors
end

function getMinTile(nodes, dist)
    local min = math.huge
    local tile
    for _, x, y, __ in mk.tuples(nodes) do
        local cost = mk.get(dist, x, y)
        if cost ~= nil and cost <= min then
            min = cost
            tile = { x, y }
        end
    end
    return tile
end

function getPath(tileset, x1, y1, x2, y2)
    -- we just use dijkstra's to get the shortest path
    local pq = newPriorityQueue()
    local dist = mk.new()
    local nodes = mk.new()
    local prev = mk.new()
    for _, x, y, trav in mk.tuples(tileset) do
        if trav then
            dist:put(x, y, math.huge)
            nodes:put(x, y, true)
        end
    end
    pq:insert({val={x1, y1}, cost=0})
    dist:put(x1, y1, 0)
    while not pq:empty() do
        local u = pq:popMin().val
        assert(u ~= nil)
        mk.put(nodes, u[1], u[2], nil)
        if u[1] == x2 and u[2] == y2 then
            assert(prev:get(u[1], u[2]) ~= nil)
            return parsePrev(prev, x1, y1, x2, y2)
        end
        for v, _ in pairs(getNeighboring(nodes, u[1], u[2])) do
            local altDist = dist:get(u[1], u[2]) + 1
            if dist:get(v[1], v[2]) == nil or altDist < dist:get(v[1], v[2]) then
                dist:put(v[1], v[2], altDist)
                prev:put(v[1], v[2], u)
                pq:insert({val=v, cost=altDist})
            end
        end
    end
end

function parsePrev(prev, x1, y1, x2, y2)
    -- goes backwards along the prev chain to find the tiles in the shortest path
    local path = {}
    local u = { x2, y2 }
    path[1] = u
    while not (u[1] == x1 and u[2] == y1) do
        u = mk.get(prev, u[1], u[2])
        assert(u ~= nil)
        path[#path + 1] = u
    end
    reverse(path)
    return path
end

function reverse(arr)
    local i, j = 1, #arr

    while i < j do
        arr[i], arr[j] = arr[j], arr[i]

        i = i + 1
        j = j - 1
    end
end

local function getMk()
    -- i don't know enough lua to mess around with metatables and do this myself, so i yoinked this from somewhere

    -- simple table adaptor for using multiple keys in a lookup table

    -- cache some global functions/tables for faster access
    local assert = assert
    local select = assert(select)
    local next = assert(next)
    local setmetatable = assert(setmetatable)

    -- sentinel values for the key tree, nil keys, and nan keys
    local KEYS, NIL, NAN = {}, {}, {}


    local M = {}
    local M_meta = { __index = M }


    function M.new()
        return setmetatable({ [KEYS] = {} }, M_meta)
    end

    setmetatable(M, { __call = M.new })


    function M.clear(t)
        for k in next, t do
            t[k] = nil
        end
        return t
    end


    -- local helper function to map a vararg of keys to the real key
    local function get_key(key, ...)
        for i = 1, select('#', ...) do
            if key == nil then break
            end
            local e = select(i, ...)
            if e == nil then
                e = NIL
            elseif e ~= e then -- can only happen for NaNs
                e = NAN
            end
            key = key[e]
        end
        return key
    end


    function M.get(t, ...)
        local key = get_key(t[KEYS], ...)
        if key ~= nil then
            return t[key]
        end
        return nil
    end


    -- local helper function for both put variants below
    local function put(t, idx, val, n, ...)
        for i = 1, n do
            local e = select(i, ...)
            if e == nil then
                e = NIL
            elseif e ~= e then -- can only happen for NaNs
                e = NAN
            end
            local nextidx = idx[e]
            if not nextidx then
                nextidx = {}
                idx[e] = nextidx
            end
            idx = nextidx
        end
        t[idx] = val
    end


    -- returns true if tab can be removed from the parent table
    local function del(t, idx, n, ...)
        if n > 0 then
            local e = ...
            if e == nil then
                e = NIL
            elseif e ~= e then -- can only happen for NaNs
                e = NAN
            end
            local nextidx = idx[e]
            if nextidx and del(t, nextidx, n - 1, select(2, ...)) then
                idx[e] = nil
                return t[idx] == nil and next(idx) == nil
            end
            return false
        else
            t[idx] = nil
            return next(idx) == nil
        end
    end


    function M.put(t, ...)
        local n, keys, val = select('#', ...), t[KEYS], nil
        if n > 0 then
            val = select(n, ...)
            n = n - 1
        end
        if val == nil then
            if keys ~= nil then
                del(t, keys, n, ...)
            end
        else
            if keys == nil then
                keys = {}
                t[KEYS] = keys
            end
            put(t, keys, val, n, ...)
        end
        return t
    end


    -- same as M.put, but value comes first not last
    function M.putv(t, val, ...)
        local keys = t[KEYS]
        if val == nil then
            if keys ~= nil then
                del(t, keys, select('#', ...), ...)
            end
        else
            if keys == nil then
                keys = {}
                t[KEYS] = keys
            end
            put(t, keys, val, select('#', ...), ...)
        end
        return t
    end


    -- iteration is only available with coroutine support
    if coroutine ~= nil then
        local unpack = assert(unpack or table.unpack)
        local pairs = assert(pairs)
        local ipairs = assert(ipairs)
        local co_yield = assert(coroutine.yield)
        local co_wrap = assert(coroutine.wrap)


        -- internal iterator function
        local function iterate(iter, t, key, keystack, n)
            if t[key] ~= nil then
                keystack[n + 1] = t[key]
                co_yield(unpack(keystack, 1, n + 1))
            end
            for k, v in iter(key) do
                if k == NIL then
                    k = nil
                elseif k == NAN then
                    k = 0 / 0
                end
                keystack[n + 1] = k
                iterate(iter, t, v, keystack, n + 1)
            end
            return nil
        end


        -- iterator similar to pairs, but since we have multiple keys ...
        function M.tuples(t, ...)
            local vals, n = { true, ... }, select('#', ...) + 1
            return co_wrap(function()
                local key = get_key(t[KEYS], unpack(vals, 2, n))
                if key ~= nil then
                    return iterate(pairs, t, key, vals, n)
                end
            end)
        end


        function M.ituples(t, ...)
            local vals, n = { ... }, select('#', ...)
            return co_wrap(function()
                local key = get_key(t[KEYS], unpack(vals, 1, n))
                if key ~= nil then
                    return iterate(ipairs, t, key, vals, n)
                end
            end)
        end


        -- Lua 5.2 metamethods for iteration
        M_meta.__pairs = M.tuples
        M_meta.__ipairs = M.ituples
    end

    return M
end

function newPriorityQueue()
    local lt = function(a, b)
        return a.cost < b.cost
    end
    return MinHeap.new(lt)
end

MinHeap = {}
MH_meta = { __index = MinHeap }

function getminfn(ltfn)
    return function(a, b)
        if ltfn(a, b) then
            return a
        else
            return b
        end
    end
end

function MinHeap.new(ltfn)
    if ltfn == nil then
        ltfn = function(a, b) return a < b end
    end
    local minfn = getminfn(ltfn)
    local t = setmetatable({}, MH_meta)
    t.lt = ltfn
    t.min = minfn
    return t
end

function MinHeap.getMin(heap)
    return heap[1]
end

function MinHeap.popMin(heap)
    local min = heap[1]
    heap[1] = heap[#heap]
    heap[#heap] = nil
    downheap(heap, 1)
    return min
end

function MinHeap.insert(heap, val)
    heap[#heap + 1] = val
    upheap(heap, #heap)
end

function MinHeap.empty(heap)
    return #heap == 0
end

function downheap(heap, idx)
    local l = 2 * idx
    local r = 2 * idx + 1
    if heap[l] ~= nil then
        if heap[r] ~= nil then
            local minchild = heap.min(heap[l], heap[r])
            if not heap.lt(minchild, heap[idx]) then
                return
            end
            local swap
            if minchild == heap[l] then
                swap = l
            else
                swap = r
            end
            heap[swap] = heap[idx]
            heap[idx] = minchild
            downheap(heap, swap)
        else
            if not heap.lt(heap[l], heap[idx]) then
                return
            end
            local temp = heap[l]
            heap[l] = heap[idx]
            heap[idx] = temp
        end
    end
end

function upheap(heap, idx)
    if idx == 1 then
        return
    end
    local parent = math.floor(idx / 2)
    if not heap.lt(heap[idx], heap[parent]) then
        return
    end
    local temp = heap[parent]
    heap[parent] = heap[idx]
    heap[idx] = heap[temp]
    upheap(heap, parent)
end

function startOrStep()
    if (not _pillarDance) then
        dancePillar()
    else
        if _path == nil then
            crawl.mpr("No path to travel!")
            return
        end
        -- find which tile (if any) of the path we are standing on
        local x, y = travel.waypoint_delta(7)
        local path = _path
        local idx = 0
        for i, xy in ipairs(path) do
            if xy[1] == x and xy[2] == y then
                idx = i
                break
            end
        end
        if idx == 0 then

            local wx, wy = travel.waypoint_delta(7)

            if _pillar ~= nil and wx ~= nil then
                for i, xy in ipairs(_pillar) do
                    if xy[1] == wx and xy[2] == wy then
                        killPillar()
                        crawl.mpr("Path cleared.")
                        return
                    end
                end
            end

            killPillar()
            travel.set_waypoint(7, 0, 0)
            crawl.mpr("Path cleared.")

            return
        end
    end
    doStep()
end

mk = getMk()
