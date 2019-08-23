#include<RHReliableDatagram.h>
#include<RH_NRF24.h>
#include<SPI.h>


#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2


RH_NRF24 RadioDriver;

RHReliableDatagram RadioManager (RadioDriver, SERVER_ADDRESS);

uint8_t ReturnMessage[] = "Pot data received";

uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];

void setup()
{
  Serial.begin(9600);
  if(!RadioManager.init())
    Serial.println("init failed");
}

void loop()
{
  if (RadioManager.available())
  {
   uint8_t len =sizeof(buf);
   uint8_t from;
   if (RadioManager.recvfromAck(buf, &len, &from))
   {
     Serial.print("Got request from : 0x");
     Serial.print(from, HEX);
     Serial.print("pot1: ");
     Serial.print(buf[0]);
     Serial.print("pot2: ");
     Serial.print(buf[1]);
     Serial.print("pot3: ");
     Serial.print(buf[2]);
     Serial.print("pot4: ");
     Serial.print(buf[3]);
     
     if(!RadioManager.sendtoWait(ReturnMessage, sizeof(ReturnMessage), from))
     Serial.println("sendtowait failed:");
   }
  }
  delay(100);
}
