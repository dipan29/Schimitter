#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (9, 10);
byte addresses[][6] = {"0"};

struct package {
  int id=1;
  float temperature = 18.3;
  char  text[300] = "Text to be transmit";
};


typedef struct package Package;
Package data;


void setup() {
  Serial.begin(115200);
  delay(1000);
  
  myRadio.begin();  
  myRadio.setChannel(55); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_1MBPS );
  myRadio.openWritingPipe(addresses[0]);
}

void loop() {
  myRadio.write(&data, sizeof(data)); 

  Serial.print("\nPackage:");
  Serial.print(data.id);
  Serial.print("\n");
  Serial.println(data.temperature);
  Serial.println(data.text);
  data.id = data.id + 1;
  data.temperature = data.temperature+0.1;
  delay(100);
}
