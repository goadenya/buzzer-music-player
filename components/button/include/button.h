#pragma once

#include <stdbool.h>
#include "driver/gpio.h"

typedef struct
{
    gpio_num_t pin;
    bool previous_state;
} button_t;

void button_init(button_t *button, gpio_num_t pin);
bool button_pressed_state(button_t *button);
