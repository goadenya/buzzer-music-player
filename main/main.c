#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "buzzer.h"
#include "melody_player.h"

#define NOTE_E5 659
#define NOTE_C5 523
#define NOTE_G5 784
#define NOTE_G4 392
#define REST 0

void app_main(void)
{
    // --- Define the Super Mario melody ---
    const int mario_notes[] = {
        NOTE_E5, NOTE_E5, REST, NOTE_E5,
        REST, NOTE_C5, NOTE_E5, REST,
        NOTE_G5, REST, REST, REST,
        NOTE_G4, REST, REST, REST};

    const int mario_durations[] = {
        4, 4, 4, 4,
        4, 4, 4, 4,
        2, 4, 4, 4,
        2, 4, 4, 4};

    melody_t mario_theme = {
        .notes = mario_notes,
        .durations = mario_durations,
        .length = sizeof(mario_notes) / sizeof(int),
        .beat_ms = 100};

    buzzer_t buzzer;
    buzzer_init(&buzzer, LEDC_TIMER_0, LEDC_CHANNEL_0, GPIO_NUM_18);

    start_melody_player_task(&mario_theme, &buzzer);
}