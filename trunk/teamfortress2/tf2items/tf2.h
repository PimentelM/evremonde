#ifndef TF2_H_
#define TF2_H_

#define TF2_BACKPACK_MAX_SIZE 100

#define TF2_METAL_MULTIPLIER 3

#define TF2_CRAFT_RANDOM_HAT_ITEM_REQUIREMENT         81
#define TF2_CRAFT_CLASS_SPECIFIC_HAT_ITEM_REQUIREMENT 108

#define TF2_INVENTORY_EQUIPPED 0x0FFF0000
#define TF2_INVENTORY_POSITION 0x0000FFFF

/*
    .----------------- Engineer
    | .--------------- Spy
    | | .------------- Pyro
    | | | .----------- Heavy
    | | | | .--------- Medic
    | | | | | .------- Demoman
    | | | | | | .----- Soldier
    | | | | | | | . -- Sniper
    | | | | | | | | .- Scout
    | | | | | | | | |
    0 0 0 0 0 0 0 0 0
*/

#define TF2_EQUIPPED_CLASS_ENGINEER 0x01000000
#define TF2_EQUIPPED_CLASS_SPY      0x00800000
#define TF2_EQUIPPED_CLASS_PYRO     0x00400000
#define TF2_EQUIPPED_CLASS_HEAVY    0x00200000
#define TF2_EQUIPPED_CLASS_MEDIC    0x00100000
#define TF2_EQUIPPED_CLASS_DEMOMAN  0x00080000
#define TF2_EQUIPPED_CLASS_SOLDIER  0x00040000
#define TF2_EQUIPPED_CLASS_SNIPER   0x00020000
#define TF2_EQUIPPED_CLASS_SCOUT    0x00010000

#endif // TF2_H_
