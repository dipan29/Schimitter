#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

RH_NRF24 RadioDriver;

RHReliableDatagram RadioManager (RadioDriver, CLIENT_ADDRESS);

uint8_t servocontrol[5];

uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];

void setup()
{
  Serial.begin(9600);
  if(!RadioManager.init())
    Serial.println("Init failed");
}

void loop()
{
  //Serial.println("Reading pot values");
  //servocontrol[0]= map(analogRead(Potpin1), 0, 1023, 0, 180);
  //servocontrol[1]= map(analogRead(Potpin2), 0, 1023, 0, 180);
  //servocontrol[2]= map(analogRead(Potpin3), 0, 1023, 0, 180);
  //servocontrol[3]= map(analogRead(Potpin4), 0, 1023, 0, 180);
  //servocontrol[4]= map(analogRead(Potpin5), 0, 1023, 0, 180);
  
  servocontrol[0] = 10;
  servocontrol[1] = 20;
  servocontrol[2] = 30;
  servocontrol[3] = 40;
  servocontrol[4] = 50;
  Serial.println("..............");
  Serial.println("pot1:");
  Serial.println(servocontrol[0]);
  Serial.println("pot2:");
  Serial.println(servocontrol[1]);
  Serial.println("pot3:");
  Serial.println(servocontrol[2]);
  Serial.println("pot4:");
  Serial.println(servocontrol[3]);
  Serial.println("pot5:");
  Serial.println(servocontrol[4]);
  Serial.println("Sending data to nrf server side");
  if (RadioManager.sendtoWait(servocontrol, sizeof(servocontrol), SERVER_ADDRESS))
  {
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (RadioManager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf); 
    }
    else
    {
      Serial.println("No reply, is the server running?");
    }
  }
  else
  Serial.println("sendtowait failed");

  delay(100);
}
