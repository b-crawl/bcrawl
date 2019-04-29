# bcrawl

X-Crawl was a notable fork of Dungeon Crawl Stone Soup; bcrawl is a continuation of it.

Like X-Crawl, discussion is done on the [CXC Discord](https://discord.gg/SzXGZNS).

David Ploog (a former DCSS developer) [wrote](https://www.rockpapershotgun.com/2017/03/23/making-dungeon-crawl-stone-soup-with-253-cooks-and-no-head-chef/): 
> In NetHack they have this model where if somebody [on the team] objects to something, it will not happen. So every person can veto every change. And in the end you could see that NetHack was dead, much less from having no releases – even when they had a few releases the changes that would appear were miniscule. Minor and minor. And they did improve, but they wouldn’t do anything radical. I was very much afraid that in Crawl, something like this could happen.
> 
> I try very hard to always shift in the direction of more changes. Because if you try something and it doesn’t work – and this happens from time to time – you can revert. But if you don’t even try, you are stuck. We have to try radical things.

bcrawl exists because I believe DCSS has become stagnant, generally [unwilling](https://www.reddit.com/r/dcss/comments/9aseh1/has_dcss_development_stagnated/e4yty2n/) to try new content.

bcrawl is meant to be for all players, not just long-time players who adopted certain canonized strategies. Neither will bcrawl be [changed](https://www.reddit.com/r/dcss/comments/8poib2/make_lees_rapid_deconstruction_never_break_walls/) to prevent strategies *just* because they violate developer preconceptions about how to play. Instead, the guiding principle of bcrawl is that fun and fair things are good, and unfun and unfair things are bad.

This is why bcrawl exists. I hope that it can meet those goals; for now, I (bhauth) think it is.

Pull requests for new content are welcome.

## How to Play
#### offline
- [Windows builds](https://github.com/b-crawl/bcrawl/releases)

#### online
  - [CKO](https://crawl.kelbi.org/) (New York)
  - [CPO](https://crawl.project357.org/) (Australia)
  - [x-crawl.de](https://x-crawl.de/) (Germany)

## Changes from Vanilla Crawl
#### Branches
  - Dungeon is shortened to 12 floors.
  - Vaults does not require any runes to enter.
  - Vaults is shortened to 3 floors.
  - Lair Branches (Swamp, Spider, Snake, Shoals) are shortened to 3 floors.
  - Crypt is shortened to 2 floors.
  - Orc is shortened to 1 floor.
  - v1.5.3: Orc doesn't spawn Stone Giants except from vaults.
  - v1.6: To compensate for fewer floors, shops are more likely.
  - v1.6.1: Vaults has fewer orcs.
  - v1.7.3: Out-of-depth monster spawns are changed: centaurs and water moccasins can't appear until D:3, and killer bees can't appear until D:4.
  - v1.9: Hell branches are 2 levels deep and can't be left without their rune.
  - v1.11.5: Orb Guardians can spawn on any level of Zot.

#### Backgrounds
  - Venom Mage, Arcane Marksman, and Warper are completely reworked. (based on Doesnty's work)
  - Reaver is a melee background that starts the game worshipping Vehumet.
  - Archaeologists start with a crate and a dusty tome. The tome reveals itself to be a random skill manual at XL 3, and when the manual is finished, the crate unlocks and gives the player a related artifact.
  - Necromancers start with Bolt of Draining.
  - v1.1: Skalds have a reworked Ozocubu's Armour instead of Shroud of Golubria.
  - v1.5.2: Assassin and Artificer have been merged into Rogue.
  - v1.9.3: Monks start with an amulet (usually guardian spirit) and a hat.
  - v1.11.3: Sloth Apostles worship Cheibriados and start with a shield.
  - v1.11.3: Transmuters start with a potion of lignification.
  - v1.11.3: Skalds start with a scroll of fear.
  - v1.11.6: Conjurers have Irradiate instead of Dazzling Spray.
  - v1.11.7: Enchanters start with a scroll of fog.
  - v1.11.7: Wanderers start with significantly more spells on average.
  - v1.11.8: Monks start with 2 Invocations skill.

#### Features
  - All items are automatically identified if the player has a rune.
  - v1.1.1: Like in mainline DCSS, all traps are revealed, but there are no trap effects from exploration.
  - v1.3: To replace traps, when taking stairs there's a 4% chance of being teleported onto the lower floor.
  - v1.7: An "Adventure" mode is added, which is a normal game that starts the player with 2 extra lives.
  - v1.9.3: When in Zot or carrying the Orb, teleports from stairs and traps are replaced by blinks.
  - v1.11: Seeded runs are merged from DCSS.

#### Balance Changes
  - The Paralyse spell duration is reduced to 2-4 turns.
  - Plain unidentified weapons are always +0.
  - Long Blades now give +SH instead of a chance to riposte.
  - v1.4.1: Distortion weapons can't banish on hit.
  - v1.8.6: Starting skills are based on +0 aptitude (-1 for Spellcasting) instead of Human aptitudes.

#### New Species
  - Fairy has great magic aptitudes, -1 MP costs on all spells, bonus defenses, and can't wear armour.
  - Sand Dwarf is small, slow, and has no spellcasting penalty in armour.
  - Oni can't learn spells from books, instead gaining random spells every 2 XL. All Oni spells only use the Spellcasting skill.
  - Duskwalker gains the Nightstalker mutation, has draining melee attacks, and has half the stealth penalty from armour.
  - Halflings have been merged into Kobolds.
  - Deep Elves and Tengu have been merged into Avariel, winged elves with good aptitudes for magic.
  - v1.9: Djinni can cast spells without mana, but doing so prevents spellcasting for 4*cost^2/experience_level turns.

#### Species Changes
  - Hill Orcs use Fighting as their skill for all weapons.
  - Humans have +1 to most aptitudes.
  - Mummies have Clarity, so they can't be confused.
  - Formicids can wield giant clubs, but can't use a shield with them.
  - Ogres have a +2 Maces & Flails aptitude.
  - Vine Stalkers start with Regeneration 1.
  - Demigod's XP modifier is -1.
  - Felids and Octopodes can wear scarves.
  - v1.4: [Trolls](https://github.com/b-crawl/bcrawl/wiki/Troll) have natural AC but can't wear body armour, and are slightly better at magic but worse with shields. They gain Regen 1 / Regen 2 / rC+ / rF+ at level 4/12/16/20.
  - v1.4: Deep Dwarves lose their Heal Wounds ability and start the game with Elyvilon if their background doesn't start with a god. They also don't gain HP from vampiric effects.
  - v1.5: Hill Orcs have a +1 Conjurations aptitude.
  - v1.5.2: Ghouls have adjusted magic aptitudes, including +4 Necromancy.
  - v1.6: Vampires are reworked: above Satiated gives Bat Form and regeneration, and below Satiated gives resistances and stealth. Bloodless Vampires still regenerate at half normal speed.
  - v1.8: Demigods have various divine abilities.
  - v1.10: Felids gain half XP for seeing enemies, and no experience for killing them. They have -30% HP instead of -40%, and +0 XP instead of -1 XP.
  - v1.11.5: Spriggans can't train Conjurations, but have some better aptitudes.
  - v1.11.8: Barachim hop target radius is reduced to 1.

#### New Spells
  - Summon Scorpions summons one group of 1-4 scorpions.
  - Shackle prevents enemy movement.
  - False Image is a summon that blocks while you shoot through it.
  - Blink Other blinks enemies.
  - Piercing Shot adds the penetration brand to a ranged weapon.
  - v1.8.3: Tree Form (L5 Hexes/Transmutation) acts like a Potion of Lignification with a shorter duration.

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
  - v1.4: Statue Form does slightly less damage when unarmed.
  - v1.9.1: Infusion scales much more with spell power.
  - v1.9.1: Song of Slaying starts at +1.
  - v1.9.1: Force Lance is pure translocations, and removed from the Conjurer book.
  - v1.9.2: Spectral Weapon lasts longer at high spell power.
  - v1.9.2: Deflect Missiles is never dispelled by attacks, but can drain mana when it deflects missiles. It drains less mana at higher power.
  - v1.11.3: Slow is level 1.
  - v1.11.4: Ozocubu's Refrigeration now has the same damage formula as Fireball. (Its damage calculation was wrong for several months.)
  - v1.11.6: Iskenderun's Mystic Blast is level 3, and has lower base damage but higher damage scaling.
  - v1.11.7: Irradiate has the same damage formula as Bolt of Fire, which has lower base damage but higher damage scaling.
  - v1.11.8: Lesser Beckoning has +1 range.

#### Removed Spells
  - Summon Guardian Golem
  - Gell's Gravitas
  - Excruciating Wounds
  - Death's Door
  - v1.1: Shroud of Golubria
  - v1.8.3: Hydra Form
  - v1.9.1: Confusing Touch

#### Gods
  - Ashenzari now curses items for free, but uncursing items loses a large amount of piety.
  - Qazlal's Upheaval has a much lower piety cost.
  - Makhleb's Major Destruction is now more powerful.
  - Lugonu appears in the Temple.
  - Yredelemnul allows Statue Form and worship from Gargoyles.
  - v1.1: Wu Jian is reworked: abilities require less piety, Serpent's Lash only costs drain (scaling down with piety), Whirlwind does full dmg but only pins with Serpent's Lash, and Heavenly Storm starts out weaker but costs much less piety.
  - v1.2: Fedhas is reworked: piety is gained by killing natural and undead monsters, and abilities cost piety instead of rations.
  - v1.3: Sif Muna has faster piety gain.
  - v1.3.2: Nemelex decks now exist in an ability menu instead of your inventory. (based on ebering's work) Decks have a max size of 20, Triple Draw doesn't cost piety, and Deal Four doesn't destroy decks.
  - v1.4: TSO's Cleansing Flame has 1/3 less damage scaling with invocations.
  - v1.4: Elyvilon's self-healing abilities are better.
  - v1.5: Dithmenos is [reworked](https://github.com/b-crawl/bcrawl/wiki/Dithmenos): Actions aren't mimicked at high piety, but Shadow Step is much cheaper and makes a short-lived shadow clone of the player.
  - v1.6.1: Sif Muna gives Divine Energy at 0* and book gifts starting at 1*.
  - v1.6.1: Okawaru and Trog no longer give needles as gifts.
  - v1.10.1: Nemelex's Degeneration card reduces enemy HP at high power.
  - v1.11.5: Kikubaaqudgha prevents miscasts from any spell including Necromancy, not just from pure Necromancy spells, and gives full protection at 3* piety.
  - v1.11.8: Uskayaw's Line Pass ability no longer costs piety.
  - v1.11.8: Zin's Vitalisation gives a much bigger attribute bonus, but costs more piety.
  - v1.11.8: Lugonu's self-banishment costs much less piety. (About half as much.)

#### New Items
  - The 'Ring of Insulation' gives rElec.
  - Iron Dragons may drop 'Iron Dragon Scales' on death.
  - Scarf of Stasis

#### Changed Items
  - Missiles are always lost when fired, and have a proportionately higher spawn rate.
  - Morningstars and Eveningstars are 2-handed for small species.
  - Potions of mutation are more common.
  - Centaur and Naga bardings have been merged.
  - Staves of Earth have a +3 AC bonus.
  - v1.1: Players can use a Wand of Polymorph on themselves repeatedly.
  - v1.4: Some thrown weapon types are consolidated.
  - v1.5.3: Wands of random effects have fewer charges, and a new spell set: Inner Flame, Malmutate, Sleep, Slow, Shackle, Petrify, Teleport Other, Iceblast, Fireball, Bolt of Draining, Venom Bolt.
  - v1.7.1: Wands of digging are partly un-nerfed, getting 8 average charges.
  - v1.8.3: Removing an amulet of faith drains 1/4 your piety instead of 1/3.
  - v1.8.5: Changed base damage for weapons: bardiche (18->20), glaive (15->16), triple sword (17->18).
  - v1.11.5: Shadow dragon armour has (10->11) base AC, and storm dragon armour has (15->13) encumbrance.
  - v1.11.8: Lightning rod damage scales more with Evocations.
  - v1.11.8: Randart crossbows of penetration are 2x as likely.
  - v1.11.8: Changed base delay for weapons: falchion (13->12), long sword (14->13).

#### Removed Items
  - Amulet of Harm
  - Needles of Sleeping
  - Scrolls of Random Uselessness
  - v1.3.2: Crystal Ball of Energy
  - v1.4.2: Scrolls of Holy Word

#### Artefacts
  - The 'Fists of Thunder' unrandart gives electric-branded unarmed attacks.
  - The scimitar of Flaming Death and the dagger of Chilly Death have returned.
  - v1.5.2: The skin of Zhor gains +4 slaying.
  - v1.6: The dagger of Chilly Death is stronger, can slow enemies, and is renamed to the sapphire dagger.
  - v1.7: The gauntlets of War are +0 with +6 slaying.
  - v1.7: The salamander hide armour is +8, rF++ rC-, and can be evoked to make fog.
  - v1.7.1: Morg has +12 int, but curses itself.
  - v1.8.5: Kryia's mail coat has Repel Missiles.
  - v1.11: The serpentine sling can petrify enemies it hits.
  - v1.11.2: Scorpio is a hand crossbow that does high damage and pulls enemies towards you.
  - v1.11.2: The butterfly sword attacks quickly and summons neutral butterflies.
  - v1.11.8: Bloodbane is reworked, doing high damage but causing teleports.

#### Normal Monsters
  - The monster spell 'Twisted Resurrection' has been removed. Deep Elf Death mages now get the spell 'Haunt'.
  - Hornets don't Paralyse anymore.
  - Naga Mages have the 'Dimensional Anchor' spell that stops the player from Teleporting.
  - Death Drakes may spawn in Swamp.
  - The new 'Orb Demon' enemy appears in Zot.
  - New monster spell: Freezing Blast, a cold equivalent to fireball that Frost Giants and Wizards can cast.
  - v1.3.2: Neqoxecs and Shining Eyes have Corrupting Pulse instead of Malmutate.
  - v1.4.1: Gnolls no longer sometimes spawn with nets.
  - v1.5: Nagaraja get a new spell set that includes Fireball.
  - v1.5: Orc Priests cast more often, but take longer to cast.
  - v1.5: "Orc Wizard II" gets a Cantrip spell.
  - v1.5.2: Sphinxes have Sentinel's Mark instead of Confuse.
  - v1.5.3: Skeletons now resist electricity.
  - v1.6.1: Vault Guards can now use Battlecry when injured.
  - v1.8.4: Killer Klowns carry branded weapons, and have a new spell set: Blink, Monstrous Menagerie, Corrupting Pulse, Dig, Silence.
  - v1.11.5: Orb Guardians get a magical gaze, with Lesser Beckoning, Sleep, and Lightning Bolt.
  - v1.11.8: Reapers can seal doors like Vault Wardens do.
  - v1.11.8: Juggernauts are tougher, hit faster, and knock their foes back.

#### Unique Monsters
  - Ijyb always has a Polymorph wand, and gets a sling and branded dagger.
  - Xtahua can drop unrand fire dragon scales, and Xtahua's breath applies rF-.
  - v1.3.1: Harold has Bolt of Fire, Blink, a branded trident, and a shield.
  - v1.3.2: Boris has Haunt instead of Iron Shot, and casts spells more.
  - v1.4.1: Psyche is reworked to be as dangerous as Erica, with a new spell set including Deflect Missiles, Sleep, and Poison Arrow. (Read the lore.)
  - v1.5.3: Margery has +4 AC and +4 EV.
  - v1.6.1: Natasha no longer appears on D:2.
  - v1.6.1: Gastronok casts faster, and has summoning instead of Airstrike.
  - v1.7.2: Fannar is more dangerous, getting Freezing Cloud.
  - v1.7.2: Psyche, Fannar, and Sojobo sometimes drop a spellbook.
  - v1.11.7: Bai Suzhen now summons drakes and then goes berserk, making her significantly more dangerous.
  - v1.11.8: Agnes is much harder to hit.
