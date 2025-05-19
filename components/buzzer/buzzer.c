#include <stdio.h>
#include "buzzer.h"

void buzzer_init(buzzer_t *buzzer, ledc_timer_t buzzer_timer, ledc_channel_t buzzer_channel, int buzzer_gpio_pin)
{
    buzzer->buzzer_timer = buzzer_timer;
    buzzer->buzzer_channel = buzzer_channel;
    buzzer->buzzer_gpio_pin = buzzer_gpio_pin;

    ledc_timer_config_t buzzer_ledc_timer_cfig = {
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = 1000, // Will be updated dynamically
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = buzzer_timer,
        .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&buzzer_ledc_timer_cfig);

    ledc_channel_config_t buzzer_ledc_channel_cfig = {
        .channel = buzzer_channel,
        .duty = 0,
        .gpio_num = buzzer_gpio_pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint = 0,
        .timer_sel = buzzer_timer};
    ledc_channel_config(&buzzer_ledc_channel_cfig);
}

void play_note(buzzer_t *buzzer, int note)
{
    ledc_set_freq(LEDC_LOW_SPEED_MODE, buzzer->buzzer_timer, note);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, buzzer->buzzer_channel, 512);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, buzzer->buzzer_channel);
}

void stop_buzzer(buzzer_t *buzzer)
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, buzzer->buzzer_channel, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, buzzer->buzzer_channel);
}