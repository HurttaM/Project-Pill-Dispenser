#include "pill-dispenser.h"

int step_current;
queue_t events;

int main() {
    setup();
    int state = 1;
    int steps;
    printf(">> Program started, press middle button to begin <<\n");

    step_current = 0;
    queue_init(&events, sizeof(int), 100);

    gpio_set_irq_enabled_with_callback(PIEZO_SENS, GPIO_IRQ_EDGE_FALL, true, &handler);

    while (true) {
        switch (state) {

        case 1:
                // blink led
                toggle_led();
                if (pressed(BUTT1)) {
                    state = 2;
                }
                sleep_ms(100);
                break;

            case 2:
                // calibrate and put led on
                steps = calibrate();
                gpio_put(LED1, 1);
                state = 3;
                break;

            case 3:
                // LED is already ON, wait for button press (closest to led)
                if (pressed(BUTT0)) {
                    state = 4;
                }
                break;

            case 4:
                if (pressed(BUTT0)) {
                    gpio_put(LED1, 0);
                    int value;

                    // dispensing pills
                    for (int counter = 0; counter < DISPENSER_SLOTS; counter++) {
                        // empties events queue
                        while (queue_try_remove(&events, &value));

                        if (counter < 7) {
                            printf("dropping #%d\n", counter+1);
                        }

                        // moving a slot forward
                        for (int i = 0; i < steps / 8; i++) {
                            motor_step();
                            sleep_ms(1);
                        }
                        sleep_ms(PIEZO_TIMER); // 100 ms should work

                        // if there is no falling edge detected during the time via interrupt, no drop detected
                        while (!queue_try_remove(&events, &value)) {
                            indicate_miss();
                            break;
                        }

                        // currently put for 5 seconds between doses for testing and demoing
                        if (counter < DISPENSER_SLOTS - 2) {
                            sleep_ms(TIME_BETWEEN_DISPENSES);
                        }
                    }
                    sleep_ms(5000);
                    printf("All slots emptied! Press button to start calibrating again.\n");

                    // led on and back to beginning
                    gpio_put(LED1, 1);
                    state = 1;
                }
        }
    }
    return 0;
}