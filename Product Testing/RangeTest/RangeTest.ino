#define LEDPIN 3
#include <SPI.h>
#include "RF24.h"
#include <printf.h>

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

byte addresses[][6] = {"0","1"};

// Used to control whether this node is sending or receiving
bool role = 1;

void setup() {

    pinMode(LEDPIN, OUTPUT);
    printf_begin();

    Serial.begin(57600);
    Serial.println(F("RF24/examples/GettingStarted"));
    Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

    radio.begin();

    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);

    // Open a writing and reading pipe on each radio, with opposite addresses
    if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
    }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
    }

    // Start the radio listening for data
    radio.startListening();
}

void loop() {


/****************** Ping Out Role ***************************/
if (role == 1)  {

    radio.stopListening();

    Serial.println(F("Now sending"));

    unsigned long time = micros();
        if (!radio.write( &time, sizeof(unsigned long) )){
        Serial.println(F("failed"));
        }

    radio.startListening();                                    // Now, continue listening

    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not

    while ( ! radio.available() ){                             // While nothing is received
        if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
            timeout = true;
            break;
        }
    }

    if ( timeout ){                                             // Describe the results
        Serial.println(F("Failed, response timed out."));
    }else{
        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
        radio.read( &got_time, sizeof(unsigned long) );
        unsigned long time = micros();

        // Spew it
        Serial.print(F("Sent "));
        Serial.print(time);
        Serial.print(F(", Got response "));
        Serial.print(got_time);
        Serial.print(F(", Round-trip delay "));
        Serial.print(time-got_time);
        Serial.println(F(" microseconds"));

        Serial.println(radio.getDataRate());
        radio.printDetails();
        digitalWrite(LEDPIN, HIGH);
        delay(100);
        digitalWrite(LEDPIN, LOW);

    }

    // Try again 1s later
    delay(1000);
    }



/****************** Pong Back Role ***************************/

    if ( role == 0 )
    {
    unsigned long got_time;

    if( radio.available()){
                                                                    // Variable for the received timestamp
        while (radio.available()) {                                   // While there is data ready
        radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
        }

        radio.stopListening();                                        // First, stop listening so we can talk
        radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.
        radio.startListening();                                       // Now, resume listening so we catch the next packets.
        Serial.print(F("Sent response "));
        Serial.println(got_time);
    }
    }

} // Loop
