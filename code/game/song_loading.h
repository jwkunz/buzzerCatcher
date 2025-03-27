#pragma once
#include "piano_key_definitions.h"

// This is the "La Cucaracha"
#define SONG_LOADING_LENGTH (2 * 27 + 1)
static const uint8_t song_loading[SONG_LOADING_LENGTH]{
    // Length is 2X number of notes + 1

    // Preamble

    200, // Beats per minute

    // Insert pairs of key number and duration

    // First value in pair is the piano key (use the piano_key_definitions #defines)
    // Second value in pair is the number beats in terms of 1/32 notes
    // e.g. 1/32 note = 1, 1/8 note = 4, 1/4 note = 8
    // Reccomend adding // to separate notes and using // MEASURE markings to stay organized!

    // MEASURE 1
    KEY_D_4,
    2,
    //
    KEY_REST,
    1,
    //
    KEY_D_4,
    2,
    //
    KEY_REST,
    1,
    //
    KEY_D_4,
    2,
    // MEASURE 2
    KEY_G_4,
    11,
    //
    KEY_REST,
    1,
    //
    KEY_B_5,
    4,
    //
    KEY_REST,
    4,
    KEY_D_4,
    2,
    //
    KEY_REST,
    1,
    //
    KEY_D_4,
    2,
    //
    KEY_REST,
    1,
    //
    KEY_D_4,
    2,
    // MEASURE 3
    KEY_G_4,
    11,
    //
    KEY_REST,
    1,
    //
    KEY_B_5,
    4,
    //
    KEY_REST,
    16,
    // MEASURE 4
    KEY_REST,
    4,
    //
    KEY_G_4,
    7,
    //
    KEY_REST,
    1,
    //
    KEY_G_4,
    4,
    //
    KEY_Gb_4,
    4,
    //
    KEY_Gb_4,
    4,
    //
    KEY_E_4,
    4,
    //
    KEY_E_4,
    4,
    //
    KEY_D_4,
    16
    // END OF SONG
};

/*

// This is "Star Wars"
#define SONG_LOADING_LENGTH (2 * 27 + 1)
static const uint8_t song_loading[SONG_LOADING_LENGTH]{
    // Length is 2X number of notes + 1

    // Preamble

    120, // Beats per minute

    // Insert pairs of key number and duration

    // First value in pair is the piano key (use the piano_key_definitions #defines)
    // Second value in pair is the number beats in terms of 1/32 notes
    // e.g. 1/32 note = 1, 1/8 note = 4, 1/4 note = 8
    // Reccomend adding // to separate notes and using // MEASURE markings to stay organized!

    //
    KEY_D_4,
    2,
    //
    KEY_REST,
    1,
    //
    KEY_D_4,
    2,
    //
    KEY_REST,
    1,
    //
    KEY_D_4,
    2,
    // 
    KEY_G_4,
    16,
    //
    KEY_D_5,
    16,
    //
    KEY_C_5,
    2,
    //
    KEY_REST,
    1,
    //
    KEY_B_6,
    2,
    //
    KEY_REST,
    1,
    //
    KEY_A_6,
    1,
    //
    KEY_G_5,
    16,
    //
    KEY_D_5,
    8,
    //
    KEY_C_5,
    2,
    //
    KEY_REST,
    1,
    //
    KEY_D_5,
    2,
    //
    KEY_REST,
    1,
    //
    KEY_A_6,
    2,
    //
    KEY_G_5,
    16,
    //
    KEY_D_5,
    8,
    //
    KEY_C_5,
    2,
    //
    KEY_REST,
    1,
    //
    KEY_B_6,
    2,
    //
    KEY_REST,
    1,
    //
    KEY_C_5,
    2,
    //
    KEY_A_4,
    16
        // END OF SONG
};

*/