#include <stdio.h>
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "button.h"
#include "buzzer.h"

#define START_BTN_PIN GPIO_NUM_12
#define NEXT_BTN_PIN GPIO_NUM_14

#define BUZZER_PIN GPIO_NUM_18
#define BUZZER_CHANNEL LEDC_CHANNEL_0
#define BUZZER_TIMER LEDC_TIMER_0

volatile bool g_is_playing = false;
volatile int g_melody_index = 0;

button_t play_btn, next_btn;
buzzer_t buzzer;

void button_task(void *pvParameters)
{
    while (1)
    {
        if (button_pressed_state(&play_btn) == true)
        {
            g_is_playing = !g_is_playing;

            printf(g_is_playing ? "Playing...\n" : "Paused.\n");
        }

        if (button_pressed_state(&next_btn) == true)
        {
            g_melody_index = (g_melody_index + 1) % 2;
            printf("Switched to melody #%d\n", g_melody_index + 1);
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void melody_task(void *pvParameters)
{
    const int melody1[] = {262, 294, 330, 349}; // C D E F
    const int melody2[] = {392, 440, 494, 523}; // G A B C

    const int *melodies[] = {melody1, melody2};
    const int melody_lengths[] = {4, 4}; // number of notes in each

    int note_index = 0;

    while (1)
    {
        if (g_is_playing)
        {
            const int *current_melody = melodies[g_melody_index];
            int note = current_melody[note_index];

            play_note(&buzzer, note);

            note_index = (note_index + 1) % melody_lengths[g_melody_index];
        }
        else
        {
            stop_buzzer(&buzzer);
            note_index = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    buzzer_init(
        &buzzer,
        BUZZER_TIMER,
        BUZZER_CHANNEL,
        BUZZER_PIN);

    button_init(&play_btn, START_BTN_PIN);
    button_init(&next_btn, NEXT_BTN_PIN);

    xTaskCreate(button_task, "button_task", 4096, NULL, 5, NULL);
    xTaskCreate(melody_task, "melody_task", 4096, NULL, 5, NULL);
}