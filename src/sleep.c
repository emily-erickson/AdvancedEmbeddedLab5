#include <stdio.h>
#include <pico/stdlib.h>

int main(void)
{
    int toggle = 0;
    stdio_init_all();
    gpio_init(11);
    gpio_set_dir(11, GPIO_OUT);
    gpio_put(11, toggle);

    while (true) {
        toggle = !toggle;
        gpio_put(11, toggle);
        sleep_ms(100);
    }
}
