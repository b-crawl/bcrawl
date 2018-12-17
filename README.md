# bcrawl

bcrawl is a fork of X-Crawl, which is a fork of Dungeon Crawl Stone Soup.

Like X-Crawl, discussion is done on the CXC Discord. (https://discord.gg/SzXGZNS)

## Releases
https://github.com/b-crawl/bcrawl/releases

## Changes from Vanilla Crawl
#### Branches
  - Traps are not placed in the first 3 floors of the Dungeon.
  - Dungeon is shortened to 12 floors.
  - Vaults does not require any runes to enter.
  - Vaults is shortened to 3 floors.
  - Lair Branches (Swamp, Spider, Snake, Shoals) are shortened to 3 floors.
  - Crypt is shortened to 2 floors.
  - Orc is shortened to 1 floor.
  
#### Backgrounds
  - Venom Mage, Arcane Marksman, and Warper are completely reworked. (based on Doesnty's work)
  - Reaver is a melee background that starts the game worshipping Vehumet.
  - Archaeologists start with a crate and a dusty tome. The tome reveals itself to be a random skill manual at XL 3, and when the manual is finished, the crate unlocks and gives the player a related artifact.
  - Necromancers start with Bolt of Draining.
  - 'Assassin' has been renamed to 'Rogue'.
  - v1.1: Skalds have a reworked Ozocubu's Armour instead of Shroud of Golubria.

#### Features
  - All items are automatically identified if the player has a rune.
  - v1.1.1: Like in mainline DCSS, all traps are revealed, but there are no trap effects from exploration.
  - v1.3: To replace traps, when taking stairs there's a 4% chance of being teleported onto the lower floor.

#### Balance Changes
  - The Paralyse spell duration is reduced to 2-4 turns.
  - Long Blades now give +SH instead of a chance to riposte.

#### New Items
  - The 'Ring of Insulation' gives rElec.
  - Iron Dragons may drop 'Iron Dragon Scales' on death.
  - The unrandarts the scimitar of Flaming Death and the dagger of Chilly Death have returned.
  - The 'Fists of Thunder' unrandart gives electric-branded unarmed attacks.
  - Scarf of Stasis

#### Changed Items
  - Missiles are always lost when fired, and have a proportionately higher spawn rate.
  - Morningstars and Eveningstars are 2-handed for small species.
  - Potions of mutation are more common.
  - Centaur and Naga bardings have been merged.
  - Staves of Earth have a +3 AC bonus.
  - v1.1: Players can use a Wand of Polymorph on themselves repeatedly.
  - v1.3.4: Some thrown weapon types are consolidated.

#### Removed Items
  - Amulet of Harm
  - Needles of Sleeping
  - Scrolls of Random Uselessness
  - v1.3.2: Crystal Ball of Energy

#### New Species
  - Fairy has great magic aptitudes, -1 MP costs on all spells, bonus defenses, and can't wear armour.
  - Sand Dwarf is small, slow, and has no spellcasting penalty in armour.
  - Oni can't learn spells from books, instead gaining random spells every 2 XL. All Oni spells only use the Spellcasting skill.
  - Duskwalker gains the Nightstalker mutation, has draining melee attacks, and has half the stealth penalty from armour.
  - Halflings have been merged into Kobolds.
  - Deep Elves and Tengu have been merged into Avariel, winged elves with good aptitudes for magic.

#### Species Changes
  - Humans have +1 to most aptitudes.
  - Mummies have Clarity, so they can't be confused.
  - Formicids can wield giant clubs, but can't use a shield with them.
  - Ogres have a +2 Maces & Flails aptitude.
  - Vine Stalkers start with Regeneration 1.
  - Demigod's XP modifier is -1.
  - Felids and Octopodes can wear scarves.
  - Hill Orcs use Fighting as their skill for all weapons.
  - Trolls have natural AC but can't wear body armour, and are slightly better at magic but worse with shields.

#### New Spells
  - Summon Scorpions summons one group of 1-4 scorpions.
  - Shackle prevents enemy movement.
  - False Image is a summon that blocks while you shoot through it.
  - Blink Other blinks enemies.
  - Piercing Shot adds the penetration brand to a ranged weapon.

#### Spell Changes
  - At high spell power, transformations now last forever.
  - Inner Flame doesn't check magic resistance.
  - Corona has a higher accuracy bonus.
  - Yara's Violent Unravelling is level 4.
  - Summon Lightning Spire is level 5.
  - Poisonous Vapours lasts longer at high power.
  - Poison Arrow is more common and less resistible.
  - Bolt of Cold and Throw Frost have +1 range.
  - Lightning Bolt has better accuracy.
  - v1.1: Ozocubu's Armour now works with any armour and freezes melee attackers, but only lasts a few turns and gives a smaller AC bonus.
  - v1.3.1: Lee's Rapid Deconstruction can break rock and stone walls, but not metal walls.
  - v1.3.3: Borgnjor's Vile Clutch is level 6.
  - v1.3.3: Haunt, Chain Lightning, and Fire Storm are more common.
  - v1.3.3: Glaciate is a level 8 Ice/Air/Conjurations spell.
  - v1.3.4: Statue Form does slightly less damage when unarmed.

#### Removed Spells
  - Summon Guardian Golem
  - Gell's Gravitas
  - Excruciating Wounds
  - Death's Door

#### Gods
  - Ashenzari now curses items for free, but uncursing items loses a large amount of piety.
  - Qazlal's Upheaval has a much lower piety cost.
  - Makhleb's Major Destruction is now more powerful.
  - Lugonu appears in the Temple.
  - Yredelemnul allows Statue Form and worship from Gargoyles.
  - v1.1: Wu Jian is reworked: abilities require less piety, Serpent's Lash only costs drain (scaling down with piety), Whirlwind does full dmg but only pins with Serpent's Lash, and Heavenly Storm starts out weaker but costs much less piety.
  - v1.2: Fedhas is reworked: piety is gained by killing natural and undead monsters, and abilities cost piety instead of rations.
  - v1.3: Sif Muna has faster piety gain.
  - v1.3.2: Nemelex decks now exist in an ability menu instead of your inventory. (based on ebering's work)
  - v1.3.4: TSO's Cleansing Flame has 1/3 less damage scaling with invocations.

#### Normal Monsters
  - The monster spell 'Twisted Resurrection' has been removed. Deep Elf Death mages now get the spell 'Haunt'.
  - Hornets don't Paralyse anymore.
  - Naga Mages and Nagarajas have the 'Dimensional Anchor' spell that stops the player from Teleporting.
  - Death Drakes may spawn in Swamp.
  - The new 'Orb Demon' enemy appears in Zot.
  - New monster spell: Freezing Blast, a cold equivalent to fireball that Frost Giants and Wizards can cast.
  - v1.3.2: Neqoxecs and Shining Eyes have Corrupting Pulse instead of Malmutate.

#### Unique Monsters
  - Ijyb always has a Polymorph wand, and gets a sling and branded dagger.
  - Xtahua's breath now applies rF-.
  - Xtahua now drops the +8 fire dragon scales of Xtahua {rF+++ rC- MR+} when he drops a corpse.
  - v1.3.1: Harold has Bolt of Fire, Blink, a branded trident, and a shield.
  - v1.3.2: Boris has Haunt instead of Iron Shot, and casts spells more.
