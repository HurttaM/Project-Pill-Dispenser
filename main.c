#include "pill-dispenser.h"

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

    // initializing everything here:

    // Initialize LED pin
    gpio_init(LED1);
    gpio_set_dir(LED1, GPIO_OUT);

    // Initialize chosen serial port
    stdio_init_all();



    uint count = 0; // for testing (printing)


    // Loop forever
    while (true) {

        // waits for a button to be pressed and blinks a led
        printf("Blinking! %u\r\n", ++count);
        gpio_put(LED1, true);
        sleep_ms(1000);
        gpio_put(LED1, false);
        sleep_ms(1000);

        // if button pressed, calibrates itself





    }
}