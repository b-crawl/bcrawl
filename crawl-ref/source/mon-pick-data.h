//FLAT,  full chance throughout the range
//SEMI, // 50% chance at range ends, 100% in the middle
//PEAK, // 0% chance just outside range ends, 100% in the middle, range
      // ends typically get ~20% or more
//RISE, // linearly from near-zero to 100%, increasing with depth
//FALL, // linearly from 100% at the start to near-zero
static const pop_entry pop_d[] =
{ // Dungeon (OOD cap: 27)
  { -2,  5,  515, SEMI, MONS_GIANT_COCKROACH },
  { -2,  6,  192, PEAK, MONS_BALL_PYTHON },
  { -1,  5,  640, SEMI, MONS_FRILLED_LIZARD},
  { -1,  7,  136, SEMI, MONS_OOZE },
  {  1,  3, 1000, FLAT, MONS_GOBLIN },
  {  1,  3, 1000, FLAT, MONS_BAT },
  {  1,  3, 1000, FLAT, MONS_KOBOLD },
  {  1,  3, 1000, FLAT, MONS_RAT },
  {  1,  3, 1000, FLAT, MONS_HOBGOBLIN },
  {  1,  5,  335, FALL, MONS_DART_SLUG },
  {  1,  8,  515, SEMI, MONS_LEOPARD_GECKO },
  {  1, 10,  335, SEMI, MONS_JACKAL },
  {  2, 11,  192, PEAK, MONS_GNOLL },
  {  2,  8,  515, SEMI, MONS_QUOKKA },
  {  2,  6,  515, FALL, MONS_WORM },
  {  3, 11,  925, FLAT, MONS_ADDER },
  {  3, 11, 1000, FLAT, MONS_ORC },
  {  4,  7, 1000, FLAT, MONS_NO_MONSTER},
  {  4, 10, 4000, FLAT, MONS_NO_MONSTER},
  {  4, 10,   52, SEMI, MONS_GNOLL_SHAMAN },
  {  4, 11,  136, SEMI, MONS_ORC_PRIEST },
  {  4, 12,  925, FLAT, MONS_WORKER_ANT },
  {  4, 12,  825, SEMI, MONS_HOUND },
  {  4, 12,  825, SEMI, MONS_OGRE },
  {  4, 12,  515, SEMI, MONS_IGUANA },
  {  4, 12,  515, SEMI, MONS_ORC_WIZARD },
  {  4, 12,  515, SEMI, MONS_SCORPION },
  {  4, 12,  335, SEMI, MONS_PHANTOM },
  {  4, 12,  335, SEMI, MONS_HOWLER_MONKEY },
  {  4, 12, 1000, FLAT, MONS_SKELETON },
  {  4, 12,  650, FLAT, MONS_CRIMSON_IMP },
  {  4, 12,  515, SEMI, MONS_SKY_BEAST },
  {  5, 12,   89, PEAK, MONS_GNOLL_SERGEANT },
  {  5, 12,  350, FLAT, MONS_SHADOW_IMP },
  {  5, 12,  825, SEMI, MONS_CENTAUR },
  {  5, 12,  515, SEMI, MONS_ICE_BEAST },
  {  5, 12,  515, SEMI, MONS_JELLY },
  {  5, 12,  192, PEAK, MONS_NECROPHAGE },
  {  5, 12,  192, PEAK, MONS_QUASIT },
  {  5, 12,  192, PEAK, MONS_WATER_MOCCASIN },
  {  5, 13, 1000, FLAT, MONS_ZOMBIE },
  {  6, 11,  285, SEMI, MONS_BLACK_BEAR },
  {  6, 11, 1000, FLAT, MONS_BULLFROG },
  {  6, 13,  825, SEMI, MONS_ORC_WARRIOR },
  {  6, 13,  335, SEMI, MONS_WIGHT },
  {  6, 13,  335, SEMI, MONS_NO_MONSTER }, // Old ballistomycete spore weight.
  {  6, 12,  415, SEMI, MONS_BIG_KOBOLD },
  {  6, 12,   89, PEAK, MONS_STEAM_DRAGON },
  {  6, 14,  615, SEMI, MONS_HIPPOGRIFF },
  {  6, 14,  350, SEMI, MONS_HUNGRY_GHOST },
  {  6, 14,  192, PEAK, MONS_CROCODILE },
  {  7, 11,  470, SEMI, MONS_KILLER_BEE },
  {  7, 14,  192, PEAK, MONS_HORNET },
  {  7, 14,  825, SEMI, MONS_TROLL },
  {  8, 14,  675, SEMI, MONS_TWO_HEADED_OGRE },
  {  8, 12,  335, SEMI, MONS_WYVERN },
  {  8, 14,  230, PEAK, MONS_EYE_OF_DRAINING },
  {  8, 12,  825, SEMI, MONS_YAK },
  {  9, 15,  192, PEAK, MONS_VAMPIRE },
  {  9, 13,  140, PEAK, MONS_SOLDIER_ANT },
  { 10, 15,  370, SEMI, MONS_GARGOYLE },
  { 10, 15,  335, SEMI, MONS_WRAITH },
  { 10, 16,  192, PEAK, MONS_KOMODO_DRAGON },
  { 10, 16,  235, SEMI, MONS_KOBOLD_DEMONOLOGIST },
  { 10, 16,  750, FLAT, MONS_UGLY_THING },
  { 10, 17,  520, SEMI, MONS_MANTICORE },
  { 10, 17,  825, SEMI, MONS_CYCLOPS },
  { 10, 17,  380, SEMI, MONS_SHADOW },
  { 10, 17,  335, SEMI, MONS_ACID_DRAGON },
  { 10, 17,  260, PEAK, MONS_BASILISK },
  { 10, 18,  440, FALL, MONS_SLIME_CREATURE },
  { 10, 17,  192, PEAK, MONS_SKELETAL_WARRIOR },
  { 10, 18,  192, PEAK, MONS_DEATH_KNIGHT },
  { 10, 19,  515, SEMI, MONS_HYDRA },
  { 11, 16,  515, SEMI, MONS_UNSEEN_HORROR },
  { 11, 16,  475, SEMI, MONS_DEEP_ELF_MAGE },
  { 11, 17,  500, SEMI, MONS_VAMPIRE_MOSQUITO },
  { 11, 16,  150, SEMI, MONS_MELIAI },
  { 11, 18,   89, PEAK, MONS_RAKSHASA },
  { 11, 14,  925, FLAT, MONS_CENTAUR_WARRIOR },
  { 15, 17,  925, FALL, MONS_CENTAUR_WARRIOR },
  { 11, 16,  365, SEMI, MONS_TENGU_WARRIOR },
  { 11, 16,  205, SEMI, MONS_TENGU_CONJURER },
  { 11, 16,  285, SEMI, MONS_EFREET },
  { 11, 19,  675, SEMI, MONS_SHAPESHIFTER },
  { 11, 23,   89, FALL, MONS_CATOBLEPAS },
  { 12, 18,  260, SEMI, MONS_FREEZING_WRAITH },
  { 12, 18,  270, SEMI, MONS_SIMULACRUM },
  { 12, 18,  115, SEMI, MONS_BOGGART },
  { 12, 16,  335, SEMI, MONS_YAKTAUR },
  { 12, 20,  345, FALL, MONS_ORC_KNIGHT },
  { 12, 21,  315, FALL, MONS_OGRE_MAGE },
  { 12, 21,  275, PEAK, MONS_WOLF_SPIDER },
  { 13, 20,   89, PEAK, MONS_REDBACK },
  { 14, 20,   25, PEAK, MONS_SHADOW_WRAITH },
  { 14, 23,  192, PEAK, MONS_FIRE_DRAGON },
  { 14, 23,   92, PEAK, MONS_ICE_DRAGON },
  { 14, 19,   89, PEAK, MONS_VERY_UGLY_THING },
  { 14, 20,  102, SEMI, MONS_GREAT_ORB_OF_EYES },
  { 14, 20,   52, SEMI, MONS_NECROMANCER },
  { 14, 21,  136, SEMI, MONS_DEEP_ELF_KNIGHT },
  { 14, 21,  136, SEMI, MONS_DEEP_ELF_ARCHER },
  { 14, 21,  136, SEMI, MONS_HORNET },
  { 14, 21,   89, PEAK, MONS_FIRE_CRAB },
  { 14, 21,   89, PEAK, MONS_HARPY },
  { 14, 21,  136, SEMI, MONS_DEEP_ELF_MAGE },
  { 15, 20,   52, SEMI, MONS_GLOWING_ORANGE_BRAIN },
  { 15, 24,  180, PEAK, MONS_FLAYED_GHOST },
  { 15, 21,   35, PEAK, MONS_ORC_HIGH_PRIEST },
  { 15, 22,  136, SEMI, MONS_DEEP_TROLL },
  { 15, 23,   30, SEMI, MONS_ORC_SORCERER },
  { 15, 16,  140, FLAT, MONS_WIZARD },
  { 15, 23,   28, SEMI, MONS_ORC_WARLORD },
  { 16, 23,  100, RISE, MONS_SPHINX },
  { 16, 26,  310, FALL, MONS_YAKTAUR_CAPTAIN },
  { 16, 26,  136, SEMI, MONS_FIRE_GIANT },
  { 16, 26,  136, SEMI, MONS_FROST_GIANT },
  { 17, 23,  310, FALL, MONS_STONE_GIANT },
  { 17, 27,  192, PEAK, MONS_ETTIN },
  { 18, 26,  136, SEMI, MONS_DEEP_TROLL_EARTH_MAGE },
  { 19, 23,   89, RISE, MONS_STORM_DRAGON },
  { 19, 25,   25, PEAK, MONS_HELL_KNIGHT },
  { 19, 27,   89, PEAK, MONS_PHANTASMAL_WARRIOR },
  { 19, 23,  136, FLAT, MONS_GLOWING_SHAPESHIFTER },
  { 20, 23,   22, RISE, MONS_DEEP_ELF_ANNIHILATOR },
  { 20, 23,   22, RISE, MONS_DEEP_ELF_HIGH_PRIEST },
  { 20, 23,   32, RISE, MONS_VAMPIRE_KNIGHT },
  { 20, 23,  136, RISE, MONS_TENGU_REAVER },
  { 20, 23,    8, RISE, MONS_DANCING_WEAPON },
  { 20, 23,   25, FLAT, MONS_WAR_GARGOYLE },
  { 21, 23,   12, RISE, MONS_CRYSTAL_GUARDIAN },
  { 22, 23,   18, FLAT, MONS_SHADOW_DRAGON },
  { 22, 23,    8, FLAT, MONS_IRON_DRAGON },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_temple[] =
{ // Temple
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_orc[] =
{ // Orcish Mines
  {  1,  4,  384, FLAT, MONS_NO_MONSTER },
  {  1,  4,  192, FLAT, MONS_ORC_WARRIOR },
  {  1,  4,   25, FLAT, MONS_ORC_PRIEST },
  {  1,  4,   25, FLAT, MONS_ORC_WIZARD },
  {  1,  4,   25, FLAT, MONS_ORC_KNIGHT },
  {  1,  4,    8, FLAT, MONS_ORC_HIGH_PRIEST },
  {  1,  4,   25, FLAT, MONS_ORC_SORCERER },
  {  1,  4,    8, FLAT, MONS_ORC_WARLORD },
  {  1,  4,   40, FLAT, MONS_WARG },
  {  1,  4,    3, FLAT, MONS_GNOLL },
  {  1,  4,    1, FLAT, MONS_GNOLL_SHAMAN },
  {  1,  4,    1, FLAT, MONS_GNOLL_SERGEANT },
  {  1,  4,   89, FLAT, MONS_OGRE },
  {  1,  4,    8, FLAT, MONS_TWO_HEADED_OGRE },
  {  1,  4,    1, FLAT, MONS_OGRE_MAGE },
  {  1,  4,   40, FLAT, MONS_TROLL },
  {  1,  4,   12, FLAT, MONS_CYCLOPS },
  {  1,  4,    4, FLAT, MONS_ETTIN },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_elf[] =
{ // Elven Halls (OOD cap: 7)
  {  1,  5,   50, FLAT, MONS_ORC_HIGH_PRIEST },
  {  1,  6,   50, FLAT, MONS_ORC_SORCERER },
  {  1,  7, 2775, FLAT, MONS_DEEP_ELF_MAGE },
  {  1,  7, 1385, FLAT, MONS_DANCING_WEAPON },
  {  1,  7,  925, FLAT, MONS_DEEP_ELF_KNIGHT },
  {  1,  7,  925, FLAT, MONS_DEEP_ELF_ARCHER },
  {  1,  7,  345, FLAT, MONS_FIRE_ELEMENTAL },
  {  1,  7,  345, FLAT, MONS_WATER_ELEMENTAL },
  {  1,  7,  345, FLAT, MONS_AIR_ELEMENTAL },
  {  1,  7,  345, FLAT, MONS_EARTH_ELEMENTAL },
  {  1,  8,   65, SEMI, MONS_DEEP_ELF_SORCERER },
  {  1,  7,   40, SEMI, MONS_DEEP_ELF_ANNIHILATOR },
  {  1,  8,   65, SEMI, MONS_DEEP_ELF_DEMONOLOGIST },
  {  1,  7,   40, SEMI, MONS_DEEP_ELF_HIGH_PRIEST },
  {  1,  8,   65, SEMI, MONS_DEEP_ELF_ELEMENTALIST },
  {  0,  7,   45, SEMI, MONS_DEEP_ELF_DEATH_MAGE },
  {  6,  7,   10, RISE, MONS_DEEP_ELF_BLADEMASTER },
  {  6,  7,   10, RISE, MONS_DEEP_ELF_MASTER_ARCHER },
  {  1,  6,  150, FLAT, MONS_SHAPESHIFTER },
  {  1,  6,   65, SEMI, MONS_GLOWING_SHAPESHIFTER },
  { 0,0,0,FLAT,MONS_0 }
};
#if TAG_MAJOR_VERSION == 34
static const pop_entry pop_dwarf[] =
{ // Dwarven Hall
  {  1,  1, 1000, FLAT, MONS_DEEP_DWARF },
  {  1,  1,  690, FLAT, MONS_DEATH_KNIGHT },
  {  1,  1,    3, FLAT, MONS_DEEP_TROLL },
  {  1,  1,    3, FLAT, MONS_DEEP_TROLL_EARTH_MAGE },
  {  1,  1,    3, FLAT, MONS_DEEP_TROLL_SHAMAN },
  {  1,  1,    8, FLAT, MONS_STONE_GIANT },
  {  1,  1,    8, FLAT, MONS_FIRE_GIANT },
  {  1,  1,    8, FLAT, MONS_FROST_GIANT },
  {  1,  1,  192, FLAT, MONS_WRAITH },
  {  1,  1,    3, FLAT, MONS_SHADOW_WRAITH },
  {  1,  1,    8, FLAT, MONS_EIDOLON },
  {  1,  1,    8, FLAT, MONS_PHANTASMAL_WARRIOR },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_blade[] =
{ // Hall of Blades
  {  1,  1, 1000, FLAT, MONS_DANCING_WEAPON },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_lab[] =
{ // Labyrinth
  { 0,0,0,FLAT,MONS_0 }
};
#endif

static const pop_entry pop_lair[] =
{ // Lair (OOD cap: 12)
  {  1,  5, 5500, FLAT, MONS_NO_MONSTER }, // Roughly old D:1-4 chaff weight
  { -1,  5,   52, SEMI, MONS_BLACK_BEAR },
  { -1,  7,  260, SEMI, MONS_BASILISK },
  { -1, 11,  440, SEMI, MONS_HYDRA },
  {  0,  4,   24, PEAK, MONS_SCORPION },
  {  0,  6,  260, SEMI, MONS_WOLF },
  {  0,  9,  240, PEAK, MONS_RIME_DRAKE },
  {  0, 11,  380, SEMI, MONS_BLACK_MAMBA },
  {  1,  5, 1000, FLAT, MONS_PORCUPINE },
  {  1,  5, 1000, FLAT, MONS_BULLFROG },
  {  1,  6,  990, FLAT, MONS_CROCODILE },
  {  1,  6,  990, FLAT, MONS_WATER_MOCCASIN },
  {  1,  6,  515, FLAT, MONS_HIPPOGRIFF },
  {  1,  6,  515, FLAT, MONS_YAK },
  {  1,  7,  188, PEAK, MONS_WYVERN },
  {  1,  8,  380, SEMI, MONS_BLINK_FROG },
  {  1,  6,  900, FLAT, MONS_CANE_TOAD },
  {  7, 10,  160, FLAT, MONS_CANE_TOAD },
  {  1,  6,  515, FLAT, MONS_KOMODO_DRAGON },
  {  7, 10,  160, FLAT, MONS_KOMODO_DRAGON },
  {  2,  5,    8, PEAK, MONS_HORNET },
  {  2,  5,   25, PEAK, MONS_STEAM_DRAGON },
  {  2,  7,   25, PEAK, MONS_OKLOB_PLANT },
  {  2,  8,   25, PEAK, MONS_HELL_RAT },
  {  2,  8,  100, SEMI, MONS_POLAR_BEAR },
  {  2,  9,  240, PEAK, MONS_ELEPHANT },
  {  2, 10,  160, RISE, MONS_DEATH_YAK },
  {  2, 11,   44, SEMI, MONS_CATOBLEPAS },
  {  2, 16,   60, SEMI, MONS_TORPOR_SNAIL },
  {  3,  8,   52, SEMI, MONS_POLAR_BEAR },
  {  3, 10,   25, PEAK, MONS_LINDWURM },
  {  4, 12,  140, SEMI, MONS_DREAM_SHEEP },
  {  4, 16,   36, RISE, MONS_FIRE_CRAB },
  { 11, 12,   32, FLAT, MONS_DIRE_ELEPHANT },
  { 11, 12,   32, FLAT, MONS_ALLIGATOR },
  { 11, 12,   32, FLAT, MONS_MANTICORE },
  { 11, 12,   32, FLAT, MONS_ANACONDA },
  { 11, 12,   32, FLAT, MONS_WOLF_SPIDER },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_swamp[] =
{ // Swamp
  {  0,  3, 2000, FLAT, MONS_NO_MONSTER }, // was plant + fungus
  { -7,  3,  480, RISE, MONS_SLIME_CREATURE },
  { -3,  2,  245, SEMI, MONS_SHADOW },
  { -1,  4,  480, SEMI, MONS_SPRIGGAN_RIDER },
  { -1,  5,   40, PEAK, MONS_UGLY_THING },
  {  0,  3, 1000, FLAT, MONS_ALLIGATOR },
  {  0,  3, 1000, FLAT, MONS_CANE_TOAD },
  {  0,  3,  925, FLAT, MONS_BOG_BODY },
  {  0,  3,  925, FLAT, MONS_SWAMP_DRAKE },
  {  0,  3,  915, FLAT, MONS_HYDRA },
  {  0,  3,  825, FLAT, MONS_TYRANT_LEECH },
  {  0,  3,  665, FLAT, MONS_VAMPIRE_MOSQUITO },
  {  0,  3,  500, FLAT, MONS_SWAMP_DRAGON },
  {  0,  3,  335, FLAT, MONS_INSUBSTANTIAL_WISP },
  {  0,  3,  192, FLAT, MONS_HORNET },
  {  0,  3,  100, FALL, MONS_KOMODO_DRAGON },
  {  0,  3,   25, RISE, MONS_TENTACLED_MONSTROSITY },
  {  0,  3,  435, RISE, MONS_THORN_HUNTER },
  {  0,  3,  385, RISE, MONS_GHOST_CRAB },
  {  0,  3,  180, RISE, MONS_DEATH_DRAKE },
  {  0,  4,  425, SEMI, MONS_SHAMBLING_MANGROVE },
  {  0,  6,  115, PEAK, MONS_BLINK_FROG },
  {  0,  6,   52, SEMI, MONS_VERY_UGLY_THING },
  {  1,  3,  525, RISE, MONS_SPRIGGAN_DRUID },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_shoals[] =
{ // Shoals
  { -2,  3,   89, SEMI, MONS_CENTAUR },
  { -1,  2,  355, SEMI, MONS_FAUN },
  { -1,  3,  220, FALL, MONS_HIPPOGRIFF },
  { -1,  5,  300, SEMI, MONS_WATER_NYMPH },
  { -1,  5,  170, SEMI, MONS_MERFOLK_AVATAR },
  { -1,  5,  110, SEMI, MONS_CYCLOPS },
  { -1,  5,   73, SEMI, MONS_KRAKEN },
  { -1,  6,  265, SEMI, MONS_MERFOLK_IMPALER },
  { -1,  6,  125, SEMI, MONS_MERFOLK_AQUAMANCER },
  {  0,  3,  735, FLAT, MONS_MERFOLK },
  {  0,  3,  475, FLAT, MONS_SEA_SNAKE },
  {  0,  3,  385, FLAT, MONS_SNAPPING_TURTLE },
  {  0,  3,  460, FLAT, MONS_MANTICORE },
  {  0,  3,  300, FLAT, MONS_MERFOLK_SIREN },
  {  0,  3,  195, FLAT, MONS_WATER_ELEMENTAL },
  {  0,  3,  225, FLAT, MONS_WIND_DRAKE },
  {  0,  3,  175, FLAT, MONS_HARPY },
  {  0,  6,  135, PEAK, MONS_MERFOLK_JAVELINEER },
  {  0,  6,   85, PEAK, MONS_ALLIGATOR_SNAPPING_TURTLE },
  {  1,  3,  190, SEMI, MONS_SATYR },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_snake[] =
{ // Snake Pit
  { -5,  3,  750, SEMI, MONS_NAGA },
  { -1,  3,  215, SEMI, MONS_SALAMANDER },
  { -1,  5,  315, SEMI, MONS_ANACONDA },
  { -1,  7, 1125, SEMI, MONS_BLACK_MAMBA },
  { -1,  7,  340, SEMI, MONS_NAGA_WARRIOR },
  { -1,  7,  550, SEMI, MONS_NAGA_MAGE },
  { -1,  7,  225, SEMI, MONS_NAGA_RITUALIST },
  { -1,  7,  315, SEMI, MONS_NAGA_SHARPSHOOTER },
  { -1,  7,  415, PEAK, MONS_SHOCK_SERPENT },
  {  0,  3,  200, FLAT, MONS_MANA_VIPER },
  {  0,  6,  225, PEAK, MONS_GUARDIAN_SERPENT },
  {  1,  4,  110, PEAK, MONS_SALAMANDER_MYSTIC },
  {  1,  7,  245, SEMI, MONS_NAGARAJA },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_spider[] =
{ // Spider Nest
  { -2,  4,  260, RISE, MONS_HORNET },
  { -2,  9,  765, SEMI, MONS_DEMONIC_CRAWLER },
  {  0,  3,  955, FLAT, MONS_WOLF_SPIDER },
  {  0,  3,  985, FLAT, MONS_JUMPING_SPIDER },
  {  0,  3,  910, FLAT, MONS_TARANTELLA },
  {  0,  3, 1045, FLAT, MONS_REDBACK },
  {  0,  3,  550, FLAT, MONS_ORB_SPIDER },
  {  0,  5,  100, FLAT, MONS_TORPOR_SNAIL },
  {  0,  5,  190, SEMI, MONS_EMPEROR_SCORPION },
  {  0,  5,  155, SEMI, MONS_MOTH_OF_WRATH },
  {  0,  8,  300, SEMI, MONS_ENTROPY_WEAVER },
  {  0,  8,  200, PEAK, MONS_GHOST_MOTH },
  {  1,  3,   12, FLAT, MONS_MELIAI },
  {  1,  9,  260, PEAK, MONS_SPARK_WASP },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_slime[] =
{ // Slime Pits
  {  1,  5, 2000, FLAT, MONS_SLIME_CREATURE },
  {  1,  5, 1000, FLAT, MONS_ACID_BLOB },
  {  1,  5,  515, FLAT, MONS_EYE_OF_DRAINING },
  {  1,  5,  515, FLAT, MONS_AZURE_JELLY },
  {  1,  5,  515, FLAT, MONS_SHINING_EYE },
  {  1,  5,  200, FLAT, MONS_GOLDEN_EYE },
  {  1,  8,  300, SEMI, MONS_EYE_OF_DEVASTATION },
  {  1,  8,  390, SEMI, MONS_GREAT_ORB_OF_EYES },
  {  1,  5,  200, RISE, MONS_DEATH_OOZE },
  {  2,  5,  100, RISE, MONS_GLOWING_ORANGE_BRAIN },
  {  2,  8,   50, SEMI, MONS_FLOATING_EYE },
  {  1,  5, 2750, FLAT, MONS_NO_MONSTER }, // Old ooze and jelly weight
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_vaults[] =
{ // Vaults (OOD cap: 12)
  { -6,  4,  675, SEMI, MONS_SHAPESHIFTER },
  { -5,  3,  270, SEMI, MONS_SIMULACRUM },
  { -5,  3,  178, SEMI, MONS_BOGGART },
  { -5,  5,  192, PEAK, MONS_FIRE_DRAGON },
  { -4,  6,  420, SEMI, MONS_OGRE_MAGE },
  { -4,  6,  335, SEMI, MONS_YAKTAUR },
  { -2,  6,  136, SEMI, MONS_GREAT_ORB_OF_EYES },
  { -2,  6,  136, SEMI, MONS_HORNET },
  { -2,  6,  136, SEMI, MONS_DEEP_ELF_KNIGHT },
  { -2,  6,  136, SEMI, MONS_DEEP_ELF_ARCHER },
  { -2,  6,  178, PEAK, MONS_ICE_DRAGON },
  { -2,  6,   89, PEAK, MONS_HARPY },
  { -1,  5,   89, PEAK, MONS_GLOWING_ORANGE_BRAIN },
  { -1,  9,  260, PEAK, MONS_FREEZING_WRAITH },
  { -1,  9,   89, PEAK, MONS_SHADOW_WRAITH },
  { -1,  9,   60, PEAK, MONS_FLAYED_GHOST },
  {  0,  8,   89, PEAK, MONS_ENTROPY_WEAVER },
  {  0, 12,   89, PEAK, MONS_SPHINX },
  {  0, 12,   89, PEAK, MONS_VERY_UGLY_THING },
  {  1,  1,  255, FLAT, MONS_UNSEEN_HORROR },
  {  1,  1,  185, FLAT, MONS_WRAITH },
  {  1,  2,  415, FLAT, MONS_CYCLOPS },
  {  1,  2,  165, FLAT, MONS_ACID_DRAGON },
  {  1,  2,   89, FLAT, MONS_SKELETAL_WARRIOR },
  {  1,  3,  136, FLAT, MONS_RAKSHASA },
  {  1,  3,  440, FALL, MONS_SLIME_CREATURE },
  {  1,  3,  222, FLAT, MONS_WIZARD },
  {  1,  4,  440, FALL, MONS_UGLY_THING },
  {  1,  4,  440, FALL, MONS_CENTAUR_WARRIOR },
  {  1,  5,   89, FLAT, MONS_NECROMANCER },
  {  1,  5,   89, FLAT, MONS_DEATH_KNIGHT },
  {  1,  6,  275, FALL, MONS_WOLF_SPIDER },
  {  1, 10,  640, FALL, MONS_VAULT_SENTINEL },
  {  1, 10,  640, FALL, MONS_IRONBRAND_CONVOKER },
  {  1, 11,  575, FALL, MONS_IRONHEART_PRESERVER },
  {  1, 12,  510, FALL, MONS_VAULT_WARDEN },
  {  1, 12,  415, FALL, MONS_YAKTAUR_CAPTAIN },
  {  2,  6,   32, PEAK, MONS_CRYSTAL_GUARDIAN },
  {  2, 12,  340, FALL, MONS_STONE_GIANT },
  {  2, 12,  192, PEAK, MONS_ETTIN },
  {  3, 10,   80, FLAT, MONS_DEEP_ELF_MAGE },
  {  3, 11,  136, SEMI, MONS_FIRE_GIANT },
  {  3, 11,  136, SEMI, MONS_FROST_GIANT },
  {  3, 11,   68, SEMI, MONS_DEEP_TROLL },
  {  3, 11,   66, PEAK, MONS_LICH },
  {  4,  8,   30, PEAK, MONS_ORC_WARLORD },
  {  4, 10,   25, PEAK, MONS_HELL_KNIGHT },
  {  4, 12,  155, PEAK, MONS_TENGU_REAVER },
  {  4, 12,   89, PEAK, MONS_STORM_DRAGON },
  {  4, 12,   89, PEAK, MONS_PHANTASMAL_WARRIOR },
  {  5, 11,   70, FLAT, MONS_WAR_GARGOYLE },
  {  5, 12,   25, PEAK, MONS_DANCING_WEAPON },
  {  5, 13,  193, FALL, MONS_GLOWING_SHAPESHIFTER },
  {  5, 13,   66, PEAK, MONS_TENTACLED_MONSTROSITY },
  {  8, 12,   50, FLAT, MONS_SHADOW_DRAGON },
  {  9, 12,   50, FLAT, MONS_TITAN },
  { 10, 12,   25, FLAT, MONS_DEEP_ELF_HIGH_PRIEST },
  { 10, 12,   16, RISE, MONS_QUICKSILVER_DRAGON },
  { 10, 12,   16, RISE, MONS_IRON_DRAGON },
  { 10, 14,   25, SEMI, MONS_GOLDEN_DRAGON },
  { 11, 12,   16, FLAT, MONS_DEEP_ELF_DEMONOLOGIST },
  { 11, 12,   16, FLAT, MONS_DEEP_ELF_ANNIHILATOR },
  { 11, 12,   16, FLAT, MONS_DEEP_ELF_SORCERER },
  { 11, 12,   16, FLAT, MONS_DEEP_ELF_DEATH_MAGE },
  { 11, 12,   16, FLAT, MONS_DEEP_ELF_ELEMENTALIST },
  { 12, 12,   16, FLAT, MONS_ANCIENT_LICH },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_crypt[] =
{ // Crypt (OOD cap: 5)
  { -3,  3,   70, PEAK, MONS_SHADOW },
  { -3,  3,   40, PEAK, MONS_WIGHT },
  { -3,  3,   45, PEAK, MONS_FLYING_SKULL },
  { -3,  3,   75, SEMI, MONS_NECROMANCER },
  { -3,  3,   75, SEMI, MONS_DEATH_KNIGHT },
  { -2,  4,  125, SEMI, MONS_WRAITH },
  { -1,  5,   75, PEAK, MONS_JIANGSHI },
  { -1,  5,  125, PEAK, MONS_PHANTASMAL_WARRIOR },
  { -1,  9,  120, PEAK, MONS_ANCIENT_CHAMPION },
  {  0,  2,   65, SEMI, MONS_FREEZING_WRAITH },
  {  0,  5,  125, FLAT, MONS_VAMPIRE_MAGE },
  {  0,  5,   93, PEAK, MONS_GHOUL },
  {  0,  5,  105, FLAT, MONS_VAMPIRE_KNIGHT },
  {  1,  4,   75, FLAT, MONS_SKELETAL_WARRIOR },
  {  1,  5,   55, SEMI, MONS_SOUL_EATER },
  {  1,  6,  145, SEMI, MONS_EIDOLON },
  {  1,  6,   80, SEMI, MONS_DEEP_ELF_DEATH_MAGE },
  {  1,  7,   85, SEMI, MONS_REVENANT },
  {  1,  7,   75, SEMI, MONS_CURSE_SKULL },
  {  2,  5,  145, SEMI, MONS_FLAYED_GHOST },
  {  2,  5,   95, SEMI, MONS_SHADOW_WRAITH },
  {  2,  7,   40, SEMI, MONS_REAPER },
  {  2,  7,   55, PEAK, MONS_LICH },
  {  3,  7,   15, PEAK, MONS_ANCIENT_LICH },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_tomb[] =
{ // Tomb (OOD cap: 5)
  {  1,  5,  200, FLAT, MONS_SKELETON },
  {  1,  5,  185, FLAT, MONS_ZOMBIE },
  {  1,  5,   50, SEMI, MONS_SIMULACRUM },
  {  1,  5, 1000, FLAT, MONS_MUMMY },
  {  1,  5,  230, FLAT, MONS_FLYING_SKULL },
  {  1,  5,  600, FLAT, MONS_GUARDIAN_MUMMY },
  { -1,  9,  335, SEMI, MONS_MUMMY_PRIEST },
  {  4,  5,    3, RISE, MONS_GREATER_MUMMY },
  {  3,  5,    6, FLAT, MONS_LICH },
  {  3,  5,    3, SEMI, MONS_ANCIENT_LICH },
  {  1,  5,  250, FLAT, MONS_USHABTI },
  {  1,  5,  150, FLAT, MONS_DEATH_SCARAB },
  {  3,  5,   12, SEMI, MONS_BENNU },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_hell[] =
{ // Hell
  {  1,  1,   89, FLAT, MONS_HELL_RAT },
  {  1,  1,   89, FLAT, MONS_DEMONIC_CRAWLER },
  {  1,  1,   89, FLAT, MONS_HELL_HOUND },
  {  1,  1,  136, FLAT, MONS_HELL_HOG },
  {  1,  1,  136, FLAT, MONS_HELL_KNIGHT },
  {  1,  1,  136, FLAT, MONS_NECROMANCER },
  {  1,  1,   25, FLAT, MONS_HELLEPHANT },
  {  1,  1,   25, FLAT, MONS_TENTACLED_MONSTROSITY },
  {  1,  1,   89, FLAT, MONS_RED_DEVIL },
  {  1,  1,   89, FLAT, MONS_ICE_DEVIL },
  {  1,  1,  515, FLAT, MONS_SUN_DEMON },
  {  1,  1,  515, FLAT, MONS_SOUL_EATER },
  {  1,  1,  515, FLAT, MONS_REAPER },
  {  1,  1,   89, FLAT, MONS_LOROCYPROCA },
  {  1,  1,   25, FLAT, MONS_HELLION },
  {  1,  1,   25, FLAT, MONS_TORMENTOR },
  {  1,  1,   89, FLAT, MONS_RUST_DEVIL },
  {  1,  1,   52, FLAT, MONS_GREEN_DEATH },
  {  1,  1,   52, FLAT, MONS_BLIZZARD_DEMON },
  {  1,  1,   52, FLAT, MONS_BALRUG },
  {  1,  1,   52, FLAT, MONS_CACODEMON },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_dis[] =
{ // Dis
  {  1,  7, 2000, FLAT, MONS_ZOMBIE },
  {  1, 12,  270, FALL, MONS_SKELETON },
  {  1,  7,  136, FALL, MONS_NECROPHAGE },
  {  1,  7,  420, FALL, MONS_MUMMY },
  {  1,  7,   66, FALL, MONS_PHANTOM },
  {  1,  6,  420, FALL, MONS_WIGHT },
  {  1,  7,  475, SEMI, MONS_WRAITH },
  {  1,  5,  625, FALL, MONS_SHADOW },
  {  1,  7,  260, FALL, MONS_FLYING_SKULL },
  {  1,  7,  192, SEMI, MONS_FREEZING_WRAITH },
  {  1, 12,  515, FALL, MONS_SKELETAL_WARRIOR },
  {  1,  7,  160, PEAK, MONS_PHANTASMAL_WARRIOR },
  {  1,  7,  136, SEMI, MONS_FLAYED_GHOST },
  {  1,  9,   89, PEAK, MONS_BONE_DRAGON },
  {  1,  7,   25, PEAK, MONS_LICH },
//{  4,  6,    4, SEMI, MONS_ANCIENT_LICH },
  {  1,  7,   25, FLAT, MONS_EFREET },
  {  1, 12,  440, FALL, MONS_HELL_HOUND },
  {  1,  7,   89, FALL, MONS_HELL_HOG },
  {  1,  7,   25, FALL, MONS_HELL_KNIGHT },
  {  1,  7, 1000, FLAT, MONS_IRON_IMP },
  {  1,  2,    8, FALL, MONS_CRIMSON_IMP },
  {  1,  7, 1000, FLAT, MONS_RUST_DEVIL },
  {  1,  7,  540, FALL, MONS_RED_DEVIL },
  {  1,  7,  335, FLAT, MONS_ICE_DEVIL },
  {  1,  7,   25, PEAK, MONS_SOUL_EATER },
  {  1,  7,  900, FLAT, MONS_REAPER },
  {  1,  7,  775, FLAT, MONS_TORMENTOR },
  {  1,  7,  300, FLAT, MONS_HELLION },
  {  4,  7,    4, FLAT, MONS_BRIMSTONE_FIEND },
  {  2,  8,   25, PEAK, MONS_IRON_DRAGON },
  {  1, 12,   60, FALL, MONS_IRON_GOLEM },
  {  1,  7,   25, PEAK, MONS_DANCING_WEAPON },
  {  1,  7,  270, RISE, MONS_IRON_GIANT },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_geh[] =
{ // Gehenna
  {  1,  7, 1950, FLAT, MONS_SKELETON },
  {  1,  7, 1950, FLAT, MONS_ZOMBIE },
  {  1,  7,  215, FALL, MONS_PHANTOM },
  {  1,  7,  515, FALL, MONS_NECROPHAGE },
  {  1,  7,  690, FALL, MONS_WIGHT },
  {  0,  7,  570, FLAT, MONS_WRAITH },
  {  1,  7,  705, FALL, MONS_SHADOW },
  {  1,  7,  385, FLAT, MONS_FLYING_SKULL },
  {  1,  9,   89, PEAK, MONS_PHANTASMAL_WARRIOR },
  {  1,  7,  215, FLAT, MONS_FLAYED_GHOST },
  {  2,  8,   25, PEAK, MONS_BONE_DRAGON },
  {  1,  9,  136, SEMI, MONS_LICH },
  {  4,  7,    2, FLAT, MONS_ANCIENT_LICH },
  {  2,  6,  215, FLAT, MONS_EFREET },
  { -1,  7,  515, PEAK, MONS_HELL_KNIGHT },
  {  1,  7,  350, FLAT, MONS_HELL_HOUND },
  {  1,  7,  300, FLAT, MONS_HELL_HOG },
  {  1,  7,  192, FALL, MONS_CRIMSON_IMP },
  {  1, 12,  690, FALL, MONS_RED_DEVIL },
  { -1,  7,   89, PEAK, MONS_RUST_DEVIL },
  {  3,  7,    8, PEAK, MONS_SOUL_EATER },
  {  1,  7,  590, FLAT, MONS_HELLION },
  {  1,  7,  365, FLAT, MONS_TORMENTOR },
  {  1,  7,  192, FLAT, MONS_BALRUG },
  {  1,  9,  385, SEMI, MONS_REAPER },
  {  3,  7,   14, SEMI, MONS_HELL_SENTINEL },
  {  3,  7,    8, PEAK, MONS_BRIMSTONE_FIEND },
  {  1,  7,   46, FALL, MONS_FIRE_BAT },
  {  2,  6,    8, PEAK, MONS_FIRE_CRAB },
  {  1,  7,   30, FLAT, MONS_IRON_GOLEM },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_coc[] =
{ // Cocytus
  {  1,  7, 1915, FLAT, MONS_SKELETON },
  {  1,  7, 1915, FLAT, MONS_ZOMBIE },
  {  1,  7,   70, FALL, MONS_SIMULACRUM },
  {  1,  5,  136, FALL, MONS_PHANTOM },
  { -1,  7,  260, SEMI, MONS_MUMMY },
  { -1,  7,  146, SEMI, MONS_HUNGRY_GHOST },
  {  1,  7,  136, FALL, MONS_NECROPHAGE },
  {  1,  7,  420, FALL, MONS_WIGHT },
  { -1,  7,  420, SEMI, MONS_WRAITH },
  {  1,  6,  625, FALL, MONS_SHADOW },
  {  1,  7,  970, FLAT, MONS_FREEZING_WRAITH },
  {  1,  7,  640, SEMI, MONS_FLYING_SKULL },
  {  1,  7,  515, FLAT, MONS_SKELETAL_WARRIOR },
  {  1,  7,   89, PEAK, MONS_PHANTASMAL_WARRIOR },
  {  2,  7,   35, FLAT, MONS_BONE_DRAGON },
  {  2,  8,   35, PEAK, MONS_LICH },
  {  3,  7,    8, PEAK, MONS_ANCIENT_LICH },
  {  1,  7,  890, FLAT, MONS_WHITE_IMP },
  {  1,  7,  890, FLAT, MONS_ICE_DEVIL },
  {  1,  7,   81, PEAK, MONS_SOUL_EATER },
  {  1,  7,  285, FLAT, MONS_TORMENTOR },
  {  1,  7,  192, FLAT, MONS_BLIZZARD_DEMON },
  {  1,  7,  455, RISE, MONS_REAPER },
  {  1,  7,  955, FALL, MONS_ICE_BEAST },
  {  1,  7,  305, FLAT, MONS_ICE_DRAGON },
  {  1,  7,  270, RISE, MONS_SHARD_SHRIKE },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_tar[] =
{ // Tartarus
  {  1,  7, 2000, FLAT, MONS_SKELETON },
  {  1,  7, 2000, FLAT, MONS_ZOMBIE },
  {  1,  7,   70, PEAK, MONS_SIMULACRUM },
  {  1,  7,  850, SEMI, MONS_NECROPHAGE },
  {  1,  7,  720, FLAT, MONS_NECROPHAGE },
  {  1,  7,  215, FLAT, MONS_HUNGRY_GHOST },
  {  1,  7,  990, FLAT, MONS_SHADOW },
  {  1,  7,  550, SEMI, MONS_WRAITH },
  {  1,  7,  550, FALL, MONS_PHANTOM },
  {  1,  7,  835, FALL, MONS_WIGHT },
  {  1,  7,  200, FLAT, MONS_VAMPIRE_MAGE },
  {  1,  7,  200, FLAT, MONS_VAMPIRE_KNIGHT },
  {  1,  7,  570, FLAT, MONS_FLYING_SKULL },
  {  1,  7,  690, SEMI, MONS_FREEZING_WRAITH },
  {  1,  7,  420, FLAT, MONS_PHANTASMAL_WARRIOR },
  {  1,  7, 1000, FLAT, MONS_SKELETAL_WARRIOR },
  {  1,  7,  192, FLAT, MONS_FLAYED_GHOST },
  {  1,  7,  260, FLAT, MONS_SILENT_SPECTRE },
  {  1,  7,  334, FLAT, MONS_BONE_DRAGON },
  {  1,  9,  125, PEAK, MONS_LICH },
  {  3,  7,   11, PEAK, MONS_ANCIENT_LICH },
  {  1,  7,  550, FALL, MONS_HELL_HOUND },
  {  1,  7,  729, FLAT, MONS_DOOM_HOUND },
  {  1,  7,   46, FALL, MONS_HELL_KNIGHT },
  {  1,  7,   35, PEAK, MONS_NECROMANCER },
  {  1,  7,   89, FALL, MONS_CRIMSON_IMP },
  {  1,  7, 1000, FLAT, MONS_SHADOW_IMP },
  {  1,  7,   40, PEAK, MONS_RED_DEVIL },
  {  1,  7,  245, SEMI, MONS_ICE_DEVIL },
  {  1,  9,  260, SEMI, MONS_SOUL_EATER },
  {  1,  7,  860, FLAT, MONS_REAPER },
  {  1,  7,  365, FLAT, MONS_TORMENTOR },
  {  1,  7,  365, FLAT, MONS_HELLION },
  {  1,  7,  365, FLAT, MONS_SHADOW_DEMON },
  {  1,  7,  230, SEMI, MONS_GHOST_CRAB },
  {  1,  7,   89, FLAT, MONS_DEATH_DRAKE },
  {  1,  7,   89, SEMI, MONS_SHADOW_DRAGON },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_zot[] =
{ // Zot
  {  1,  5,  970, FLAT, MONS_MOTH_OF_WRATH },
  {  1,  5,  192, RISE, MONS_GHOST_MOTH },
  {  1,  5,  113, FLAT, MONS_BLACK_DRACONIAN },
  {  1,  5,  113, FLAT, MONS_YELLOW_DRACONIAN },
  {  1,  5,  113, FLAT, MONS_GREEN_DRACONIAN },
  {  1,  5,  113, FLAT, MONS_PURPLE_DRACONIAN },
  {  1,  5,  113, FLAT, MONS_RED_DRACONIAN },
  {  1,  5,  113, FLAT, MONS_WHITE_DRACONIAN },
  {  1,  5,   70, FLAT, MONS_DRACONIAN_STORMCALLER },
  {  1,  5,   70, FLAT, MONS_DRACONIAN_MONK },
  {  1,  5,   70, FLAT, MONS_DRACONIAN_SHIFTER },
  {  1,  5,   70, FLAT, MONS_DRACONIAN_ANNIHILATOR },
  {  1,  5,   70, FLAT, MONS_DRACONIAN_KNIGHT },
  {  1,  5,   70, FLAT, MONS_DRACONIAN_SCORCHER },
  {  1,  5,  200, FLAT, MONS_QUICKSILVER_DRAGON },
  {  1,  5,  200, FLAT, MONS_SHADOW_DRAGON },
  { -4,  5,  515, RISE, MONS_STORM_DRAGON },
  {  1, 11,  365, SEMI, MONS_GOLDEN_DRAGON },
  {  2,  8,   52, SEMI, MONS_KILLER_KLOWN },
  {  1,  5,  335, FLAT, MONS_DEATH_COB },
  {  1,  5,  150, RISE, MONS_CURSE_TOE },
  {  1,  5,  515, FLAT, MONS_TENTACLED_MONSTROSITY },
  {  1,  5,   89, FALL, MONS_ELECTRIC_GOLEM },
  {  1,  5,   42, FLAT, MONS_ORB_OF_FIRE },
  {  1,  9,  150, SEMI, MONS_ORB_DEMON },
  { 0,0,0,FLAT,MONS_0 }
};
#if TAG_MAJOR_VERSION == 34
static const pop_entry pop_forest[] =
{ // Forest
  {  1,  5,  120, FALL, MONS_WOLF },
  {  1,  5,   35, FALL, MONS_BLACK_BEAR },
  {  1,  5,   50, FLAT, MONS_YAK },
  {  1,  7,  145, SEMI, MONS_DIRE_ELEPHANT },
  {  1,  5,   45, FLAT, MONS_HORNET },
  {  1,  6,   75, FALL, MONS_REDBACK },
  {  2,  7,   35, SEMI, MONS_WOLF_SPIDER },
  {  1,  9,   75, SEMI, MONS_OKLOB_PLANT },
  {  1,  5,  170, FLAT, MONS_DRYAD },
  {  1,  5,  120, FLAT, MONS_WIND_DRAKE },
  { -1,  5,   75, SEMI, MONS_FAUN },
  {  0,  9,  105, SEMI, MONS_SATYR },
  {  2,  8,   55, SEMI, MONS_SPRIGGAN_DRUID },
  {  1,  6,  155, SEMI, MONS_SPRIGGAN_RIDER },
  {  1,  9,  235, SEMI, MONS_SPRIGGAN_BERSERKER },
  {  1,  8,  155, SEMI, MONS_SPRIGGAN_AIR_MAGE },
  {  3,  5,  115, RISE, MONS_SPRIGGAN_DEFENDER },
  {  1,  7,   85, PEAK, MONS_APIS },
  {  2,  7,  165, SEMI, MONS_SHAMBLING_MANGROVE },
  {  1,  6,   85, SEMI, MONS_ANACONDA },
  {  1,  9,  100, PEAK, MONS_THORN_HUNTER },
  {  1,  5,  125, FLAT, MONS_BUTTERFLY },
  { 0,0,0,FLAT,MONS_0 }
};
#endif

static const pop_entry pop_abyss[] =
{ // Abyss
  {  1,  5,   50, FLAT, MONS_SHAPESHIFTER },
  {  1, 10,   10, SEMI, MONS_GLOWING_SHAPESHIFTER },
  {  1,  5,   35, FLAT, MONS_UNSEEN_HORROR },
  { -1,  6, 1500, FALL, MONS_ABOMINATION_SMALL },
  {  1,  5, 1000, FLAT, MONS_ABOMINATION_LARGE },
  {  1, 10,   16, SEMI, MONS_TENTACLED_MONSTROSITY },
  {  1,  5,  335, FLAT, MONS_WRETCHED_STAR },
  {  1,  5,  335, FLAT, MONS_TENTACLED_STARSPAWN },
  {  1,  5,  335, FLAT, MONS_ANCIENT_ZYME },
  {  1,  5,  335, FLAT, MONS_STARCURSED_MASS },
  {  1,  5,  335, FLAT, MONS_THRASHING_HORROR },
  {  1,  5,   89, FLAT, MONS_LURKING_HORROR },
  {  1, 10,  180, SEMI, MONS_SPATIAL_MAELSTROM },
  {  1,  5,   89, FLAT, MONS_APOCALYPSE_CRAB },
  {  1,  5,  300, FLAT, MONS_RAIJU },
  {  1,  8,  165, FALL, MONS_WORLDBINDER },
  {  1,  5,   52, FLAT, MONS_VERY_UGLY_THING },
  // Demons
  {  1,  4,   25, FALL, MONS_CRIMSON_IMP },
  {  1,  4,  192, FALL, MONS_WHITE_IMP },
  {  1,  4,  192, FALL, MONS_QUASIT },
  {  1,  4,  192, FALL, MONS_UFETUBUS },
  {  1,  4,  192, FALL, MONS_IRON_IMP },
  {  1,  4,   25, FALL, MONS_SHADOW_IMP },
  {  1,  5,    8, FALL, MONS_RED_DEVIL },
  {  1,  5,   25, FALL, MONS_ICE_DEVIL },
  {  1,  5,   36, FALL, MONS_RUST_DEVIL },
  {  1,  8,   73, FALL, MONS_HELLWING },
  {  1,  8,   91, FALL, MONS_SIXFIRHY },
  {  1,  5,  125, FALL, MONS_ORANGE_DEMON },
  {  1,  8,  315, FALL, MONS_YNOXINUL },
  {  1,  5,  225, FLAT, MONS_NEQOXEC },
  {  1,  5,  300, FLAT, MONS_SMOKE_DEMON },
  {  1,  5,  192, FLAT, MONS_CHAOS_SPAWN },
  {  1,  5,   22, FLAT, MONS_SUN_DEMON },
  {  1,  5,   14, FLAT, MONS_SOUL_EATER },
  {  1,  5,   25, FLAT, MONS_HELLION },
  {  1,  5,   25, FLAT, MONS_LOROCYPROCA },
  {  1,  5,   21, FLAT, MONS_TORMENTOR },
  {  1,  5,   12, FLAT, MONS_REAPER },
  {  1,  5,   22, FLAT, MONS_GREEN_DEATH },
  {  1,  5,   22, FLAT, MONS_BLIZZARD_DEMON },
  {  1,  5,   22, FLAT, MONS_BALRUG },
  {  1,  5,   25, FLAT, MONS_CACODEMON },
  {  1, 10,   16, SEMI, MONS_SHADOW_DEMON },
  {  1,  5,   25, FLAT, MONS_EXECUTIONER },
  {  1,  5,   10, FLAT, MONS_HELL_SENTINEL },
  {  1,  5,   63, FLAT, MONS_EFREET },
  {  1,  5,   66, FLAT, MONS_RAKSHASA },
  {  1,  5,   25, FLAT, MONS_DEMONIC_CRAWLER },
  {  1,  8,   30, FALL, MONS_HELL_HOUND },
  {  1, 10,   40, SEMI, MONS_HELL_HOG },
  {  1, 10,   16, SEMI, MONS_HELLEPHANT },
  // Misc Undead
  {  1,  5,  110, FALL, MONS_SKELETON },
  {  1,  5,  104, FALL, MONS_ZOMBIE },
  {  1,  5,   42, FLAT, MONS_SIMULACRUM },
  {  1,  4,   25, FALL, MONS_WIGHT },
  {  1,  5,   30, FALL, MONS_HUNGRY_GHOST },
  {  1,  8,   52, FALL, MONS_FLYING_SKULL },
  {  1,  8,   52, FALL, MONS_WRAITH },
  {  1,  8,   30, FALL, MONS_SHADOW },
  {  1,  8,   21, FALL, MONS_VAMPIRE },
  {  1,  5,   40, FLAT, MONS_FREEZING_WRAITH },
  {  1,  5,   23, FLAT, MONS_SHADOW_WRAITH },
  {  1,  5,    8, FLAT, MONS_SILENT_SPECTRE },
  {  1,  5,    8, FLAT, MONS_PHANTASMAL_WARRIOR },
  {  1, 10,   22, SEMI, MONS_PROFANE_SERVITOR },
  {  1,  5,   80, FLAT, MONS_BONE_DRAGON },
  {  1,  5,    9, FLAT, MONS_REVENANT },
  {  1,  5,   46, FLAT, MONS_LICH },
  {  1,  5,    8, FLAT, MONS_ANCIENT_LICH },
  // Thematic branch representatives, outsiders
  {  1,  5,    8, FLAT, MONS_ORC_SORCERER },
  {  1,  5,    4, FLAT, MONS_DEEP_ELF_MAGE },
  {  1,  5,    8, FLAT, MONS_DEEP_ELF_SORCERER },
  {  1,  5,    5, FLAT, MONS_DEEP_ELF_DEMONOLOGIST },
  {  1,  5,    8, FLAT, MONS_DEEP_ELF_ELEMENTALIST },
  {  1,  5,   29, FLAT, MONS_BLINK_FROG },
  {  1,  5,    4, FALL, MONS_BASILISK },
  {  1,  5,    8, FLAT, MONS_MANA_VIPER },
  {  1,  5,   25, FLAT, MONS_GUARDIAN_SERPENT },
  {  1,  8,   35, FALL, MONS_INSUBSTANTIAL_WISP },
  {  1,  5,    8, FLAT, MONS_ORB_SPIDER },
  {  1, 10,    4, SEMI, MONS_GHOST_MOTH },
  {  1,  5,   17, FLAT, MONS_EIDOLON },
  {  1, 10,   18, SEMI, MONS_FLAYED_GHOST },
  {  1,  5,   12, FLAT, MONS_VAMPIRE_MAGE },
  {  1,  5,    3, FLAT, MONS_DANCING_WEAPON },
  {  1,  5,   21, FALL, MONS_JELLY },
  {  1,  5,   46, FLAT, MONS_EYE_OF_DRAINING },
  {  1,  5,    8, FLAT, MONS_SHINING_EYE },
  {  1,  5,   33, FLAT, MONS_GOLDEN_EYE },
  {  1,  5,    8, FLAT, MONS_EYE_OF_DEVASTATION },
  {  1,  5,   33, FLAT, MONS_GREAT_ORB_OF_EYES },
  {  1,  5,   52, FLAT, MONS_GLOWING_ORANGE_BRAIN },
  {  1,  5,    5, FLAT, MONS_BLACK_DRACONIAN },
  {  1,  5,    5, FLAT, MONS_YELLOW_DRACONIAN },
  {  1,  5,    5, FLAT, MONS_GREEN_DRACONIAN },
  {  1,  5,    5, FLAT, MONS_PURPLE_DRACONIAN },
  {  1,  5,    5, FLAT, MONS_RED_DRACONIAN },
  {  1,  5,    5, FLAT, MONS_WHITE_DRACONIAN },
  {  1,  5,    3, FLAT, MONS_DRACONIAN_STORMCALLER },
  {  1,  5,    3, FLAT, MONS_DRACONIAN_MONK },
  {  1,  5,    5, FLAT, MONS_DRACONIAN_SHIFTER },
  {  1,  5,    3, FLAT, MONS_DRACONIAN_ANNIHILATOR },
  {  1,  5,    3, FLAT, MONS_DRACONIAN_KNIGHT },
  {  1,  5,    3, FLAT, MONS_DRACONIAN_SCORCHER },
  {  3,  5,    1, FLAT, MONS_CORRUPTER },
  // Misc
  {  1,  5,    8, FLAT, MONS_DEATH_DRAKE },
  {  1,  5,    2, FLAT, MONS_SHADOW_DRAGON },
  {  1,  5,    2, FLAT, MONS_QUICKSILVER_DRAGON },
  {  1,  5,    2, FLAT, MONS_CRYSTAL_GUARDIAN },
  {  1,  5,    1, FLAT, MONS_TOENAIL_GOLEM },
  {  1,  5,   18, FLAT, MONS_EARTH_ELEMENTAL },
  {  1,  5,    6, FLAT, MONS_FIRE_ELEMENTAL },
  {  1,  5,    6, FLAT, MONS_AIR_ELEMENTAL },
  {  1,  5,    6, FLAT, MONS_WATER_ELEMENTAL },
  {  1,  5,   46, FALL, MONS_FIRE_BAT },
  {  1,  4,   21, FALL, MONS_ICE_BEAST },
  {  1,  4,   21, FALL, MONS_SKY_BEAST },
  {  1,  5,   14, FALL, MONS_KOBOLD_DEMONOLOGIST },
  {  1,  5,    8, FLAT, MONS_DEATH_KNIGHT },
  {  1,  5,    8, FLAT, MONS_HELL_KNIGHT },
  {  1,  5,    8, FLAT, MONS_NECROMANCER },
  {  1,  5,    8, FLAT, MONS_WIZARD },
  {  1,  5,   11, FLAT, MONS_ANGEL },
  {  1,  5,   14, FLAT, MONS_DAEVA },
  {  1,  5,    8, FLAT, MONS_OPHAN },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_pan[] =
{ // Pandemonium - only used for 1 in 40 random Pan spawns (the rest are drawn
  // from each floor's monster list), and for the Orb run (where monsters are
  // picked randomly without checking rarity).
  {  1,  1,    4, FLAT, MONS_EYE_OF_DRAINING },
  {  1,  1,    4, FLAT, MONS_GOLDEN_EYE },
  {  1,  1,    4, FLAT, MONS_GREAT_ORB_OF_EYES },
  {  1,  1,    4, FLAT, MONS_GLOWING_ORANGE_BRAIN },
  {  1,  1,    8, FLAT, MONS_TOENAIL_GOLEM },
  {  1,  1,  192, FLAT, MONS_DEMONIC_CRAWLER },
  {  1,  1,   25, FLAT, MONS_HELL_HOUND },
  {  1,  1,   25, FLAT, MONS_HELL_HOG },
  {  1,  1, 1000, FLAT, MONS_ABOMINATION_LARGE },
  {  1,  1,   89, FLAT, MONS_EFREET },
  {  1,  1,   89, FLAT, MONS_RAKSHASA },
  {  1,  1,   25, FLAT, MONS_HELLEPHANT },
  {  1,  1, 1000, FLAT, MONS_ORANGE_DEMON },
  {  1,  1, 1000, FLAT, MONS_HELLWING },
  {  1,  1, 1000, FLAT, MONS_SIXFIRHY },
  {  1,  1,  670, FLAT, MONS_RED_DEVIL },
  {  1,  1,  335, FLAT, MONS_ICE_DEVIL },
  {  1,  1,  335, FLAT, MONS_RUST_DEVIL },
  {  1,  1, 1000, FLAT, MONS_YNOXINUL },
  {  1,  1,  400, FLAT, MONS_NEQOXEC },
  {  1,  1,  900, FLAT, MONS_SMOKE_DEMON },
  {  1,  1,  335, FLAT, MONS_SUN_DEMON },
  {  1,  1,  335, FLAT, MONS_SOUL_EATER },
  {  1,  1,  192, FLAT, MONS_CHAOS_SPAWN },
  {  1,  1, 1000, FLAT, MONS_BALRUG },
  {  1,  1, 1000, FLAT, MONS_BLIZZARD_DEMON },
  {  1,  1, 1000, FLAT, MONS_GREEN_DEATH },
  {  1,  1, 1000, FLAT, MONS_CACODEMON },
  {  1,  1,  335, FLAT, MONS_HELLION },
  {  1,  1,  335, FLAT, MONS_TORMENTOR },
  {  1,  1,  335, FLAT, MONS_REAPER },
  {  1,  1,  335, FLAT, MONS_SHADOW_DEMON },
  {  1,  1,  335, FLAT, MONS_HELL_BEAST },
  {  1,  1,  335, FLAT, MONS_LOROCYPROCA },
  {  1,  1, 1000, FLAT, MONS_EXECUTIONER },
  {  1,  1,  335, FLAT, MONS_BRIMSTONE_FIEND },
  {  1,  1,  335, FLAT, MONS_ICE_FIEND },
  {  1,  1,  335, FLAT, MONS_HELL_SENTINEL },
  {  1,  1,  335, FLAT, MONS_SHADOW_FIEND },
  {  1,  1,    8, FLAT, MONS_PROFANE_SERVITOR },
  {  1,  1,  250, FLAT, MONS_MONSTROUS_DEMONSPAWN },
  {  1,  1,  250, FLAT, MONS_GELID_DEMONSPAWN },
  {  1,  1,  250, FLAT, MONS_INFERNAL_DEMONSPAWN },
  {  1,  1,  250, FLAT, MONS_TORTUROUS_DEMONSPAWN },
  {  1,  1,  625, FLAT, MONS_BLOOD_SAINT },
  {  1,  1,  625, FLAT, MONS_WARMONGER },
  {  1,  1,  625, FLAT, MONS_CORRUPTER },
  {  1,  1,  625, FLAT, MONS_BLACK_SUN },
  {  1,  1,   50, FLAT, MONS_ANGEL },
  {  1,  1,   40, FLAT, MONS_CHERUB },
  {  1,  1,   25, FLAT, MONS_DAEVA },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_zig[] =
{ // Ziggurat
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_gauntlet[] =
{ // Gauntlet
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_bazaar[] =
{ // Bazaar
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_trove[] =
{ // Trove
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_desolation[] =
{ // Desolation
  {  1,  1, 1200, FLAT, MONS_SALTLING },
  {  1,  1,   50, FLAT, MONS_DANCING_WEAPON },
  {  1,  1,   50, FLAT, MONS_MOLTEN_GARGOYLE },
  {  1,  1,   50, FLAT, MONS_CRYSTAL_GUARDIAN },
  {  1,  1,   50, FLAT, MONS_IMPERIAL_MYRMIDON },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_sewer[] =
{ // Sewer
  {  1,  1, 1000, FLAT, MONS_FRILLED_LIZARD },
  {  1,  1,  515, FLAT, MONS_LEOPARD_GECKO },
  {  1,  1, 1000, FLAT, MONS_BAT },
  {  1,  1,  515, FLAT, MONS_BALL_PYTHON },
  {  1,  1,  515, FLAT, MONS_ADDER },
  {  1,  1,  515, FLAT, MONS_WORM },
  {  1,  1,  515, FLAT, MONS_OOZE },
  {  1,  1,  515, FLAT, MONS_GIANT_COCKROACH },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_ossuary[] =
{ // Ossuary
  {  1,  1,   89, FLAT, MONS_MUMMY },
  {  1,  1,  515, FLAT, MONS_SKELETON },
  {  1,  1,  515, FLAT, MONS_ZOMBIE },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_bailey[] =
{ // Bailey
  {  1,  1,  515, FLAT, MONS_GNOLL },
  {  1,  1,  515, FLAT, MONS_ORC },
  {  1,  1,  260, FLAT, MONS_ORC_WARRIOR },
  {  1,  1,   25, FLAT, MONS_ORC_KNIGHT },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_icecv[] =
{ // Ice Cave
  {  1,  1,   89, FLAT, MONS_YAK },
  {  1,  1,   89, FLAT, MONS_POLAR_BEAR },
  {  1,  1,  515, FLAT, MONS_ICE_BEAST },
  {  1,  1,  515, FLAT, MONS_WHITE_IMP },
  {  1,  1,  515, FLAT, MONS_ICE_DEVIL },
  {  1,  1,  515, FLAT, MONS_RIME_DRAKE },
  {  1,  1,  260, FLAT, MONS_FREEZING_WRAITH },
  {  1,  1, 1030, FLAT, MONS_SIMULACRUM },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_volcano[] =
{ // Volcano
  {  1,  1,  515, FLAT, MONS_SALAMANDER },
  {  1,  1,  515, FLAT, MONS_HELL_HOUND },
  {  1,  1,  385, FLAT, MONS_RED_DEVIL },
  {  1,  1,  515, FLAT, MONS_EFREET },
  {  1,  1,  515, FLAT, MONS_LINDWURM },
  {  1,  1,  255, FLAT, MONS_TOENAIL_GOLEM },
  {  1,  1,  515, FLAT, MONS_FIRE_ELEMENTAL },
  {  1,  1,  385, FLAT, MONS_GARGOYLE },
  {  1,  1,  515, FLAT, MONS_MOLTEN_GARGOYLE },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_wizlab[] =
{ // Wizlab
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_depths[] =
{ // Depths (OOD cap: 14)
  { -2,  4,  100, SEMI, MONS_BOGGART },
  { -2,  4,  200, SEMI, MONS_SLIME_CREATURE },
  { -2,  4,  250, SEMI, MONS_RAKSHASA },
  { -2,  4,  100, SEMI, MONS_WIZARD },
  { -2,  4,  100, SEMI, MONS_NECROMANCER },
  { -2, 10,  250, PEAK, MONS_FIRE_GIANT },
  { -2, 10,  260, PEAK, MONS_FROST_GIANT },
  { -2, 10,   70, PEAK, MONS_HELL_KNIGHT },
  { -2, 10,  100, PEAK, MONS_VAMPIRE_KNIGHT },
  {  1,  3,  300, FALL, MONS_UGLY_THING },
  {  1,  4,  250, FALL, MONS_CENTAUR_WARRIOR },
  {  1,  4,  300, FALL, MONS_OGRE_MAGE },
  {  1,  6,  375, FALL, MONS_YAKTAUR },
  {  1,  6,  375, RISE, MONS_YAKTAUR_CAPTAIN },
  {  1,  6,  300, FALL, MONS_SHAPESHIFTER },
  {  1,  6,  180, FLAT, MONS_DEEP_TROLL_SHAMAN },
  {  1,  6,  180, FLAT, MONS_DEEP_TROLL_EARTH_MAGE },
  {  1,  6,  295, FLAT, MONS_FIRE_DRAGON },
  {  1,  6,  295, FLAT, MONS_ICE_DRAGON },
  {  1,  6,  250, FLAT, MONS_VERY_UGLY_THING },
  {  1,  6,  225, FLAT, MONS_GREAT_ORB_OF_EYES },
  {  1,  6,  100, FLAT, MONS_GLOWING_ORANGE_BRAIN },
  {  1,  7,  100, FALL, MONS_VAMPIRE_MAGE },
  {  1,  7,  200, FALL, MONS_TENGU_WARRIOR },
  {  1,  7,  100, FALL, MONS_TENGU_CONJURER },
  {  1,  8,  600, FALL, MONS_STONE_GIANT },
  {  1,  8,  370, FALL, MONS_ETTIN },
  {  1, 10,  100, SEMI, MONS_SPARK_WASP },
  {  1, 12,   60, FLAT, MONS_LICH },
  {  1, 12,   40, FLAT, MONS_FLAYED_GHOST },
  {  1, 14,   80, SEMI, MONS_GLOWING_SHAPESHIFTER },
  {  1, 14,   80, SEMI, MONS_TENGU_REAVER },
  {  1, 14,  100, SEMI, MONS_SPHINX },
  {  2,  8,  135, SEMI, MONS_SPRIGGAN_AIR_MAGE },
  {  2,  8,  185, SEMI, MONS_SPRIGGAN_BERSERKER },
  {  2, 10,   25, FLAT, MONS_FLOATING_EYE },
  {  3, 14,   45, FLAT, MONS_SPRIGGAN_DEFENDER },
  {  4, 14,   80, SEMI, MONS_TENTACLED_MONSTROSITY },
  {  4, 14,   40, FLAT, MONS_STORM_DRAGON },
  {  4, 14,   40, FLAT, MONS_SHADOW_DRAGON },
  {  4, 14,   20, FLAT, MONS_QUICKSILVER_DRAGON },
  {  4, 14,   40, FLAT, MONS_IRON_DRAGON },
  {  4, 14,   80, SEMI, MONS_GOLDEN_DRAGON },
  {  4, 14,   50, RISE, MONS_DEEP_ELF_HIGH_PRIEST },
  {  4, 14,   25, RISE, MONS_DEEP_ELF_DEMONOLOGIST },
  {  4, 14,   25, RISE, MONS_DEEP_ELF_ANNIHILATOR },
  {  4, 14,   25, RISE, MONS_DEEP_ELF_SORCERER },
  {  4, 14,   25, RISE, MONS_DEEP_ELF_DEATH_MAGE },
  {  4, 14,   25, RISE, MONS_DEEP_ELF_ELEMENTALIST },
  {  5, 14,   50, FLAT, MONS_JUGGERNAUT },
  {  5, 14,   50, FLAT, MONS_CAUSTIC_SHRIKE },
  {  6, 14,   50, FLAT, MONS_TITAN },
  { 10, 14,   10, FLAT, MONS_ANCIENT_LICH },
  { 0,0,0,FLAT,MONS_0 }
};

struct population_list
{
    const pop_entry *pop;
    int count;
};

// This list must be in the same order as the branch-type enum values.
//
// In C++ prior to C++11, we can't have an array of pointers to initialized
// arrays of different sizes without declaring individual arrays as
// standalone symbols first. Thus this fugly list.
// Statically defined counts are merely an inconsequential nicety, no need
// to preserve them.
#define POP(x) { pop_##x, ARRAYSZ(pop_##x) - 1 }
static const population_list population[] =
{
    POP(d),
    POP(temple),
    POP(orc),
    POP(elf),
#if TAG_MAJOR_VERSION == 34
    POP(dwarf),
#endif
    POP(lair),
    POP(swamp),
    POP(shoals),
    POP(snake),
    POP(spider),
    POP(slime),
    POP(vaults),
#if TAG_MAJOR_VERSION == 34
    POP(blade),
#endif
    POP(crypt),
    POP(tomb),
#if TAG_MAJOR_VERSION > 34
    POP(depths),
#endif
    POP(hell),
    POP(dis),
    POP(geh),
    POP(coc),
    POP(tar),
    POP(zot),
#if TAG_MAJOR_VERSION == 34
    POP(forest),
#endif
    POP(abyss),
    POP(pan),
    POP(zig),
#if TAG_MAJOR_VERSION == 34
    POP(lab),
#endif
    POP(bazaar),
    POP(trove),
    POP(sewer),
    POP(ossuary),
    POP(bailey),
#if TAG_MAJOR_VERSION > 34
    POP(gauntlet),
#endif
    POP(icecv),
    POP(volcano),
    POP(wizlab),
#if TAG_MAJOR_VERSION == 34
    POP(depths),
#endif
    POP(desolation),
#if TAG_MAJOR_VERSION == 34
    POP(gauntlet),
#endif
};
COMPILE_CHECK(ARRAYSZ(population) == NUM_BRANCHES);

// Lists for picking zombies from.

static const pop_entry pop_generic_late_zombie[] =
{ // Extended generic zombie bases
  {  1,  27,  110, FLAT, MONS_ETTIN },
  {  1,  27,  105, FLAT, MONS_FIRE_GIANT },
  {  1,  27,  105, FLAT, MONS_FROST_GIANT },
  {  1,  27,  105, FLAT, MONS_STONE_GIANT },
  {  1,  27,  100, FLAT, MONS_TITAN },
  {  1,  27,   80, FLAT, MONS_FIRE_DRAGON },
  {  1,  27,   80, FLAT, MONS_ICE_DRAGON },
  {  1,  27,   80, FLAT, MONS_STORM_DRAGON },
  {  1,  27,   80, FLAT, MONS_SHADOW_DRAGON },
  {  1,  27,   80, FLAT, MONS_IRON_DRAGON },
  {  1,  27,   80, FLAT, MONS_QUICKSILVER_DRAGON },
  {  1,  27,   65, FLAT, MONS_GOLDEN_DRAGON },
  {  1,  27,   80, FLAT, MONS_DIRE_ELEPHANT },
  {  1,  27,   45, FLAT, MONS_CANE_TOAD },
  {  1,  27,   25, FLAT, MONS_CATOBLEPAS },
  {  1,  27,   90, FLAT, MONS_HYDRA },
  {  1,  27,   30, FLAT, MONS_SWAMP_DRAGON },
  {  1,  27,   30, FLAT, MONS_ALLIGATOR },
  {  1,  27,   85, FLAT, MONS_ANACONDA },
  {  1,  27,   35, FLAT, MONS_BLACK_MAMBA },
  {  1,  27,   30, FLAT, MONS_SHOCK_SERPENT },
  {  1,  27,   75, FLAT, MONS_SNAPPING_TURTLE },
  {  1,  27,   45, FLAT, MONS_HARPY },
  {  1,  27,   30, FLAT, MONS_MANTICORE },
  {  1,  27,   80, FLAT, MONS_WOLF_SPIDER },
  {  1,  27,   35, FLAT, MONS_EMPEROR_SCORPION },
  {  1,  27,   35, FLAT, MONS_GHOST_MOTH },
  {  1,  27,   10, FLAT, MONS_CENTAUR },
  {  1,  27,   12, FLAT, MONS_ELF },
  {  1,  27,   27, FLAT, MONS_HUMAN },
  {  1,  27,   10, FLAT, MONS_KOBOLD },
  {  1,  27,   10, FLAT, MONS_MERFOLK },
  {  1,  27,   10, FLAT, MONS_NAGA },
  {  1,  27,   10, FLAT, MONS_OGRE },
  {  1,  27,   12, FLAT, MONS_ORC },
  {  1,  27,   10, FLAT, MONS_SPRIGGAN },
  {  1,  27,   10, FLAT, MONS_TENGU },
  {  1,  27,   10, FLAT, MONS_TROLL },
  {  1,  27,    1, FLAT, MONS_BLACK_DRACONIAN },
  {  1,  27,    1, FLAT, MONS_YELLOW_DRACONIAN },
  {  1,  27,    1, FLAT, MONS_GREEN_DRACONIAN },
  {  1,  27,    1, FLAT, MONS_PURPLE_DRACONIAN },
  {  1,  27,    1, FLAT, MONS_RED_DRACONIAN },
  {  1,  27,    1, FLAT, MONS_WHITE_DRACONIAN },
  {  1,  27,    5, FLAT, MONS_MINOTAUR },
  {  1,  27,    1, FLAT, MONS_MONSTROUS_DEMONSPAWN },
  {  1,  27,    1, FLAT, MONS_INFERNAL_DEMONSPAWN },
  {  1,  27,    1, FLAT, MONS_GELID_DEMONSPAWN },
  {  1,  27,    1, FLAT, MONS_TORTUROUS_DEMONSPAWN },
  { 0,0,0,FLAT,MONS_0 }
};

// This list must be in the same order as the branch-type enum values.
static const population_list population_zombie[] =
{
    POP(d),
    POP(temple),
    POP(orc),
    POP(elf),
#if TAG_MAJOR_VERSION == 34
    POP(dwarf),
#endif
    POP(lair),
    POP(swamp),
    POP(shoals),
    POP(snake),
    POP(spider),
    POP(slime),
    POP(vaults),
#if TAG_MAJOR_VERSION == 34
    POP(blade),
#endif
    POP(generic_late_zombie), // Crypt
    POP(generic_late_zombie), // Tomb
#if TAG_MAJOR_VERSION > 34
    POP(depths),
#endif
    POP(generic_late_zombie), // Vestibule
    POP(generic_late_zombie), // Dis
    POP(generic_late_zombie), // Geh
    POP(generic_late_zombie), // Coc
    POP(generic_late_zombie), // Tar
    POP(zot),
#if TAG_MAJOR_VERSION == 34
    POP(forest),
#endif
    POP(generic_late_zombie), // Abyss
    POP(generic_late_zombie), // Pan
    POP(zig),
#if TAG_MAJOR_VERSION == 34
    POP(lab),
#endif
    POP(bazaar),
    POP(trove),
    POP(sewer),
    POP(ossuary),
    POP(bailey),
#if TAG_MAJOR_VERSION > 34
    POP(gauntlet),
#endif
    POP(icecv),
    POP(volcano),
    POP(wizlab),
#if TAG_MAJOR_VERSION == 34
    POP(depths),
#endif
    POP(desolation),
#if TAG_MAJOR_VERSION == 34
    POP(gauntlet),
#endif
};
COMPILE_CHECK(ARRAYSZ(population_zombie) == NUM_BRANCHES);

static const pop_entry pop_water_generic[] =
{ // Generic water monsters
  {  1,  27,  150, FLAT, MONS_ELECTRIC_EEL },
  {  1,  27,  500, FLAT, MONS_NO_MONSTER },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_water_d[] =
{ // Dungeon water monsters
  {  5,  16,   60, FLAT, MONS_ELECTRIC_EEL },
  {  7,  16,  185, PEAK, MONS_ELECTRIC_EEL },
  {  11, 27,  600, RISE, MONS_WATER_ELEMENTAL },
  {  5,  22,  130, FLAT, MONS_NO_MONSTER },
  {  9,  32,  250, SEMI, MONS_NO_MONSTER },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_water_depths[] =
{ // Depths water monsters
  {  1,  6,   600, FLAT, MONS_WATER_ELEMENTAL },
  {  1,  6,    45, FLAT, MONS_MERFOLK_IMPALER },
  {  1,  6,    45, FLAT, MONS_MERFOLK_JAVELINEER },
  {  1,  6,   200, FALL, MONS_NO_MONSTER },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_water_swamp[] =
{ // Swamp water monsters
  {  1,  4,   400, FLAT, MONS_SWAMP_WORM },
  {  1,  4,   100, FLAT, MONS_TYRANT_LEECH },
  {  1,  4,   100, FLAT, MONS_ALLIGATOR },
  {  1,  4,  1050, FLAT, MONS_NO_MONSTER },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_water_snake[] =
{ // Snake water monsters
  {  1,   4,  100, FALL, MONS_ELECTRIC_EEL },
  {  0,   4,  200, RISE, MONS_SEA_SNAKE },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_water_hell[] =
{ // Hell branch water monsters
  {  1,  7,   150, FLAT, MONS_CANE_TOAD },
  {  1,  7,   120, FLAT, MONS_ALLIGATOR },
  {  1,  7,   130, FLAT, MONS_ANACONDA },
  {  1,  7,   230, FLAT, MONS_HYDRA },
  {  1,  7,    37, FLAT, MONS_KRAKEN },
  {  1,  7,   115, FLAT, MONS_WATER_ELEMENTAL },
  {  0,  8,    74, RISE, MONS_ELEMENTAL_WELLSPRING },
  { 0,0,0,FLAT,MONS_0 }
};

// This list must be in the same order as the branch-type enum values.
// Shoals, Abyss, Pan, Zot, D:1-5 liquid monsters are blocked in dungeon.cc
static const population_list population_water[] =
{
    POP(water_d),
    POP(water_generic), // Temple
    POP(water_generic), // Orc
    POP(water_generic), // Elf
#if TAG_MAJOR_VERSION == 34
    POP(water_generic), // Dwarf
#endif
    POP(water_generic), // Lair
    POP(water_swamp),
    POP(water_generic), // Shoals
    POP(water_snake),
    POP(water_generic), // Spider
    POP(water_generic), // Slime
    POP(water_generic), // Vaults
#if TAG_MAJOR_VERSION == 34
    POP(water_generic), // Blade
#endif
    POP(water_generic), // Crypt
    POP(water_generic), // Tomb
#if TAG_MAJOR_VERSION > 34
    POP(water_depths),
#endif
    POP(water_hell), // Vestibule
    POP(water_hell), // Dis
    POP(water_hell), // Geh
    POP(water_hell), // Coc
    POP(water_hell), // Tar
    POP(water_generic), // Zot
#if TAG_MAJOR_VERSION == 34
    POP(water_generic), // Forest
#endif
    POP(water_generic), // Abyss
    POP(water_generic), // Pan
    POP(water_generic), // Zig
#if TAG_MAJOR_VERSION == 34
    POP(water_generic), // Lab
#endif
    POP(water_generic), // Bazaar
    POP(water_generic), // Trove
    POP(water_generic), // Sewer
    POP(water_generic), // Ossuary
    POP(water_generic), // Bailey
#if TAG_MAJOR_VERSION > 34
    POP(water_generic), // Gauntlet
#endif
    POP(water_generic), // IceCv
    POP(water_generic), // Volcano
    POP(water_generic), // WizLab
#if TAG_MAJOR_VERSION == 34
    POP(water_depths),
#endif
    POP(water_generic), // Desolation
#if TAG_MAJOR_VERSION == 34
    POP(water_generic), // Gauntlet
#endif
};
COMPILE_CHECK(ARRAYSZ(population_water) == NUM_BRANCHES);

static const pop_entry pop_lava_generic[] =
{ // Generic lava monsters
  {  1,  27,  100, FLAT, MONS_FIRE_BAT },
  {  1,  27,  100, FLAT, MONS_FIRE_ELEMENTAL },
  {  1,  27,   50, FLAT, MONS_MOLTEN_GARGOYLE },
  {  1,  27,   50, FLAT, MONS_FIRE_VORTEX },
  {  1,  27,  145, FLAT, MONS_LAVA_SNAKE },
  {  1,  27,   15, FLAT, MONS_SALAMANDER },
  {  1,  27,  290, FLAT, MONS_NO_MONSTER },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_lava_d[] =
{ // Dungeon lava monsters
  {  7,  27,  145, FLAT, MONS_LAVA_SNAKE },
  {  11, 27,  290, RISE, MONS_FIRE_ELEMENTAL },
  {  11, 27,  145, RISE, MONS_MOLTEN_GARGOYLE },
  {  11, 27,  145, RISE, MONS_FIRE_VORTEX },
  {  7,  27,  290, FLAT, MONS_NO_MONSTER },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_lava_snake[] =
{ // Snake lava monsters
  {  1,   4,  200, FLAT, MONS_LAVA_SNAKE },
  {  1,   4,  200, FLAT, MONS_SALAMANDER },
  {  0,   6,  85,  SEMI, MONS_SALAMANDER_MYSTIC },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_lava_depths[] =
{ // Depths lava monsters
  {  1,  6,   22, FALL, MONS_FIRE_ELEMENTAL },
  {  1,  6,   22, FALL, MONS_FIRE_BAT },
  {  1,  6,   11, FALL, MONS_FIRE_VORTEX },
  {  1,  6,   11, FALL, MONS_MOLTEN_GARGOYLE },
  {  1,  6,  100, FLAT, MONS_SALAMANDER },
  {  1,  8,   85, SEMI, MONS_SALAMANDER_MYSTIC },
  {  1,  6,  400, FLAT, MONS_NO_MONSTER },
  { 0,0,0,FLAT,MONS_0 }
};

static const pop_entry pop_lava_hell[] =
{ // Hell branch lava monsters
  {  1,  8,   300, FALL, MONS_FIRE_ELEMENTAL },
  {  1,  6,    25, FALL, MONS_MOLTEN_GARGOYLE },
  {  1,  7,    25, FLAT, MONS_SMOKE_DEMON },
  {  1,  7,   100, FLAT, MONS_NO_MONSTER },
  { 0,0,0,FLAT,MONS_0 }
};

// This list must be in the same order as the branch-type enum values.
static const population_list population_lava[] =
{
    POP(lava_d),
    POP(lava_generic), // Temple
    POP(lava_generic), // Orc
    POP(lava_generic), // Elf
#if TAG_MAJOR_VERSION == 34
    POP(lava_generic), // Dwarf
#endif
    POP(lava_generic), // Lair
    POP(lava_generic), // Swamp
    POP(lava_generic), // Shoals
    POP(lava_snake),
    POP(lava_generic), // Spider
    POP(lava_generic), // Slime
    POP(lava_generic), // Vaults
#if TAG_MAJOR_VERSION == 34
    POP(lava_generic), // Blade
#endif
    POP(lava_generic), // Crypt
    POP(lava_generic), // Tomb
#if TAG_MAJOR_VERSION > 34
    POP(lava_depths),
#endif
    POP(lava_hell), // Vestibule
    POP(lava_hell), // Dis
    POP(lava_hell), // Geh
    POP(lava_hell), // Coc
    POP(lava_hell), // Tar
    POP(lava_generic), // Zot
#if TAG_MAJOR_VERSION == 34
    POP(lava_generic), // Forest
#endif
    POP(lava_generic), // Abyss
    POP(lava_generic), // Pan
    POP(lava_generic), // Zig
#if TAG_MAJOR_VERSION == 34
    POP(lava_generic), // Lab
#endif
    POP(lava_generic), // Bazaar
    POP(lava_generic), // Trove
    POP(lava_generic), // Sewer
    POP(lava_generic), // Ossuary
    POP(lava_generic), // Bailey
#if TAG_MAJOR_VERSION > 34
    POP(lava_generic), // Gauntlet
#endif
    POP(lava_generic), // IceCv
    POP(lava_generic), // Volcano
    POP(lava_generic), // WizLab
#if TAG_MAJOR_VERSION == 34
    POP(lava_depths),
#endif
    POP(lava_generic), // Desolation
#if TAG_MAJOR_VERSION == 34
    POP(lava_generic), // Gauntlet
#endif
};
COMPILE_CHECK(ARRAYSZ(population_lava) == NUM_BRANCHES);
