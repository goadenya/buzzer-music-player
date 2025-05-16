#include <stdio.h>
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "button.h"

#define START_BTN_PIN GPIO_NUM_12
#define NEXT_BTN_PIN GPIO_NUM_14

#define BUZZER_PIN GPIO_NUM_18
#define BUZZER_CHANNEL LEDC_CHANNEL_0
#define BUZZER_TIMER LEDC_TIMER_0

volatile bool g_is_playing = false;
volatile int g_melody_index = 0;

button_t play_btn, next_btn;

void button_task(void *pvParameters)
{
    button_init(&play_btn, START_BTN_PIN);
    button_init(&next_btn, NEXT_BTN_PIN);

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

            ledc_set_freq(LEDC_LOW_SPEED_MODE, BUZZER_TIMER, note);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, BUZZER_CHANNEL, 512);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, BUZZER_CHANNEL);

            note_index = (note_index + 1) % melody_lengths[g_melody_index];
        }
        else
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, BUZZER_CHANNEL, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, BUZZER_CHANNEL);
            note_index = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// void setupButtons()
// {
//     gpio_config_t io_cfig = {
//         .pin_bit_mask = (1ULL << START_BTN_PIN) | (1ULL << NEXT_BTN_PIN),
//         .mode = GPIO_MODE_INPUT,
//         .pull_up_en = GPIO_PULLUP_ENABLE,
//         .pull_down_en = GPIO_PULLDOWN_DISABLE,
//         .intr_type = GPIO_INTR_DISABLE};
//     gpio_config(&io_cfig);
// }

void setupBuzzer()
{

    ledc_timer_config_t buzzer_ledc_timer_cfig = {
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = 1000, // Will be updated dynamically
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = BUZZER_TIMER,
        .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&buzzer_ledc_timer_cfig);

    ledc_channel_config_t buzzer_ledc_channel_cfig = {
        .channel = BUZZER_CHANNEL,
        .duty = 0,
        .gpio_num = BUZZER_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint = 0,
        .timer_sel = BUZZER_TIMER};
    ledc_channel_config(&buzzer_ledc_channel_cfig);
}

void app_main(void)
{
    // setupButtons();
    setupBuzzer();

    xTaskCreate(button_task, "button_task", 4096, NULL, 5, NULL);
    xTaskCreate(melody_task, "melody_task", 2048, NULL, 5, NULL);
}