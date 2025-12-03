#include "pill-dispenser.h"

const int sequences[MOTOR_STEPS][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
    {1, 0, 0, 1}
};

int64_t debounce_callback(alarm_id_t id, void *user_data) {
    uint pin = (uint)(uintptr_t)user_data;
    if (pin == PIEZO_SENS) {
        piezo_detection = true;
    }
    alarm_id = 0;
    return 0;
}

void interrupt_callback(uint gpio, uint32_t event_mask) {
    if (gpio == OPTO_FORK) {
        calibration_falling++;
        if (calibration_falling == 1) step_current = 0;
    } else {
        if (alarm_id != 0) {
            cancel_alarm(alarm_id);
        }
        alarm_id = add_alarm_in_ms(DEBOUNCE_TIMER, debounce_callback, (void *)(uintptr_t)gpio, false);
    }
}

//engaging the stepper motor step
void motor_step(void) {
    static int step_index = 0;
    step_index = (step_index + 1) % 8;
    gpio_put(MOTOR_1, sequences[step_index][0]);
    gpio_put(MOTOR_2, sequences[step_index][1]);
    gpio_put(MOTOR_3, sequences[step_index][2]);
    gpio_put(MOTOR_4, sequences[step_index][3]);
}

//driving the stepper motor
void turn(const int step_count, const bool clockwise) {
    int step_direction = clockwise ? 1 : -1;
    static int current_step = 0;
    for (int i = 0; i < step_count; i++) {
        current_step = (current_step + step_direction + MOTOR_STEPS) % MOTOR_STEPS;
        motor_step();
        sleep_ms(1);
    }
}

void indicate_miss() {
    for (int i = 0; i < 5; i++) {
        gpio_put(LED1, 1);
        sleep_ms(100);
        gpio_put(LED1, 0);
        sleep_ms(100);
    }
    printf("no drop\n");
}

bool button_pressed(uint pin) { //debounce for pull_up button
    int press = 0;
    int release = 0;
    while (press < 3 && release < 3) {
        if (!gpio_get(pin)) {
            press++;
            release = 0;
        } else {
            release++;
            press = 0;
        }
        sleep_ms(10);
    }
    if (press > release) return true;
    return false;
}

int calibrate(void) {
    printf(">> Calibrating... <<\n");
    int motor_steps = 0;

    for (int i = 0; i < 2; i++) {
        motor_steps = 0;
        int last = gpio_get(OPTO_FORK);
        int curr = last;

        while (!(last == 1 && curr == 0)) {
            last = curr;

            motor_step();
            sleep_ms(1);

            curr = gpio_get(OPTO_FORK);
            motor_steps++;
        }
        if (i == 0) { // prints just fyi first (possibly incomplete) round steps and then the full revolution steps
            printf("Step count to first falling edge: %d\n", motor_steps);
        } else {
            printf("Step count of a full revolution: %d\n", motor_steps);
        }
    }
    for (int i = 0; i < 140; i++) { // offsetting the about 130 step disparency between the hole and the motor
        motor_step();
        sleep_ms(1);
    }
    return motor_steps;
}

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

void setup() {
    stdio_init_all();
    gpio_init(LED1);
    gpio_set_dir(LED1, GPIO_OUT);
    gpio_set_function(LED1, GPIO_FUNC_SIO);
    gpio_init(MOTOR_1);
    gpio_init(MOTOR_2);
    gpio_init(MOTOR_3);
    gpio_init(MOTOR_4);
    gpio_init(BUTT0);
    gpio_init(BUTT1);
    gpio_init(BUTT2);
    gpio_init(OPTO_FORK);
    gpio_init(PIEZO_SENS);
    gpio_set_dir(MOTOR_1, true);
    gpio_set_dir(MOTOR_2, true);
    gpio_set_dir(MOTOR_3, true);
    gpio_set_dir(MOTOR_4, true);
    gpio_set_dir(OPTO_FORK, GPIO_IN);
    gpio_set_dir(PIEZO_SENS, GPIO_IN);
    gpio_set_dir(BUTT0, GPIO_IN);
    gpio_set_dir(BUTT1, GPIO_IN);
    gpio_set_dir(BUTT2, GPIO_IN);
    gpio_pull_up(BUTT0);
    gpio_pull_up(BUTT1);
    gpio_pull_up(BUTT2);
    gpio_pull_up(OPTO_FORK);
    gpio_pull_up(PIEZO_SENS);
    gpio_set_irq_enabled_with_callback(OPTO_FORK, GPIO_IRQ_EDGE_FALL, true, interrupt_callback);
    gpio_set_irq_enabled(BUTT0, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(PIEZO_SENS, GPIO_IRQ_EDGE_FALL, true);
}