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
- Napin painalluksen jälkeen jakaa lääkkeen joka 30 sekunti napin painamisesta eteenpäin // testiajalla
- Piezolla tarkistetaan että lääke on tippunut, jos ei, LED vilkkuu viisi kertaa // tällä hetkellä toimii yhden slotin myöhässä, check jos osaatte auttaa
- Kun kaikki lääkkeet jaettu, takaisin alkuun (LED vilkkuu, odottaa napin painallusta ja kalibrointia) // tehty
*/

// to do: check näille oikea paikka headerissä, tässä ja funktioissa, ettei oo turhaan mutta että toimii
int step_current;
queue_t events;

int main() {
    setup();
    int state = 1;
    int steps = 4096;
    uint32_t t = time_us_32();
    printf(">> Program started, press middle button to begin <<\n");
    uint count = 0;

    step_current = 0;
    queue_init(&events, sizeof(int), 100);

    gpio_set_irq_enabled_with_callback(PIEZO_SENS, GPIO_IRQ_EDGE_FALL, true, &handler);

    while (true) {
        switch (state) {

        case 1:
                // blin led
                toggle_led();
                if (pressed(BUTT1)) {
                    state = 2;
                }
                sleep_ms(100);
                break;

            case 2:
                calibrate();
                // led on for next step
                gpio_put(LED1, 1);
                state = 3;
                break;

            case 3:
                // LED is already ON
                // Wait for button press (closest to led)
                if (pressed(BUTT0)) {
                    gpio_put(LED1, 0);    // LED off during dispensing
                    state = 4;
                }
                break;

            case 4: // detects fine with my device
                if (pressed(BUTT0)) {
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
                        sleep_ms(30); // check how to do the timer thing

                        // if there is no falling edge detected during the time via interrupt, no drop detected
                        while (!queue_try_remove(&events, &value)) {
                            indicate_miss();
                            break;
                        }
                        // what time between dispenses
                        if (counter < DISPENSER_SLOTS - 1) {
                            sleep_ms(5000);
                        }
                    }
                    printf("all slots emptied\n");
                    state = 1;
                }
        }
    }
    return 0;
}