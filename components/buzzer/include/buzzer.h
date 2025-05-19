#pragma once

#include "driver/ledc.h"

typedef struct
{
    ledc_timer_t buzzer_timer;
    ledc_channel_t buzzer_channel;
    gpio_num_t gpio;
} buzzer_t;

void buzzer_init(buzzer_t *buzzer, ledc_timer_t buzzer_timer, ledc_channel_t buzzer_channel, gpio_num_t gpio);
void play_note(buzzer_t *buzzer, int note);
void stop_buzzer(buzzer_t *buzzer);