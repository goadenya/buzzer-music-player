#include <stdio.h>
#include "button.h"

void button_init(button_t *button, gpio_num_t pin)
{
    button->pin = pin;
    button->previous_state = gpio_get_level(pin);

    gpio_config_t gpio_cfg = {
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << pin),
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};

    gpio_config(&gpio_cfg);
}

bool button_pressed_state(button_t *button)
{
    bool current_state = gpio_get_level(button->pin);
    bool is_pressed = (button->previous_state == 1 && current_state == 0);
    button->previous_state = current_state;
    return is_pressed;
}
