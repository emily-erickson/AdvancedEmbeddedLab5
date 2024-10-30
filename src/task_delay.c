#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>


void main_task(__unused void *params)
{
    int toggle = 0;
    while (1) {
        toggle = !toggle;
        gpio_put(11, toggle);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main(void)
{
    stdio_init_all();
    gpio_init(11);
    gpio_set_dir(11, GPIO_OUT);
    const char *rtos_name = "FreeRTOS";
    TaskHandle_t task;
    xTaskCreate(main_task, "MainThread",
                configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1UL, &task);
    vTaskStartScheduler();
    return 0;
}
