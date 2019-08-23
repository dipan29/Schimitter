#include <SPI.h>  
#include "RF24.h" 

RF24 myRadio (9, 10); 
byte addresses[][6] = {"0"};

struct package {
  int id=0;
  float temperature = 0.0;
  char  text[300] ="empty";
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
  
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}


void loop()  {
  if ( myRadio.available()) {
    while (myRadio.available()){
      myRadio.read( &data, sizeof(data) );
    }
    Serial.print("\nPackage:");
    Serial.print(data.id);
    Serial.print("\n");
    Serial.println(data.temperature);
    Serial.println(data.text);
  }
}
