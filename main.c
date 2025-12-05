#include "pill-dispenser.h"
#include "pico/util/queue.h"

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
bool led_state;
volatile int calibration_falling;
volatile bool piezo_detection;
volatile alarm_id_t alarm_id;
volatile uint pending_button;
queue_t events;

int main() {
    setup();
    int state = 1;
    int steps = 4096;
    uint32_t t = time_us_32();
    printf(">> Program started, press middle button to begin <<\n");
    uint count = 0;

    step_current = 0;
    led_state = false;
    piezo_detection = false;
    alarm_id = 0;
    calibration_falling = false;
    void handler(uint gpio, uint32_t event_mask);
    queue_init(&events, sizeof(int), 100);

    gpio_set_irq_enabled_with_callback(PIEZO_SENS, GPIO_IRQ_EDGE_FALL, true, &handler);

    while (true) {
        switch (state) {

            case 1:
                gpio_put(LED1, 1);
                sleep_ms(100);
                gpio_put(LED1, 0);
                sleep_ms(100);

                if (pressed(BUTT1)) {
                    state = 2;
                }
                break;

            case 2:
                calibrate();   // Uses your LAB3 logic
                gpio_put(LED1, 1);             // LED ON = ready
                state = 3;
                break;

            case 3:
                // LED is already ON

                // Wait for button 2 or 3
                if (pressed(BUTT2) || pressed(BUTT0)) {
                    gpio_put(LED1, 0);    // LED off during dispensing
                    state = 4;
                }
                break;

            case 4: // this works other than the no srop detected and drop detected lag a bit, sometimes drop detected comes after the next dropping #x print
                if (pressed(BUTT0)) {
                    int value;
                    while (queue_try_remove(&events, &value));
                    int counter;
                    for (counter = 0; counter < DISPENSER_SLOTS; counter++) {
                        if (counter < 7) {
                            printf("dropping #%d\n", counter+1);
                        }
                        turn(steps/DISPENSER_SLOTS, true);
                        sleep_ms(30);
                        int value;
                        while (!queue_try_remove(&events, &value)) { // tää laittaa ekan aina tänne eli joku alustava juttu siellä on mitä en osaa tyhjää, korjatkaa jos osaatte
                        // muuten ainakin mulla tunnistaa atm
                            printf("not dropped!\n");
                            indicate_miss();
                            piezo_detection = true;
                            break;
                        }
                        sleep_ms(5000);
                        while (queue_try_remove(&events, &value));
                        sleep_ms(10);
                    }
                    printf("all slots emptied\n");
                    state = 1;
                }
        }
    }
    return 0;
}