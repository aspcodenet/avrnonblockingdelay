#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include "millis.h"
#include "uart.h"

#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b)))) 

// NON BLOCKING DELAYS
// Delays är INTE förbjudet. 
// när ska man inte använda delay? När vi har input  från sensorer etc 
// som inte får vänta "för länge"

// två sätt att hantera - "händelsestyrd programmering"
//      när status på denna pin ändras -> kör min funktion som heter onChange
//      INTERRUPT
// timer timer.h timer.c 
// http://blog.zakkemble.co.uk/millisecond-tracking-library-for-avr/
//   vår while-loop ska gå så FORT som möjligt

// https://wokwi.com/projects/363955195435615233

// B (digital pin 8 to 13)
// C (analog input pins)
// D (digital pins 0 to 7)
#define LED_PINLEFT 2
#define LED_PINRIGHT 1
#define LED_SWICTCHCLICKER 3
#define SWITCH_PINLEFT 7
#define SWITCH_PINRIGHT 6


// DDRx   -> pinnen är INPUT eller OUTPUT
// om output mode
// ska vi skriva outputen till motsvarade pinne  i PORTx
// om input mode
// ska vi LÄSA inputen på  motsvarade pinne  i PINx

void mainold(){
       // DATA DIRECTION REGISTER B
    BIT_SET(DDRB, LED_PINLEFT); //Sätt led_pin till output mode
    BIT_SET(DDRB, LED_PINRIGHT); //Sätt led_pin till output mode
    BIT_SET(DDRB,LED_SWICTCHCLICKER);
    BIT_CLEAR(DDRD, SWITCH_PINLEFT); // Sätt till input - INPUT_PULLUP
    BIT_CLEAR(DDRD, SWITCH_PINRIGHT); 
    BIT_SET(PORTD, SWITCH_PINLEFT); 
    BIT_SET(PORTD, SWITCH_PINRIGHT); 
 //This means INPUT_PULLUP§
	// https://forum.arduino.cc/t/using-avr-internal-pull-up-for-push-button-controlling/327729/4
	// https://www.hackster.io/Hack-star-Arduino/push-buttons-and-arduino-a-simple-guide-wokwi-simulator-c2281f    

    //antalSekunder = 0;
    // BLOCKING DELAY
    // POLLING ALGORITM - är vi framme snart?
    // PRENUMERATION - du göra nåt annat istället för att bara fråga
    //                      VI KONTAKTAR DIG
    //                          Interrupt
    // setup prenumeration .- när nån ändrar på swictehn anropa funktionen onChange
        while(1){
            if(BIT_CHECK(PIND,SWITCH_PINLEFT)) // Är switchen  si eller så
                BIT_SET(PORTB,LED_SWICTCHCLICKER);
            else
                BIT_CLEAR(PORTB,LED_SWICTCHCLICKER);
            // if antalSekunder >= 3
            //        antalSekunder = 0
            BIT_SET(PORTB, LED_PINLEFT); 
            BIT_CLEAR(PORTB, LED_PINRIGHT); 
            _delay_ms(3000);

            BIT_CLEAR(PORTB, LED_PINLEFT);
            BIT_SET(PORTB, LED_PINRIGHT); 
            _delay_ms(3000);
    }
}


/*
Stephanie Hallberg 10:05
så om man skriver tio tusen rader kod, och använder ett GPL lib för 5 rader, så måste all
 kod skickas om någon frågar?
*/

int main(void)
{
 //   mainold();
    
    // DATA DIRECTION REGISTER B
    BIT_SET(DDRB, LED_PINLEFT); //Sätt led_pin till output mode
    BIT_SET(DDRB, LED_PINRIGHT); //Sätt led_pin till output mode
    BIT_SET(DDRB,LED_SWICTCHCLICKER);
    BIT_CLEAR(DDRD, SWITCH_PINLEFT); 
    BIT_CLEAR(DDRD, SWITCH_PINRIGHT); 
    BIT_SET(PORTD, SWITCH_PINLEFT); 
    BIT_SET(PORTD, SWITCH_PINRIGHT); 
 //This means INPUT_PULLUP
	// https://forum.arduino.cc/t/using-avr-internal-pull-up-for-push-button-controlling/327729/4
	// https://www.hackster.io/Hack-star-Arduino/push-buttons-and-arduino-a-simple-guide-wokwi-simulator-c2281f    

    millis_init();
    sei();
    init_serial();
    
    volatile millis_t antalMilliSekunderSenasteBytet = 0;
    bool isleft = true;
    BIT_SET(PORTB, LED_PINLEFT);
    BIT_CLEAR(PORTB, LED_PINRIGHT); 

    //antalSekunder = 0;
    while(1){
        if(BIT_CHECK(PIND,SWITCH_PINLEFT))
            BIT_SET(PORTB,LED_SWICTCHCLICKER);
        else
            BIT_CLEAR(PORTB,LED_SWICTCHCLICKER);

        //millis()


        //  9522
        // antalet millisekunder sen processorn resettades (startade/startade om)

        
        //antalMilliSekunderSenasteBytet = VAD ÄR KLOCKAn vid senaste bytet;
        //Vad äör klockan nu?
        //skillnaden
        if( millis_get() - antalMilliSekunderSenasteBytet > 3000 )
        {
            printf("Nu byter vi igen\n");

            // EXEKVERAS      VAR TREDJE SEKUND
            if(isleft){
                BIT_CLEAR(PORTB, LED_PINLEFT);
                BIT_SET(PORTB, LED_PINRIGHT); 
            }
            else {
                BIT_SET(PORTB, LED_PINLEFT);
                BIT_CLEAR(PORTB, LED_PINRIGHT); 
            }
            // if(isleft == true)
            //     isleft = false;
            // else
            //     isleft = true;
            isleft = !isleft;
            antalMilliSekunderSenasteBytet = millis_get(); // 13122
        }

        
    }
	return 0;
}