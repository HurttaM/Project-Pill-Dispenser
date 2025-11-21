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

bool pressed(int button) {
    int press = 0;
    int release = 0;
    while(press < 3 && release < 3) {
        if(gpio_get(button) == 0) {
            press++;
            release = 0;
        } else {
            release++;
            press = 0;
        }
        sleep_ms(10); // wait 10 ms
    }
    if (press > release) {
        return true;
    }
    return false;
}

int calibrate(void) {
    printf(">> Calibrating... <<\n");
    int motor_steps = 0;

    for (int i = 0; i < 2; i++) {
        motor_steps = 0;
        int last = gpio_get(OPTOFORK);
        int curr = last;

        while (!(last == 1 && curr == 0)) {
            last = curr;

            step_motor();
            sleep_ms(1);

            curr = gpio_get(OPTOFORK);
            motor_steps++;
        }
        if (i == 0) { // prints just fyi first (possibly incomplete) round steps and then the full revolution steps
            printf("Step count to first falling edge: %d\n", motor_steps);
        } else {
            printf("Step count of a full revolution: %d\n", motor_steps);
        }
    }
    for (int i = 0; i < 130; i++) { // offsetting the about 130 step disparency between the hole and the motor
        step_motor();
        sleep_ms(1);
    }
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

void step_motor(void) {
    static int step_index = 0;
    step_index = (step_index + 1) % 8;
    gpio_put(STEPM1, steps[step_index][0]);
    gpio_put(STEPM2, steps[step_index][1]);
    gpio_put(STEPM3, steps[step_index][2]);
    gpio_put(STEPM4, steps[step_index][3]);
}

int run_motor(int run_times, int motor_steps) {
    int take_steps_amount = run_times * motor_steps / 8;
    int counter = 0;

    for (int i = 0; i < take_steps_amount; i++) {
        step_motor();
        counter++;
        sleep_ms(1);
    }
    return counter;
}