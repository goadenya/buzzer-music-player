#pragma once

#include "buzzer.h"

typedef struct
{
    const int *notes;
    const int *durations;
    int length;
    int beat_ms; // base duration of a "quarter note"
} melody_t;

// Starts a task that plays a melody through the buzzer
void start_melody_player_task(const melody_t *melody, buzzer_t *buzzer);
