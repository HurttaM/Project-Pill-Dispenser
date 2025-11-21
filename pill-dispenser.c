#include "pill-dispenser.h"

int steps[8][4] = { // 8 steps, 4 motors -> multidim array
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
    {1, 0, 0, 1}
};

int calibrate(void) {
    int results[3];
    int step_index = 0;
    printf(">> Calibrating... <<\n");
    int motor_steps = 0;

    for (int edge = 0; edge < 4; edge++) {
        motor_steps = 0;
        int prev = gpio_get(OPTOFORK);
        int curr = prev;

        while (!(prev == 1 && curr == 0)) {
            prev = curr;

            step_motor(step_index);
            step_index = (step_index + 1) % 8;
            sleep_ms(1.5); // 1 tulee liikaa, 2 tulee täsmälleen 4096, 1.5 tulee 4100 esim

            curr = gpio_get(OPTOFORK);
            motor_steps++;
        }
        if (edge > 0) {
            printf("%d. revolution: %d steps\n", edge, motor_steps);
        }
        if (edge > 0) {
            results[edge - 1] = motor_steps;
        }
    }
    int average = (results[0] + results[1] + results[2]) / 3;
    printf("Average step count per revolution = %d\n", average);
    return motor_steps;
}

void set_up_gpio(void) {
    gpio_init(OPTOFORK);
    gpio_set_dir(OPTOFORK, GPIO_IN);
    gpio_pull_up(OPTOFORK);

    gpio_init(STEPM1);
    gpio_set_dir(STEPM1, GPIO_OUT);
    gpio_init(STEPM2);
    gpio_set_dir(STEPM2, GPIO_OUT);
    gpio_init(STEPM3);
    gpio_set_dir(STEPM3, GPIO_OUT);
    gpio_init(STEPM4);
    gpio_set_dir(STEPM4, GPIO_OUT);
}

void step_motor(int index) {
    gpio_put(STEPM1, steps[index][0]);
    gpio_put(STEPM2, steps[index][1]);
    gpio_put(STEPM3, steps[index][2]);
    gpio_put(STEPM4, steps[index][3]);
}

int run_motor(int run_times, int motor_steps) {
    int take_steps_amount = run_times * motor_steps / 8;
    int counter = 0;

    int step_index = 0;
    for (int i = 0; i < take_steps_amount; i++) {
        step_motor(step_index);
        step_index = (step_index + 1) % 8;
        counter++;
        sleep_ms(2);
    }
    return counter;
}

