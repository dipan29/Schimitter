#include <SPI.h>  
#include "RF24.h"



RF24 myRadio (8, 7);

struct package
{
  int id=0;
  float temperature = 0.0;
  char  text[100] ="empty";
};

byte addresses[][6] = {"0"}; 



typedef struct package Package;
Package data;

void setup() 
{
  Serial.begin(115200);
  delay(1000);

  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MIN);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  //myRadio.setPayloadSize(sizeof(data));
  myRadio.startListening();
}

void loop()  
{

  if ( myRadio.available()) 
  {
    while (myRadio.available())
    {
      myRadio.read( &data, sizeof(data) );
    }
    Serial.print("\nPackage:");
    Serial.print(data.id);
    Serial.print("\n");
    Serial.println(data.temperature);
    Serial.println(data.text);
  } else {
    //Serial.println("Radio Unavailable");
  }
  //delay(500);
}
