#ifndef DOSETTI_PILL_DISPENSER_H
#define DOSETTI_PILL_DISPENSER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"


#define BUTT0 9
#define BUTT1 8
#define BUTT2 7

#define LED1 20

#define BAUD_RATE 9600
#define TIMEOUT_WAIT 500
#define OPTO_FORK 28
#define PIEZO_SENS 27
#define MOTOR_1 2
#define MOTOR_2 3
#define MOTOR_3 6
#define MOTOR_4 13
#define MOTOR_STEPS 8

#define DISPENSER_SLOTS 8
#define CALIBRATION_OFFSET 135

#define RESPONSE_LENGTH 128
#define DEBOUNCE_TIMER 20
#define LED_BLINK 500000
#define TURN_TIME 5
#define DETECTION_TIMER 90000
#define BLINK_COUNT 5

extern int step_current;
extern bool led_state;
extern volatile int calibration_falling;
extern volatile bool piezo_detection;
extern volatile alarm_id_t alarm_id;
extern volatile uint pending_button;

int64_t debounce_callback(alarm_id_t id, void *user_data);
void interrupt_callback(uint gpio, uint32_t event_mask);
void motor_step(void);
void turn(const int step_count, const bool clockwise);
void indicate_miss(void);
int calibrate(void);
bool pressed(int button);
void setup(void);

#endif //DOSETTI_PILL_DISPENSER_H