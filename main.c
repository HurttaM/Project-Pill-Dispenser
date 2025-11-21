#include <stdio.h>
#include "pico/stdlib.h"
#include "iuart.h"

#define BUTT0 9
#define BUTT1 8
#define BUTT2 7
#define UART_NR 1
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define BAUD_RATE 9600
#define TIMEOUT_WAIT 500
#define MOTOR_REV_DET 28
#define MOTOR_1 2
#define MOTOR_2 3
#define MOTOR_3 6
#define MOTOR_4 13
#define MOTOR_STEPS 8

const int pins[4] = {MOTOR_1, MOTOR_2, MOTOR_3, MOTOR_4};
const int step1[4] = {1, 0, 0, 0};
const int step2[4] = {1, 1, 0, 0};
const int step3[4] = {0, 1, 0, 0};
const int step4[4] = {0, 1, 1, 0};
const int step5[4] = {0, 0, 1, 0};
const int step6[4] = {0, 0, 1, 1};
const int step7[4] = {0, 0, 0, 1};
const int step8[4] = {1, 0, 0, 1};
const int *sequences[MOTOR_STEPS] = {step1, step2, step3, step4, step5, step6, step7, step8};

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


/*
Projektin (vähimmäis)tavoitteet:
- Dispenser kalibroidaan, se kääntyy 30 sekunnin välein, piezo-sensori varmistaa, että pilleri on tippunut
- Dispenserin tila säilytetään in non-volatile memory, jotta sen tila säilyy käynnistämisen ja sammuttamisen yli, tila kommunikoidaan
serverille käyttäen LoRaWANia (tämä olikin more advanced)

Logiikka:
- Kun laitetaan päälle, LED vilkkuu ja odottaa napin painamista.
- Kun nappia painetaan, dispenser kalibroituu kääntymällä vähintään yhden kokonaisen kierroksen ja sitten pysähtyy oikealle kohdalle suhteessa aukkoon
- Kalibroinnin jälkeen LED päällä ennen kuin painetaan toista nappia
- Napin painalluksen jälkeen jakaa lääkkeen joka 30 sekunti napin painamisesta eteenpäin
- Piezolla tarkistetaan että lääke on tippunut, jos ei, LED vilkkuu viisi kertaa
- Kun kaikki lääkkeet jaettu, takaisin alkuun (LED vilkkuu, odottaa napin painallusta ja kalibrointia)
*/

int main() {

    const uint led_pin = 22;
    uint count = 0;

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    // Initialize chosen serial port
    stdio_init_all();

    // Loop forever
    while (true) {

        // Blink LED
        printf("Blinking! %u\r\n", ++count);
        // gpio_put(led_pin, true);
        sleep_ms(1000);
        gpio_put(led_pin, false);
        sleep_ms(1000);
    }
}