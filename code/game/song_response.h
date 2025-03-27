#pragma once
#include "piano_key_definitions.h"


// This is the "Charge!" cadence
#define SONG_RESPONSE_LENGTH 13
static const uint8_t song_response[SONG_LOADING_LENGTH]{
    // Length is 2X number of notes + 1

    // Preamble

    120, // Beats per minute

    // Insert pairs of key number and duration

    // First value in pair is the piano key (use the piano_key_definitions #defines)
    // Second value in pair is the number beats in terms of 1/32 notes
    // e.g. 1/32 note = 1, 1/8 note = 4, 1/4 note = 8
    // Reccomend adding // to separate notes and using // MEASURE markings to stay organized!

    // MEASURE 1
    KEY_G_4,
    3,
    //
    KEY_C_5,
    2,
    //
    KEY_E_5,
    3,
    //
    KEY_G_5,
    6,
    //
    KEY_E_5,
    2,
    //
    KEY_G_5,
    16

    // END OF SONG
};


/*
// This is a simple tone
#define SONG_RESPONSE_LENGTH 3
static const uint8_t song_response[SONG_RESPONSE_LENGTH]{
    // Length is 2X number of notes + 1

    // Preamble

    60, // Beats per minute

    // Insert pairs of key number and duration

    // First value in pair is the piano key (use the piano_key_definitions #defines)
    // Second value in pair is the number beats in terms of 1/32 notes
    // e.g. 1/32 note = 1, 1/8 note = 4, 1/4 note = 8
    // Reccomend adding // to separate notes and using // MEASURE markings to stay organized!

    // MEASURE 1
    KEY_A_4,
    32,
    // END OF SONG
};
*/

/*
// This is a triplet beep
#define SONG_RESPONSE_LENGTH 13
static const uint8_t song_response[SONG_RESPONSE_LENGTH]{
    // Length is 2X number of notes + 1

    // Preamble

    60, // Beats per minute

    // Insert pairs of key number and duration

    // First value in pair is the piano key (use the piano_key_definitions #defines)
    // Second value in pair is the number beats in terms of 1/32 notes
    // e.g. 1/32 note = 1, 1/8 note = 4, 1/4 note = 8
    // Reccomend adding // to separate notes and using // MEASURE markings to stay organized!

    // MEASURE 1
    KEY_A_4,
    10,
    //
    KEY_REST,
    1,
    //
    KEY_A_4,
    10,
    //
    KEY_REST,
    1,
    //
    KEY_A_4,
    10,
    //
    KEY_REST,
    1,
    //
    // END OF SONG
};
*/

/*
// This is something made up by Canyon
#define SONG_RESPONSE_LENGTH (2*12+1)
static const uint8_t song_response[SONG_LOADING_LENGTH]{
    // Length is 2X number of notes + 1

    // Preamble

    120, // Beats per minute

    // Insert pairs of key number and duration

    // First value in pair is the piano key (use the piano_key_definitions #defines)
    // Second value in pair is the number beats in terms of 1/32 notes
    // e.g. 1/32 note = 1, 1/8 note = 4, 1/4 note = 8
    // Reccomend adding // to separate notes and using // MEASURE markings to stay organized!

    // MEASURE 1
    KEY_B_5,
    15,
    //
    KEY_REST,
    1,
    //
    KEY_A_6,
    15,
    //
    KEY_REST,
    1,
    //
    KEY_B_6,
    15,
    //
    KEY_REST,
    1,
    //
    KEY_C_6,
    15,
    //
    KEY_REST,
    1,
    //
    KEY_B_6,
    15,
    //
    KEY_REST,
    1,
    //
    KEY_B_6,
    15
    //
    KEY_REST,
    1,

    // END OF SONG
};
*/