#include "pill-dispenser.h"

/*
Projektin (vähimmäis)tavoitteet:
- Dispenser kalibroidaan, se kääntyy 30 sekunnin välein, piezo-sensori varmistaa, että pilleri on tippunut
- Dispenserin tila säilytetään in non-volatile memory, jotta sen tila säilyy käynnistämisen ja sammuttamisen yli, tila kommunikoidaan
serverille käyttäen LoRaWANia (tämä olikin more advanced)

Logiikka:
- Kun laitetaan päälle, LED vilkkuu ja odottaa napin painamista. // tehty
- Kun nappia painetaan, dispenser kalibroituu kääntymällä vähintään yhden kokonaisen kierroksen ja sitten pysähtyy oikealle kohdalle suhteessa aukkoon // tehty
- Kalibroinnin jälkeen LED päällä ennen kuin painetaan toista nappia // tehty
- Napin painalluksen jälkeen jakaa lääkkeen joka 30 sekunti napin painamisesta eteenpäin // tehty
- Piezolla tarkistetaan että lääke on tippunut, jos ei, LED vilkkuu viisi kertaa
- Kun kaikki lääkkeet jaettu, takaisin alkuun (LED vilkkuu, odottaa napin painallusta ja kalibrointia) // tehty
*/

int main() {

    // initializing everything here:

    // Initialize LED pin
    gpio_init(LED1);
    gpio_set_dir(LED1, GPIO_OUT);

    gpio_init(BUTTON1);
    gpio_set_dir(BUTTON1, GPIO_IN);
    gpio_pull_up(BUTTON1);
    gpio_init(BUTTON2);
    gpio_set_dir(BUTTON2, GPIO_IN);
    gpio_pull_up(BUTTON2);
    gpio_init(BUTTON3);
    gpio_set_dir(BUTTON3, GPIO_IN);
    gpio_pull_up(BUTTON3);


    // Initialize chosen serial port
    stdio_init_all();

    set_up_gpio();

    uint count = 0; // for testing (printing)
    bool calibration_done = false;
    int motor_steps = 0;


    // Loop forever
    while (true) {

        // waits for a button to be pressed and blinks a led
        if (calibration_done == false) {
            printf("Blinking! %u\r\n", ++count);
            gpio_put(LED1, true);
            sleep_ms(100);
            gpio_put(LED1, false);
            sleep_ms(100);
        }

        // if button pressed, calibrates itself
        if (!calibration_done && pressed(BUTTON1)) {
            motor_steps = calibrate(); // the hole is not in just the right place now, has to be fixed
            calibration_done = true;
            gpio_put(LED1, true);
        }

        // if calibration done and another button pressed (here could be either one of the other buttons for convenience)
        // TO DO! piezo sensor thing not started
        if (calibration_done && (pressed(BUTTON2) || pressed(BUTTON3))) {
            gpio_put(LED1, false);
            for (int i = 0; i < 7; i++) {
                run_motor(1, motor_steps);
                sleep_ms(5000); // time requirement is 30 seconds but tested with shorter time for convenience
            }
            calibration_done = false; // this brings the program back to the "beginning" (led blinking, can be calibrated etc)
        }

    }
}