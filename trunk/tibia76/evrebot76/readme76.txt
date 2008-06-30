evrebot for Tibia 7.6
by Evremonde
February 12th, 2006

Hotkeys:
Insert		- Sudden Death target
Home		- Shoot custom rune at target
Page Up		- Magic Wall in front of target
Delete		- Ultimate Healing self
End		- Light hack on/off
Page Down	- Manafluid self
Pause		- Reload ammo

Chat commands:
/status		- Show bot status

/hotkeys	- Hotkeys on/off
/h

/light		- Light hack on/off
/l

/click		- Click hack on/off
/c
/use
/u

/reveal		- Reveal names on/off
/names

/dance		- Dance on/off
/d

/experience	- Show experience needed to level up
/exp
/e

/random		- Random outfit on/off
/rand
/r

/rainbow	- Rainbow outfit on/off
/rbow
/r2

/outfit demon	- Set outfit to demon (evrebot.xml)

/ammo		- Reload ammo

/manafluid	- Drink mana fluid
/mf

/spam		- Auto-spam @@@@@@@@@@'s on/off

/loot		- Auto-loot on/off (evrebot.xml)
/stack		- Auto-stack on/off (evrebot.xml)
/eat		- Auto-eat on/off (evrebot.xml)

/uh		- Auto-UH on/off
/uh 75		- Auto-UH at 75% health

/heal		- Auto-heal on/off
/heal 50	- Auto-heal at 50% health
*Note:
Auto-heal works with auto-spell and
uses the exura line of spells!

/shoot		- Always shoot target on/off (*Note: Shoots the custom rune)
/s

/target		- Keep target on/off (*Note: "Target lost.")
/t

/spell			- Auto-spell casting on/off
/spell 30 exevo pan	- Add spell to spells list
/spell 40 exevo pan	- Update mana cost for spell
/spell remove exevo pan - Remove spell from spells list
/spell show		- Show spells list
/spell clear		- Clear spells list
*Note:
The following spells will only cast if needed:
utamo vita, utani hur, utani gran hur, exana pox, exura, exura gran, exura vita

/id		- Show the ID of the last thing you looked at
/i

/set uh		- Set the UH rune to be the last rune you looked at
/set sd		- Set the SD rune to be the last rune you looked at
/set default	- Set the UH and SD runes back to default
/set custom	- Set the custom rune to be the last rune you looked at

/custom		- Shoot the custom rune at target

/magicwall	- Set the custom rune
/mwall
/sd
/explosion
/explo
/ex
/hmm
/gfb
/fbomb
/fb
/ebomb
/eb
/pbomb
/pb
/fwall
/fw
/ewall
/ew
/pwall
/pw
/soulfire
/sfire
/sf
/envenom

evrebot.xml:
Define an item->	item:3031:money_gold
Define an item->	item:3588:food_blueberry
Loot an item->		loot:money_gold
Stack an item->		stack:money_gold
Eat an item->		eat:food_blueberry
Define an outfit->	outfit:35:demon
*Note:
Look at something in-game and use /id to find out it's number!

Multi-client Support:
Mostly used on otserv pvp.
Use multi-client, log on the first character, open the bot,
then log on the second character, and then open the bot again.
Repeat for each character.
Using the hotkeys will work across all clients at the same time.
SD Combo->
Open your backpack of SDs on all characters,
target your enemy with all characters,
and use the SD hotkey.
Bot must be loaded for each character!