## design philosophy

#### experimentation is necessary

David Ploog (a former DCSS developer) [wrote](https://www.rockpapershotgun.com/2017/03/23/making-dungeon-crawl-stone-soup-with-253-cooks-and-no-head-chef/): 
> In NetHack they have this model where if somebody [on the team] objects to something, it will not happen. So every person can veto every change. And in the end you could see that NetHack was dead, much less from having no releases – even when they had a few releases the changes that would appear were miniscule. Minor and minor. And they did improve, but they wouldn’t do anything radical. I was very much afraid that in Crawl, something like this could happen.
> 
> I try very hard to always shift in the direction of more changes. Because if you try something and it doesn’t work – and this happens from time to time – you can revert. But if you don’t even try, you are stuck. We have to try radical things.

When I started bcrawl, I felt that DCSS had become stagnant, generally [unwilling](https://www.reddit.com/r/dcss/comments/9aseh1/has_dcss_development_stagnated/e4yty2n/) to try adding new content.

I agree with David Ploog's position, but I also believe that it's important to have:
- discussion of and collaboration on design of gameplay changes
- adequate testing before gameplay changes are made
- willingness to revert gameplay changes that don't improve the game

#### more content is good

Adding more items, spells, and abilities is good because it:
- increases replayability
- increases variety within individual games
- increases the strategic depth (and "skill cap") by increasing the action space
- lets players find a style they personally like

So, bcrawl will not be [changed](https://www.reddit.com/r/dcss/comments/8poib2/make_lees_rapid_deconstruction_never_break_walls/) to prevent strategies *just* because they violate developer preconceptions about how to play. Instead, the strength and entertainment values of tactics and strategies will be evaluated as they exist in practice.

Optional content such as species and usable items will generally only be removed if it is unfair, redundant, misleading, or useless. Content will not be removed just for being unnecessary to certain "canonical" strategies.

#### content exists for gameplay

The purpose of content is not to simply exist, but to enable gameplay. So, when feasible, balance should be adjusted to make interesting strategies viable. Highly situational unusual tactics should be encouraged, and if novel and entertaining but suboptimal strategies are found, it's better to strengthen them than to remove them as options.

#### choices should be distinct or clear

Things that are different should be distinct: differences should not be trivial. For example, a sword that does 1% extra damage against red enemies is not different enough from a regular sword to justify the mental overhead of its existence for players or programmers.

When something is strictly better than another thing, that should be obvious. (For example, a +0 sword vs a +1 sword.)

Advantages and weaknesses of things should not be both narrow and obvious. For example, a sword with bonus fire damage is better than a sword that does bonus damage only against skeletons because it's less narrow.

#### tedious strategies are bad but inevitable

Doing tedious actions to gain an advantage is boring. *Not* doing that and forgoing the advantage to save time feels bad to many players, because it's a failure to meet goals set forth in a game the player has decided to respect to some extent.

Also, a game cannot be balanced both for doing tedious actions with large benefits and for *not* doing them without punishing newer and weaker players with worse gameplay.

Still, it's impossible to completely eliminate repetitive or otherwise uninteresting parts of gameplay. So, the tedium-benefit product of player actions should be minimized.

#### diversification and specialization should be balanced

Optimal strategies should involve some specialization and some diversification.

#### design ideology should be tested

Changes should be evaluated based on their effect in practice, not based on what they "should" do according to some design ideology.

#### games have many goals

According to the philosophy bcrawl follows, strategic depth, minimizing tedium, variety, replayability, flavor, narrative, intuitiveness, and visuals all have some value in games, and should all be balanced against each other.

Some designers follow an ideology that ignores some of those, or values one infinitely more than the others, but I believe that leads to games that are incomplete and unbalanced.
