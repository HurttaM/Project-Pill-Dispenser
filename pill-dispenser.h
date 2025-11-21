#ifndef DOSETTI_PILL_DISPENSER_H
#define DOSETTI_PILL_DISPENSER_H

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "iuart.h"

#define LED1 20
#define OPTOFORK 28
#define STEPM1 2
#define STEPM2 3
#define STEPM3 6
#define STEPM4 13
#define SLEEP_BETWEEN_STEPS 2

/* not used yet ?
#define BUTT0 9
#define BUTT1 8
#define BUTT2 7
#define UART_NR 1
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define BAUD_RATE 9600
#define TIMEOUT_WAIT 500
#define MOTOR_STEPS 8
#define LED1 20
*/

extern int steps[8][4];

void set_up_gpio(void);
void step_motor(int index);
int run_motor(int run_times, int motor_steps);
int calibrate(void);


/*
void setup() {
    stdio_init_all();
    gpio_init(MOTOR_1);
    gpio_init(MOTOR_2);
    gpio_init(MOTOR_3);
    gpio_init(MOTOR_4);
    gpio_init(BUTT0);
    gpio_init(BUTT1);
    gpio_init(BUTT2);
    gpio_init(MOTOR_REV_DET);
    gpio_set_dir(MOTOR_1, true);
    gpio_set_dir(MOTOR_2, true);
    gpio_set_dir(MOTOR_3, true);
    gpio_set_dir(MOTOR_4, true);
    gpio_set_dir(MOTOR_REV_DET, GPIO_IN);
    gpio_set_dir(BUTT0, GPIO_IN);
    gpio_set_dir(BUTT1, GPIO_IN);
    gpio_set_dir(BUTT2, GPIO_IN);
    gpio_pull_up(BUTT0);
    gpio_pull_up(BUTT1);
    gpio_pull_up(BUTT2);
    gpio_pull_up(MOTOR_REV_DET);
    iuart_setup(UART_NR, UART_TX_PIN, UART_RX_PIN, BAUD_RATE);
    //gpio_set_irq_enabled_with_callback(MOTOR_REV_DET, GPIO_IRQ_EDGE_FALL, true, falling);
}

*/


#endif //DOSETTI_PILL_DISPENSER_H