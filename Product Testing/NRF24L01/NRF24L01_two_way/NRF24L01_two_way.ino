#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (9, 10);
byte addresses[][6] = {"0"};

struct package {
  int id = 1;
  float temperature = 18.3;
  char  text[300] = "Text to be transmit";
};


typedef struct package Package;
Package dataRecieve;
Package dataTransmit;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS );
  
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}

void loop() {
  if ( myRadio.available()) {
    while (myRadio.available()){
      myRadio.read( &dataRecieve, sizeof(dataRecieve) );
    }
    Serial.println("Recieve: ");
    Serial.print("Package:");
    Serial.print(dataRecieve.id);
    Serial.print("\n");
    Serial.println(dataRecieve.temperature);
    Serial.println(dataRecieve.text);
    Serial.print("\n");
  }

  delay(200);

  myRadio.stopListening();
  dataTransmit.id = dataTransmit.id + 1;
  dataTransmit.temperature = dataTransmit.temperature+0.1;
  Serial.println("Transmit: ");
  Serial.print("Package:");
  Serial.print(dataTransmit.id);
  Serial.print("\n");
  Serial.println(dataTransmit.temperature);
  Serial.println(dataTransmit.text);
  Serial.print("\n");
  myRadio.openWritingPipe(addresses[0]);
  myRadio.write(&dataTransmit, sizeof(dataTransmit));
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}
