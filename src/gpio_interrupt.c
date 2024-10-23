#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/sync.h>
#include <hardware/irq.h>
#include <pico/util/queue.h>
#include <pico/multicore.h>

#define IN_PIN 27
#define OUT_PIN 12

typedef struct
{
    uint32_t event_mask;
} gpio_event_t;

queue_t gpio_event_queue;

void gpio_worker_thread(void)
{
    gpio_event_t event;
    while (1)
    {
        if (queue_try_remove(&gpio_event_queue, &event))
        {
            if (event.event_mask & GPIO_IRQ_EDGE_RISE)
            {
                gpio_put(OUT_PIN, true);
            }
            else if (event.event_mask & GPIO_IRQ_EDGE_FALL)
            {
                gpio_put(OUT_PIN, false);
            }
        }
        sleep_ms(10);
    }
}

void irq_callback(uint gpio, uint32_t event_mask)
{
    if (gpio != IN_PIN)
        return;
    gpio_event_t event = {.event_mask = event_mask};
    queue_try_add(&gpio_event_queue, &event);
}

int main(void)
{
    stdio_init_all();

    gpio_init(IN_PIN);
    gpio_set_dir(IN_PIN, GPIO_IN);

    gpio_init(OUT_PIN);
    gpio_set_dir(OUT_PIN, GPIO_OUT);
    gpio_put(OUT_PIN, true);

    queue_init(&gpio_event_queue, sizeof(gpio_event_t), 10);

    multicore_launch_core1(gpio_worker_thread);

    gpio_set_irq_enabled_with_callback(IN_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, irq_callback);

    while(1) __wfi();
    return 0;
}
