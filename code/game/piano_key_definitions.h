#pragma once

// MIDI Note indexes in array (piano keys)
#define KEY_A_0 (9-9) // 0
#define KEY_Bb_0 (10-9)
#define KEY_B_0 (11-9)
//
#define KEY_C_1 (12+0-9)
#define KEY_Db_1 (12+1-9)
#define KEY_D_1 (12+2-9)
#define KEY_Eb_1 (12+3-9)
#define KEY_E_1 (12+4-9)
#define KEY_F_1 (12+5-9)
#define KEY_Gb_1 (12+6-9)
#define KEY_G_1 (12+7-9)
#define KEY_Ab_1 (12+8-9)
#define KEY_A_1 (12+9-9)
#define KEY_Bb_1 (12+10-9)
#define KEY_B_1 (12+11-9)
//
#define KEY_C_2 (24+0-9)
#define KEY_Db_2 (24+1-9)
#define KEY_D_2 (24+2-9)
#define KEY_Eb_2 (24+3-9)
#define KEY_E_2 (24+4-9)
#define KEY_F_2 (24+5-9)
#define KEY_Gb_2 (24+6-9)
#define KEY_G_2 (24+7-9)
#define KEY_Ab_2 (24+8-9)
#define KEY_A_2 (24+9-9)
#define KEY_Bb_2 (24+10-9)
#define KEY_B_2 (24+11-9)
//
#define KEY_C_3 (36+0-9)
#define KEY_Db_3 (36+1-9)
#define KEY_D_3 (36+2-9)
#define KEY_Eb_3 (36+3-9)
#define KEY_E_3 (36+4-9)
#define KEY_F_3 (36+5-9)
#define KEY_Gb_3 (36+6-9)
#define KEY_G_3 (36+7-9)
#define KEY_Ab_3 (36+8-9)
#define KEY_A_3 (36+9-9)
#define KEY_Bb_3 (36+10-9)
#define KEY_B_3 (36+11-9)
//
#define KEY_C_4 (48+0-9)
#define KEY_Db_4 (48+1-9)
#define KEY_D_4 (48+2-9)
#define KEY_Eb_4 (48+3-9)
#define KEY_E_4 (48+4-9)
#define KEY_F_4 (48+5-9)
#define KEY_Gb_4 (48+6-9)
#define KEY_G_4 (48+7-9)
#define KEY_Ab_4 (48+8-9)
#define KEY_A_4 (48+9-9)
#define KEY_Bb_4 (48+10-9)
#define KEY_B_4 (48+11-9)
//
#define KEY_C_5 (60+0-9)
#define KEY_Db_5 (60+1-9)
#define KEY_D_5 (60+2-9)
#define KEY_Eb_5 (60+3-9)
#define KEY_E_5 (60+4-9)
#define KEY_F_5 (60+5-9)
#define KEY_Gb_5 (60+6-9)
#define KEY_G_5 (60+7-9)
#define KEY_Ab_5 (60+8-9)
#define KEY_A_5 (60+9-9)
#define KEY_Bb_5 (60+10-9)
#define KEY_B_5 (60+11-9)
//
#define KEY_C_6 (72+0-9)
#define KEY_Db_6 (72+1-9)
#define KEY_D_6 (72+2-9)
#define KEY_Eb_6 (72+3-9)
#define KEY_E_6 (72+4-9)
#define KEY_F_6 (72+5-9)
#define KEY_Gb_6 (72+6-9)
#define KEY_G_6 (72+7-9)
#define KEY_Ab_6 (72+8-9)
#define KEY_A_6 (72+9-9)
#define KEY_Bb_6 (72+10-9)
#define KEY_B_6 (72+11-9)
//
#define KEY_C_7 (84+0-9)
#define KEY_Db_7 (84+1-9)
#define KEY_D_7 (84+2-9)
#define KEY_Eb_7 (84+3-9)
#define KEY_E_7 (84+4-9)
#define KEY_F_7 (84+5-9)
#define KEY_Gb_7 (84+6-9)
#define KEY_G_7 (84+7-9)
#define KEY_Ab_7 (84+8-9)
#define KEY_A_7 (84+9-9)
#define KEY_Bb_7 (84+10-9)
#define KEY_B_7 (84+11-9) 
//
#define KEY_C_8 (96+0-9) // 87
// Rest is a special value
#define KEY_REST 88

// This array contains a lookup for piano keys to note period in microseconds
static const uint16_t KEY_PERIOD_MICROS[88] = {
    36364, 34323, 32396, 30578,
    28862, 27242, 25713, 24270, 22907, 21622, 20409, 19263, 18182, 17161, 16198, 15289,
    14431, 13621, 12856, 12135, 11454, 10811, 10204, 9631, 9091, 8581, 8099, 7645,
    7215, 6811, 6428, 6067, 5727, 5405, 5102, 4816, 4545, 4290, 4050, 3822,
    3608, 3405, 3214, 3034, 2863, 2703, 2551, 2408, 2273, 2145, 2025, 1911,
    1804, 1703, 1607, 1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012, 956,
    902, 851, 804, 758, 716, 676, 638, 602, 568, 536, 506, 478,
    451, 426, 402, 379, 358, 338, 319, 301, 284, 268, 253, 239
};