#include <SPI.h>

int sending = 1;
int data = 7;

void IRQ() {
  Serial.println("STATUS register from IRQ ISR :  ");
  digitalWrite(8, 0);
  SPI.transfer(0x07);
  delay(10);
  Serial.println(SPI.transfer(0xff), BIN);
  delay(10);
  digitalWrite(8, 1);

  digitalWrite(8, 0);
  SPI.transfer(0x08);
  delay(10);
  Serial.println(SPI.transfer(0xff), BIN);
  delay(10);
  digitalWrite(8, 1);

  digitalWrite(8, 0);
  SPI.transfer(0x07 + 0x20);
  delay(10);
  SPI.transfer(0xff);
  delay(10);
  digitalWrite(8, 1);
}


void NRF_transmitterBegin() {

  //reading
  /*digitalWrite(8, 0);
    SPI.transfer(0x00);
    delay(10);
    Serial.println(SPI.transfer(0xff), BIN);
    delay(10);
    digitalWrite(8, 1);*/

  pinMode(7, OUTPUT);  //CE
  pinMode(8, OUTPUT);  //CSN
  delay(100);
  digitalWrite(8, 1);
  digitalWrite(7, 0);

  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.begin();
  delay(1000);


  digitalWrite(8, 0);   //flush TX_FIFO
  SPI.transfer(0xe1);
  delay(10);
  digitalWrite(8, 1);


  digitalWrite(8, 0);   //flush RX_FIFO
  SPI.transfer(0xe2);
  delay(10);
  digitalWrite(8, 1);


  digitalWrite(8, 0);
  SPI.transfer(0x07 + 0x20);
  delay(10);
  SPI.transfer(0xf0);
  delay(10);
  digitalWrite(8, 1);


  digitalWrite(8, 0);
  SPI.transfer(0x00 + 0x20);
  delay(10);
  SPI.transfer(0x0e);
  delay(10);
  digitalWrite(8, 1);
  delay(10);


  /*digitalWrite(8, 0);
    SPI.transfer(0x11 + 0x20);
    delay(10);
    SPI.transfer(0x3f);
    delay(10);
    digitalWrite(8, 1);*/


  //Down for DYNPL
  digitalWrite(8, 0);
  SPI.transfer(0x1c + 0x20);
  delay(10);
  SPI.transfer(0x01);
  delay(10);
  digitalWrite(8, 1);


  digitalWrite(8, 0);
  SPI.transfer(0x1d + 0x20);
  delay(10);
  SPI.transfer(0x04);
  delay(10);
  digitalWrite(8, 1);
  //Up for DYNPL


  digitalWrite(8, 0);
  SPI.transfer(0x01 + 0x20);
  delay(10);
  SPI.transfer(0x3f);
  delay(10);
  digitalWrite(8, 1);


  digitalWrite(8, 0);
  SPI.transfer(0x02 + 0x20);
  delay(10);
  SPI.transfer(0x03);
  delay(10);
  digitalWrite(8, 1);


  digitalWrite(8, 0);
  SPI.transfer(0x03 + 0x20);
  delay(10);
  SPI.transfer(0x03);
  delay(10);
  digitalWrite(8, 1);


  digitalWrite(8, 0);
  SPI.transfer(0x04 + 0x20);
  delay(10);
  SPI.transfer(0xff);
  delay(10);
  digitalWrite(8, 1);


  digitalWrite(8, 0);
  SPI.transfer(0x05 + 0x20);
  delay(10);
  SPI.transfer(0x4c);
  delay(10);
  digitalWrite(8, 1);


  digitalWrite(8, 0);
  SPI.transfer(0x06 + 0x20);
  delay(10);
  SPI.transfer(0x06);
  delay(10);
  digitalWrite(8, 1);


  digitalWrite(8, 0);
  SPI.transfer(0x0a + 0x20);
  delay(10);
  SPI.transfer(0xaa);
  delay(10);
  SPI.transfer(0xaa);
  delay(10);
  SPI.transfer(0xaa);
  delay(10);
  SPI.transfer(0xaa);
  delay(10);
  SPI.transfer(0xaa);
  delay(10);
  digitalWrite(8, 1);


  digitalWrite(8, 0);
  SPI.transfer(0x10 + 0x20);
  delay(10);
  SPI.transfer(0xaa);
  delay(10);
  SPI.transfer(0xaa);
  delay(10);
  SPI.transfer(0xaa);
  delay(10);
  SPI.transfer(0xaa);
  delay(10);
  SPI.transfer(0xaa);
  delay(10);
  digitalWrite(8, 1);


  attachInterrupt(digitalPinToInterrupt(2), IRQ, FALLING);
  delay(10);

}

void NRF_transmit(int whatIsData , float data , float hum) {    //hum is used just for temp&humidity
  /*
     byte 0 says what is our data:  1 -> data and humidity    2 -> movement   3 -> gas

     1(float) :   byte 1 -> integer part of temp  byte 2 -> fractional part of temp   byte 3 -> integer part of humidity    byte 4 -> fractional part of humidity    byte 5 -> %battery
     2(binary) :  byte 1 -> %battery
     3(float) :   byte 1 -> integer part of gas   byte 2 -> fractional part of gas    byte 3 -> %battery

     %battery feature is in progress ...
  */

  int byte0 = 0 , byte1 = 0 , byte2 = 0 , byte3 = 0 , byte4 = 0 , byte5 = 0;
  float buf = 0;

  byte0 = whatIsData;

  switch (whatIsData) {
    case 1: {

        byte1 = data;
        buf = data - byte1;
        byte2 = buf * 10;

        byte3 = hum;
        buf = hum - byte3;
        byte4 = buf * 10;

        break;
      }
    case 2: {

        break;
      }
    case 3: {

        byte1 = data;
        buf = data - byte1;
        byte2 = buf * 10;

        break;
      }
    default: {
        break;
      }
  }

  digitalWrite(8, 0);
  SPI.transfer(0xe1);   //flush TX_FIFO empty
  delay(10);
  digitalWrite(8, 1);

  digitalWrite(8, 0);
  SPI.transfer(0xa0);   //write on TX_FIFO
  delay(10);
  SPI.transfer(byte0);
  delay(10);
  SPI.transfer(byte1);
  delay(10);
  SPI.transfer(byte2);
  delay(10);
  SPI.transfer(byte3);
  delay(10);
  SPI.transfer(byte4);
  delay(10);
  SPI.transfer(byte5);
  delay(10);
  digitalWrite(8, 1);

  digitalWrite(7, 1);   //CE High
  delay(1);
  digitalWrite(7, 0);   //CE Low

}


void NRF_monitorRegisters() {

  for (int i = 0; i < 30; i++) {
    digitalWrite(8, 0);
    SPI.transfer(i);
    delay(10);
    Serial.print(i , HEX);
    Serial.print(" -> ");
    Serial.println(SPI.transfer(0xff), BIN);
    delay(10);
    digitalWrite(8, 1);
  }
  Serial.println("");

}


void setup() {

  Serial.begin(115200);

  NRF_transmitterBegin();
  NRF_monitorRegisters();

}

void loop() {

  NRF_transmit(3 , 15.1 , 0);

  delay(1000);
}
