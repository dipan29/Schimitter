// Include RadioHead ReliableDatagram & NRF24 Libraries
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>

// Include dependant SPI Library 
#include <SPI.h>

// Define Joystick Connections
#define joyVert_1    A0 
#define joyHorz_1    A1
#define joyVert_2    A3
#define joyHorz_2    A4

// Define Joystick Values - Start at 512 (middle position)

int joyposVert_1 = 512; 
int joyposHorz_1 = 512;
int joyposVert_2 = 512;
int joyposHorz_2 = 512;

// Define addresses for radio channels
#define CLIENT_ADDRESS 5   
#define SERVER_ADDRESS 9

// Create an instance of the radio driver
RH_NRF24 RadioDriver;

// Sets the radio driver to NRF24 and the client address to 1
RHReliableDatagram RadioManager(RadioDriver, CLIENT_ADDRESS);

// Declare unsigned 8-bit motorcontrol array
// 4 Bytes for direction control
uint8_t motorcontrol[4]; 

// Define the Message Buffer
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];

void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600);
 
  // Initialize RadioManager with defaults - 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!RadioManager.init())
    Serial.println("init failed");

}


void loop()
{
  // Print to Serial Monitor
  Serial.println("Reading motorcontrol values ");
  
  
// Read the Joystick X and Y positions
  joyposVert_1 = analogRead(joyVert_1); 
  joyposHorz_1 = analogRead(joyHorz_1);
  joyposVert_2 = analogRead(joyVert_2); 
  joyposHorz_2 = analogRead(joyHorz_2);
  
  Serial.println(joyposVert_1);
  Serial.println(joyposHorz_1);
  Serial.println(joyposVert_2);
  Serial.println(joyposHorz_2);

 // Determine if this is a forward or backward motion

  motorcontrol[0] = map(joyposVert_1, 0, 1023, 0, 255);
  motorcontrol[1] = map(joyposHorz_1, 0, 1023, 0, 255);
  motorcontrol[2] = map(joyposVert_2, 0, 1023, 0, 255);
  motorcontrol[3] = map(joyposHorz_2, 0, 1023, 0, 255);
  Serial.println(motorcontrol[0]);
  Serial.println(motorcontrol[1]);
  Serial.println(motorcontrol[2]); 
  Serial.println(motorcontrol[3]); 


//Display the Motor Control values in the serial monitor.
  Serial.print("Vertical_1: ");
  Serial.print(motorcontrol[0]);
  Serial.print("Horizontal_1: ");
  Serial.print(motorcontrol[1]);
  Serial.print("Vertical_2: ");
  Serial.println(motorcontrol[2]); 
  Serial.print("Horizontal_2: ");
  Serial.println(motorcontrol[3]); 

//Send a message containing Motor Control data to manager_server
  
  if (RadioManager.sendtoWait(motorcontrol, sizeof(motorcontrol), SERVER_ADDRESS))
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
