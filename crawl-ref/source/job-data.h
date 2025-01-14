enum weapon_choice
{
    WCHOICE_NONE,   ///< No weapon choice
    WCHOICE_PLAIN,  ///< Normal weapon choice
    WCHOICE_GOOD,   ///< Chooses from "good" weapons
    WCHOICE_RANGED, ///< Choice of ranged weapon
};

struct job_def
{
    const char* abbrev; ///< Two-letter abbreviation
    const char* name; ///< Long name
    int s, i, d; ///< Starting Str, Dex, and Int
    vector<species_type> recommended_species; ///< Which species are good at it
    /// Guaranteed starting equipment. Uses vault spec syntax, with the plus:,
    /// charges:, q:, and ego: tags supported.
    vector<string> equipment;
    weapon_choice wchoice; ///< how the weapon is chosen, if any
    vector<pair<skill_type, int>> skills; ///< starting skills
};

static const map<job_type, job_def> job_data =
{

{ JOB_ABYSSAL_KNIGHT, {
    "AK", "Abyssal Knight",
    5, 2, 5,
    { SP_DEMONSPAWN, SP_FORMICID, SP_GHOUL, SP_HILL_ORC, SP_HUMAN, SP_MINOTAUR, },
    { "ring mail", "tomahawk ego:dispersal q:10", "stone q:40" },
    WCHOICE_GOOD,
    { { SK_FIGHTING, 3 }, { SK_ARMOUR, 1 }, { SK_DODGING, 1 },
      { SK_INVOCATIONS, 2 }, { SK_WEAPON, 2 }, { SK_THROWING, 1 }, },
} },

{ JOB_AIR_ELEMENTALIST, {
    "AE", "Air Elementalist",
    0, 7, 5,
    { SP_AVARIEL, SP_BASE_DRACONIAN, SP_FELID, SP_FAIRY, SP_HUMAN },
    { "robe", "book of Air", "potion of flight", "potion of resistance" },
    WCHOICE_NONE,
    { { SK_AIR_MAGIC, 3 }, { SK_SPELLCASTING, 2 },
      { SK_CONJURATIONS, 1 }, { SK_CHARMS, 1 },
      { SK_DODGING, 2 }, { SK_STEALTH, 2 }, },
} },

{ JOB_SLOTH_APOSTLE, {
    "SA", "Sloth Apostle",
    4, 4, 4,
    { SP_NAGA, SP_BARACHI, SP_GNOLL, SP_MINOTAUR, SP_ENT },
    { "robe", "medium shield", "wand of paralysis charges:10" },
    WCHOICE_PLAIN,
    { { SK_FIGHTING, 2 }, { SK_SHIELDS, 2 }, { SK_WEAPON, 2 },
    { SK_STEALTH, 3 }, 
    { SK_SPELLCASTING, 1 }, { SK_HEXES, 2 }, { SK_EVOCATIONS, 1 } },
} },

{ JOB_ARCANE_MARKSMAN, {
    "AM", "Arcane Marksman",
    2, 5, 5,
    { SP_AVARIEL, SP_CENTAUR, SP_DEEP_DWARF, SP_FORMICID, SP_GNOLL,
    SP_HUMAN, SP_KOBOLD, SP_OGRE, SP_ONI, SP_SPRIGGAN, SP_SKELETON },
    { "dagger", "robe", "book of Debilitation" },
    WCHOICE_RANGED,
    { { SK_FIGHTING, 2 }, { SK_DODGING, 2 }, { SK_SPELLCASTING, 2 },
      { SK_HEXES, 3 }, { SK_WEAPON, 2 }, },
} },

{ JOB_ARCHAEOLOGIST, {
    "Ac", "Archaeologist",
    3, 6, 3,
    { SP_CENTAUR, SP_ENT, SP_FORMICID, SP_MINOTAUR, SP_NAGA, SP_VINE_STALKER, SP_GARGOYLE, SP_HILL_ORC, SP_HUMAN },
    { "whip", "blowgun", "robe plus:1", "hat plus:1", "pair of boots plus:1", "needle ego:curare q:6", "scroll of revelation", "dusty tome" },
    WCHOICE_NONE,
    { { SK_STEALTH, 3}, { SK_DODGING, 3}, { SK_FIGHTING, 1}, { SK_WEAPON, 1} },
} },

{ JOB_ARTIFICER, {
    "Ar", "Artificer",
    4, 3, 5,
    { SP_HUMAN, SP_KOBOLD, SP_SAND_DWARF, },
    { "short sword", "leather armour", "wand of flame charges:15",
      "wand of charming charges:15", "wand of random effects charges:15" },
    WCHOICE_NONE,
    { { SK_EVOCATIONS, 3 }, { SK_DODGING, 2 }, { SK_FIGHTING, 1 },
      { SK_WEAPON, 1 }, { SK_STEALTH, 1 }, },
} },

{ JOB_ASSASSIN, {
    "Rg", "Rogue",
    3, 3, 6,
    { SP_HUMAN, SP_KOBOLD, SP_SPRIGGAN, SP_VAMPIRE, SP_VINE_STALKER, SP_SAND_DWARF, },
    { "dagger plus:2", "blowgun", "robe", "cloak", "needle ego:poisoned q:80",
      "wand of flame charges:15", "wand of charming charges:15" },
    WCHOICE_NONE,
    { { SK_FIGHTING, 2 }, { SK_STEALTH, 4 }, { SK_WEAPON, 2 }, { SK_EVOCATIONS, 2 }, },
} },

{ JOB_BERSERKER, {
    "Be", "Berserker",
    9, -1, 4,
    { SP_HILL_ORC, SP_OGRE, SP_MERFOLK, SP_MINOTAUR, SP_GARGOYLE,
      SP_DEMONSPAWN, SP_ENT },
    { "animal skin" },
    WCHOICE_PLAIN,
    { { SK_FIGHTING, 3 }, { SK_DODGING, 2 }, { SK_WEAPON, 3 }, },
} },

{ JOB_CHAOS_KNIGHT, {
    "CK", "Chaos Knight",
    4, 4, 4,
    { SP_HILL_ORC, SP_TROLL, SP_CENTAUR, SP_MERFOLK, SP_MINOTAUR,
      SP_BASE_DRACONIAN, SP_DEMONSPAWN },
    { "leather armour plus:2", "scroll of teleportation", "wand of polymorph charges:8" },
    WCHOICE_PLAIN,
    { { SK_FIGHTING, 3 }, { SK_ARMOUR, 1 }, { SK_DODGING, 1 },
      { SK_WEAPON, 3 } },
} },

{ JOB_CONJURER, {
    "Cj", "Conjurer",
    0, 7, 5,
    { SP_AVARIEL, SP_NAGA, SP_BASE_DRACONIAN, SP_DEMIGOD,
      SP_FAIRY, SP_ENT },
    { "robe", "book of Conjurations", "potion of magic" },
    WCHOICE_NONE,
    { { SK_CONJURATIONS, 4 }, { SK_SPELLCASTING, 2 }, { SK_DODGING, 2 },
      { SK_STEALTH, 2 }, },
} },

{ JOB_EARTH_ELEMENTALIST, {
    "EE", "Earth Elementalist",
    0, 7, 5,
    { SP_SAND_DWARF, SP_DEEP_DWARF, SP_SPRIGGAN, SP_GARGOYLE, SP_DEMIGOD,
      SP_GHOUL, SP_OCTOPODE, SP_FAIRY, SP_ENT },
    { "book of Geomancy", "stone q:300", "robe", },
    WCHOICE_NONE,
    { { SK_WEAPON, 1 }, { SK_TRANSMUTATIONS, 1 }, { SK_EARTH_MAGIC, 3 }, { SK_SPELLCASTING, 2 },
      { SK_DODGING, 2 }, { SK_STEALTH, 2 }, }
} },

{ JOB_ENCHANTER, {
    "En", "Enchanter",
    0, 7, 5,
    { SP_AVARIEL, SP_FELID, SP_KOBOLD, SP_SPRIGGAN, SP_NAGA, SP_VAMPIRE, },
    { "dagger plus:1", "robe", "book of Maledictions", "scroll of fog", "potion of invisibility" },
    WCHOICE_NONE,
    { { SK_WEAPON, 1 }, { SK_HEXES, 3 }, { SK_SPELLCASTING, 2 },
      { SK_DODGING, 2 }, { SK_STEALTH, 3 }, },
} },

{ JOB_FIGHTER, {
    "Fi", "Fighter",
    8, 0, 4,
    { SP_DEEP_DWARF, SP_HILL_ORC, SP_TROLL, SP_MINOTAUR, SP_GARGOYLE,
      SP_CENTAUR, },
    { "scale mail", "medium shield", "potion of might" },
    WCHOICE_GOOD,
    { { SK_FIGHTING, 3 }, { SK_SHIELDS, 3 }, { SK_ARMOUR, 3 },
      { SK_WEAPON, 2 } },
} },

{ JOB_FIRE_ELEMENTALIST, {
    "FE", "Fire Elementalist",
    0, 7, 5,
    { SP_AVARIEL, SP_HILL_ORC, SP_NAGA, SP_DEMIGOD, SP_GARGOYLE,
      SP_FAIRY },
    { "robe", "book of Flames", "wand of flame charges:4", "stone q:40" },
    WCHOICE_NONE,
    { { SK_CONJURATIONS, 1 }, { SK_FIRE_MAGIC, 3 }, { SK_SPELLCASTING, 2 },
      { SK_DODGING, 2 }, { SK_STEALTH, 2 }, },
} },

{ JOB_GLADIATOR, {
    "Gl", "Gladiator",
    6, 0, 6,
    { SP_DEEP_DWARF, SP_HILL_ORC, SP_MERFOLK, SP_MINOTAUR, SP_GARGOYLE,
      SP_CENTAUR, },
    { "leather armour", "helmet", "throwing net q:3", "potion of heal wounds" },
    WCHOICE_GOOD,
    { { SK_FIGHTING, 3 }, { SK_THROWING, 2 }, { SK_DODGING, 3 },
      { SK_WEAPON, 3}, },
} },

{ JOB_HUNTER, {
    "Hu", "Hunter",
    4, 3, 5,
    { SP_KOBOLD, SP_OGRE, SP_TROLL, SP_CENTAUR, SP_DEEP_DWARF,
    SP_SAND_DWARF, SP_FORMICID, SP_HUMAN, SP_MINOTAUR },
    { "short sword", "leather armour" },
    WCHOICE_RANGED,
    { { SK_FIGHTING, 2 }, { SK_DODGING, 2 }, { SK_STEALTH, 1 },
      { SK_WEAPON, 4 }, },
} },

{ JOB_ICE_ELEMENTALIST, {
    "IE", "Ice Elementalist",
    0, 7, 5,
    { SP_MERFOLK, SP_NAGA, SP_BASE_DRACONIAN, SP_DEMIGOD,
      SP_GARGOYLE, SP_FAIRY },
    { "robe", "book of Frost", "wand of iceblast charges:1" },
    WCHOICE_NONE,
    { { SK_CONJURATIONS, 1 }, { SK_ICE_MAGIC, 3 }, { SK_SPELLCASTING, 2 },
      { SK_DODGING, 2 }, { SK_STEALTH, 2 }, },
} },

{ JOB_MARTIAL_ARTIST, {
    "MA", "Martial Artist",
    4, 1, 7,
    { SP_HILL_ORC, SP_TROLL, SP_CENTAUR, SP_MERFOLK, SP_GARGOYLE, SP_HUMAN, SP_VAMPIRE, SP_FELID, SP_FORMICID, SP_SAND_DWARF },
    { "robe", "potion of agility" },
    WCHOICE_NONE,
    { { SK_FIGHTING, 3 }, { SK_WEAPON, 4 }, { SK_DODGING, 3 }, { SK_STEALTH, 1 }, { SK_SPELLCASTING, 1 }, { SK_HEXES, 1 }, },
} },

{ JOB_MONK, {
    "Mo", "Monk",
    3, 2, 7,
    { SP_DEEP_DWARF, SP_HILL_ORC, SP_TROLL, SP_CENTAUR, SP_MERFOLK,
      SP_GARGOYLE, SP_DEMONSPAWN, SP_ENT },
    { "robe", "hat", "amulet of guardian spirit", "scroll of remove curse" },
    WCHOICE_PLAIN,
    { { SK_FIGHTING, 3 }, { SK_WEAPON, 3 }, { SK_DODGING, 3 },
      { SK_STEALTH, 2 }, { SK_INVOCATIONS, 2 }, },
} },

{ JOB_NECROMANCER, {
    "Ne", "Necromancer",
    0, 7, 5,
    { SP_AVARIEL, SP_DEEP_DWARF, SP_HILL_ORC, SP_DEMONSPAWN, SP_MUMMY,
      SP_VAMPIRE, SP_FAIRY },
    { "dagger ego:pain", "robe", "book of Necromancy" },
    WCHOICE_NONE,
    { { SK_SPELLCASTING, 2 }, { SK_NECROMANCY, 4 }, { SK_DODGING, 2 },
      { SK_STEALTH, 2 }, },
} },

{ JOB_REAVER, {
    "Re", "Reaver",
    3, 6, 3,
    { SP_GARGOYLE, SP_AVARIEL, SP_OGRE, SP_GNOLL, SP_FELID, },
    { "steam dragon scales", "potion of haste", "scroll of immolation" },
    WCHOICE_GOOD,
    { { SK_FIGHTING, 3 }, { SK_DODGING, 3 }, { SK_SPELLCASTING, 2 },
      { SK_WEAPON, 3}, },
} },

{ JOB_SKALD, {
    "Sk", "Skald",
    3, 5, 4,
    { SP_KOBOLD, SP_CENTAUR, SP_MERFOLK, SP_BASE_DRACONIAN, SP_VAMPIRE, },
    { "leather armour", "book of Battle", "amulet of magic regeneration", "scroll of fear" },
    WCHOICE_GOOD,
    { { SK_FIGHTING, 2 }, { SK_ARMOUR, 1 }, { SK_DODGING, 2 },
      { SK_SPELLCASTING, 2 }, { SK_CHARMS, 3 }, { SK_WEAPON, 2 }, },
} },

{ JOB_SPELUNKER, {
    "Sp", "Spelunker",
    4, 4, 4,
    { SP_HUMAN, SP_KOBOLD, SP_SPRIGGAN, SP_VAMPIRE, SP_VINE_STALKER, SP_SAND_DWARF, },
    { "dagger plus:1", "leather armour", "pair of boots", "wand of digging charges:10", "lamp of fire", "scroll of noise" },
    WCHOICE_NONE,
    { { SK_STEALTH, 3 }, { SK_WEAPON, 2 }, { SK_FIGHTING, 2 }, { SK_SPELLCASTING, 2 },
      { SK_HEXES, 2 }, { SK_TRANSMUTATIONS, 2 }, { SK_EARTH_MAGIC, 2 }, { SK_EVOCATIONS, 2 }, },} },

{ JOB_SUMMONER, {
    "Su", "Summoner",
    0, 7, 5,
    { SP_AVARIEL, SP_HILL_ORC, SP_VINE_STALKER, SP_MERFOLK, SP_VAMPIRE },
    { "robe", "book of Callings", "scroll of summoning" },
    WCHOICE_NONE,
    { { SK_SUMMONINGS, 4 }, { SK_SPELLCASTING, 2 }, { SK_DODGING, 2 },
      { SK_STEALTH, 2 }, },
} },

{ JOB_TRANSMUTER, {
    "Tm", "Transmuter",
    2, 5, 5,
    { SP_NAGA, SP_MERFOLK, SP_BASE_DRACONIAN, SP_DEMIGOD, SP_DEMONSPAWN,
      SP_TROLL, },
    { "arrow q:120", "robe", "book of Changes", "potion of lignification" },
    WCHOICE_NONE,
    { { SK_FIGHTING, 1 }, { SK_WEAPON, 3 }, { SK_DODGING, 2 },
      { SK_SPELLCASTING, 2 }, { SK_TRANSMUTATIONS, 2 }, },
} },

{ JOB_VENOM_MAGE, {
    "VM", "Venom Mage",
    0, 7, 5,
    { SP_AVARIEL, SP_SPRIGGAN, SP_NAGA, SP_MERFOLK, SP_FELID,
      SP_DEMONSPAWN, SP_FAIRY, SP_ENT },
    { "robe", "Young Poisoner's Handbook", "potion of berserk rage", "potion of degeneration" },
    WCHOICE_NONE,
    { { SK_CONJURATIONS, 1 }, { SK_POISON_MAGIC, 3 }, { SK_SPELLCASTING, 2 },
      { SK_DODGING, 2 }, { SK_STEALTH, 2 }, },
} },

{ JOB_WANDERER, {
    "Wn", "Wanderer",
    0, 0, 0, // Randomised
    { SP_HILL_ORC, SP_SPRIGGAN, SP_CENTAUR, SP_MERFOLK, SP_BASE_DRACONIAN,
      SP_HUMAN, SP_DEMONSPAWN, },
    { }, // Randomised
    WCHOICE_NONE,
    { }, // Randomised
} },

{ JOB_WARPER, {
    "Wr", "Warper",
    3, 5, 4,
    { SP_KOBOLD, SP_DEEP_DWARF, SP_SPRIGGAN, SP_CENTAUR,
      SP_BARACHI, SP_HUMAN },
    { "dagger", "robe", "book of Spatial Translocations", "scroll of blinking" },
    WCHOICE_RANGED,
    { { SK_FIGHTING, 2 }, { SK_DODGING, 2 }, { SK_SPELLCASTING, 2 },
      { SK_TRANSLOCATIONS, 3 }, { SK_WEAPON, 2 }, },
} },

{ JOB_WIZARD, {
    "Wz", "Wizard",
    -1, 10, 3,
    { SP_AVARIEL, SP_NAGA, SP_BASE_DRACONIAN, SP_OCTOPODE, SP_HUMAN,
      SP_MUMMY, SP_FAIRY, SP_ENT },
    { "staff", "robe", "hat", "book of Minor Magic", "potion of brilliance" },
    WCHOICE_NONE,
    { { SK_DODGING, 2 }, { SK_STEALTH, 2 }, { SK_SPELLCASTING, 3 },
      { SK_TRANSLOCATIONS, 1 }, { SK_CONJURATIONS, 1 }, { SK_SUMMONINGS, 1 }, },
} },

#if TAG_MAJOR_VERSION == 34
{ JOB_DEATH_KNIGHT, {
    "DK", "Death Knight",
    0, 0, 0,
    { },
    { },
    WCHOICE_NONE,
    { },
} },

{ JOB_HEALER, {
    "He", "Healer",
    0, 0, 0,
    { },
    { },
    WCHOICE_NONE,
    { },
} },

{ JOB_PRIEST, {
    "Pr", "Priest",
    0, 0, 0,
    { },
    { },
    WCHOICE_NONE,
    { },
} },
#endif
};
