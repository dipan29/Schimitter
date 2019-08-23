#include <SPI.h>

long int lastTime = 0;

void IRQ() {

  int whatIsData = 0;
  int byte1 = 0 , byte2 = 0 , byte3 = 0 , byte4 = 0 , byte5 = 0;
  float data = 0 , hum = 0 , buf = 0;

  /*Serial.println("STATUS register from IRQ ISR :  ");
    digitalWrite(8, 0);
    SPI.transfer(0x07);
    delay(10);
    Serial.println(SPI.transfer(0xff), BIN);
    delay(10);
    digitalWrite(8, 1);*/

  digitalWrite(8, 0);
  SPI.transfer(0x61);
  delay(10);
  whatIsData = SPI.transfer(0xff);
  delay(10);
  byte1 = SPI.transfer(0xff);
  delay(10);
  byte2 = SPI.transfer(0xff);
  delay(10);
  byte3 = SPI.transfer(0xff);
  delay(10);
  byte4 = SPI.transfer(0xff);
  delay(10);
  byte5 = SPI.transfer(0xff);
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
  delay(10);


  switch (whatIsData) {
    case 1: {

        buf = byte2;
        data = byte1 + buf / 10;
        buf = byte4;
        hum = byte3 + buf / 10;

        Serial.print("Received data from DHT22 -> temp:");
        Serial.print(data);
        Serial.print("  humidity:");
        Serial.println(hum);

        break;
      }
    case 2: {

        Serial.println("Received data from PIR -> Move detected");

        break;
      }
    case 3: {

        buf = byte2;
        data = byte1 + buf / 10;

        Serial.print("Received data from gas sensor -> %gas:");
        Serial.println(data);

        break;
      }
    default: {

        Serial.println("a problem accured");

        break;
      }
  }

}

void NRF_receiverBegin() {

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
  delay(100);

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
  delay(10);

  digitalWrite(8, 0);
  SPI.transfer(0x00 + 0x20);
  delay(10);
  SPI.transfer(0x0f);
  delay(10);
  digitalWrite(8, 1);
  delay(10);

  /*digitalWrite(8, 0);
    SPI.transfer(0x11 + 0x20);
    delay(10);
    SPI.transfer(0x01);
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

}


void NRF_startListening() {

  attachInterrupt(digitalPinToInterrupt(2), IRQ, FALLING);

  digitalWrite(7, 1);   //CE High
  Serial.println("Listening started");

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

  NRF_receiverBegin();
  NRF_startListening();
  NRF_monitorRegisters();

}


void loop() {

  /*if (millis() - lastTime > 20000) {
    NRF_monitorRegisters();
    lastTime = millis();
  }*/

}
