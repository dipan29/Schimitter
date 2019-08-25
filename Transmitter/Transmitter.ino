// PREFETCH

// Include RadioHead ReliableDatagram & NRF24 Libraries
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>

// Include dependant SPI Library 
#include <SPI.h>

// Define Joystick Connections
#define joyVert    A0 // X-AXIS
#define joyHorz    A1 // Y-AXIS

// Define Joystick Values - Start at 512 (middle position)
int joyposVert = 512;
int joyposHorz = 512;
int joyX; int defX;
int joyY; int defY;

struct DataManager {
  byte in1;
  byte in2;
  byte enA;

  byte in3;
  byte in4;
  byte enB;

  byte ack;
};

DataManager data;

// Define addresses for radio channels
#define CLIENT_ADDRESS 5   
#define SERVER_ADDRESS 9

// Create an instance of the radio driver
RH_NRF24 RadioDriver;

// Sets the radio driver to NRF24 and the client address to 1
RHReliableDatagram RadioManager(RadioDriver, CLIENT_ADDRESS);

// Declare unsigned 8-bit motorcontrol array
// 2 Bytes for motor speeds plus 1 byte for direction control
uint8_t motorcontrol[3]; 

// Define the Message Buffer
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
byte tx_buf[sizeof(data)] = {0};

void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600);
  data.ack = 0;
  // Initialize RadioManager with defaults - 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!RadioManager.init())
    Serial.println("init failed");
  
  // Set initial motor direction as forward
  motorcontrol[2] = 0;

}

void loop()
{
  // Print to Serial Monitor
  Serial.println("Reading motorcontrol values ");
  
  // Read the Joystick X and Y positions
  joyposVert = analogRead(joyVert); 
  joyposHorz = analogRead(joyHorz);

  joyX = map(joyposHorz,0,1023,0,255); 
  if(joyX > 127) 
    defX = joyX - 127;
  else
    defX = 128 - joyX;
    
  joyY = map(joyposHorz,0,1023,0,255);
  
  if(joyY > 127) 
    defY = joyY - 127;
  else
    defY = 128 - joyY;

  // Movement Calling
  
  if( joyY > 230 && defX < 30) //GO Straight Forward
  {
    forward(255, 255);
    Serial.println ("Direction Full Forward");
  } else if (joyY < 40 && defX < 30) // Go Straight Backward
  {
    backward(255, 255);
    Serial.println ("Direction Full Backward");
  }
  

  
  // Type Conversion
  memcpy(tx_buf, &data, sizeof(data));
  
  //Send a message containing Motor Control data to manager_server
  if (RadioManager.sendtoWait((uint8_t *)tx_buf, sizeof(data), SERVER_ADDRESS))
  {
    // Now wait for a reply from the server
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
      Serial.println("No reply, is nrf24_reliable_datagram_server running?");
    }
  }
  else
    Serial.println("sendtoWait failed");

  delay(100);  // Wait a bit before next transmission
}


// Movement Functions 

void forward(int ENA, int ENB) {
  data.in1 = 1;
  data.in2 = 0;
  data.enA = ENA;
  
  data.in3 = 1;
  data.in4 = 0;
  data.enB = ENB;
}

void backward(int ENA, int ENB) {
  data.in1 = 0;
  data.in2 = 1;
  data.enA = ENA;
  
  data.in3 = 0;
  data.in4 = 1;
  data.enB = ENB;
}

void leftF(int ENA, int ENB) {
  data.in1 = 1;
  data.in2 = 0;
  data.enA = ENA;
  
  data.in3 = 1;
  data.in4 = 0;
  data.enB = ENB;
}

void rightF(int ENA, int ENB) {
  data.in1 = 1;
  data.in2 = 0;
  data.enA = ENA;
  
  data.in3 = 1;
  data.in4 = 0;
  data.enB = ENB;
}

void leftB(int ENA, int ENB) {
  data.in1 = 0;
  data.in2 = 1;
  data.enA = ENA;
  
  data.in3 = 0;
  data.in4 = 1;
  data.enB = ENB;
}

void rightB(int ENA, int ENB) {
  data.in1 = 0;
  data.in2 = 1;
  data.enA = ENA;
  
  data.in3 = 0;
  data.in4 = 1;
  data.enB = ENB;
}

void left(int ENA, int ENB) {
  data.in1 = 0;
  data.in2 = 0;
  data.enA = ENA;
  
  data.in3 = 1;
  data.in4 = 0;
  data.enB = ENB;
}

void right(int ENA, int ENB) {
  data.in1 = 1;
  data.in2 = 0;
  data.enA = ENA;
  
  data.in3 = 0;
  data.in4 = 0;
  data.enB = ENB;
}

void Stop() {
  data.in1 = 1;
  data.in2 = 0;
  data.enA = 0;
  
  data.in3 = 0;
  data.in4 = 0;
  data.enB = 0;
}
