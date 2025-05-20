#include "melody_player.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static void melody_task(void *arg)
{
    struct
    {
        const melody_t *melody;
        buzzer_t *buzzer;
    } *params = arg;

    const melody_t *melody = params->melody;
    buzzer_t *buzzer = params->buzzer;
    int index = 0;

    while (1)
    {
        int note = melody->notes[index];
        int duration = melody->durations[index];

        if (note == 0)
        {
            stop_buzzer(buzzer);
        }
        else
        {
            play_note(buzzer, note);
        }

        int delay_ms = duration * melody->beat_ms;
        vTaskDelay(pdMS_TO_TICKS(delay_ms));

        stop_buzzer(buzzer);
        vTaskDelay(pdMS_TO_TICKS(melody->beat_ms / 2));

        index = (index + 1) % melody->length;
    }
}

void start_melody_player_task(const melody_t *melody, buzzer_t *buzzer)
{
    struct
    {
        const melody_t *melody;
        buzzer_t *buzzer;
    } *params = malloc(sizeof(*params));

    params->melody = melody;
    params->buzzer = buzzer;

    xTaskCreate(melody_task, "melody_task", 4096, params, 5, NULL);
}
