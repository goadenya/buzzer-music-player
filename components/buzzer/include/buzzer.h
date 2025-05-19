#pragma once

#include "driver/ledc.h"

typedef struct
{
    ledc_timer_t buzzer_timer;
    ledc_channel_t buzzer_channel;
    int buzzer_gpio_pin;
} buzzer_t;

void buzzer_init(buzzer_t *buzzer, ledc_timer_t buzzer_timer, ledc_channel_t buzzer_channel, int buzzer_gpio_pin);
void play_note(buzzer_t *buzzer, int note);
void stop_buzzer(buzzer_t *buzzer);