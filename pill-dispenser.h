// iuart files not used in the project, just here for reference

#ifndef DOSETTI_PILL_DISPENSER_H
#define DOSETTI_PILL_DISPENSER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/util/queue.h"

#define BUTT0 9
#define BUTT1 8
#define LED1 20
#define OPTO_FORK 28
#define PIEZO_SENS 27
#define MOTOR_1 2
#define MOTOR_2 3
#define MOTOR_3 6
#define MOTOR_4 13
#define MOTOR_STEPS 8
#define TIME_BETWEEN_DISPENSES 30000
#define DISPENSER_SLOTS 8
#define CALIBRATION_OFFSET 135
#define BLINK_COUNT 5
#define PIEZO_TIMER 20

extern int step_current;
extern queue_t events;

void motor_step(void);
void indicate_miss(void);
int calibrate(void);
bool pressed(int button);
void setup(void);
void handler(uint gpio, uint32_t event_mask);
void toggle_led(void);

#endif //DOSETTI_PILL_DISPENSER_H