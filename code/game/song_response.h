#include "piano_key_definitions.h"
static const uint8_t song_charge[13]{
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