  
// Include RadioHead ReliableDatagram & NRF24 Libraries
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>

// Include dependant SPI Library 
#include <SPI.h>

//Include Servo Library
//#include <Servo.h>
#include <ServoTimer2.h>

ServoTimer2 Servo_a;
ServoTimer2 Servo_b;
ServoTimer2 Servo_c;
ServoTimer2 Servo_d;

// Define addresses for radio channels
#define CLIENT_ADDRESS 5   
#define SERVER_ADDRESS 9

int dir_a = 0; // 0 - No movement; 1 Left; 2 Right
int Deg_a = 90; // (0-180) i.e. Servo Position
int Pos_a = 0;

int dir_b = 0; // 0 - No movement; 1 Left; 2 Right
int Deg_b = 90; // (0-180) i.e. Servo Position
int Pos_b = 0;

int dir_c = 0; // 0 - No movement; 1 Left; 2 Right
int Deg_c = 90; // (0-180) i.e. Servo Position
int Pos_c = 0;

int dir_d = 0; // 0 - No movement; 1 Left; 2 Right
int Deg_d = 90; // (0-180) i.e. Servo Position
int Pos_d = 0;

// Create an instance of the radio driver
RH_NRF24 RadioDriver;

// Sets the radio driver to NRF24 and the server address to 2
RHReliableDatagram RadioManager(RadioDriver, SERVER_ADDRESS);

// Define a message to return if values received
uint8_t ReturnMessage[] = "JoyStick Data Received"; 

// Define the Message Buffer
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];

int bufi[4];

void setup()
{
  // Setup Serial Monitor
  Serial.begin(115200);

   Servo_a.attach(3); // Set Servo to Pin 3 of Arduino

   Servo_b.attach(5); // Set Servo to Pin 5 of Arduino

   Servo_c.attach(6); // Set Servo to Pin 6 of Arduino

   Servo_d.attach(9); // Set Servo to Pin 9 of Arduino

   // Initialize RadioManager with defaults - 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!RadioManager.init())
    Serial.println("init failed");

}

void loop()
{
  Serial.println("Code");
  if (RadioManager.available())
  {
 // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (RadioManager.recvfromAck(buf, &len, &from))
    {

      //Serial Print the values of joystick
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": Servo_A = ");
      Serial.print(buf[0]);
      Serial.print(" Servo_B = ");
      Serial.print(buf[1]);
      Serial.print(" Servo_C = ");
      Serial.println(buf[2]);
      Serial.print(" Servo_D = ");
      Serial.println(buf[3]);
      
      // SET Servo_A ORIENTATION
      if(buf[0] > 525) {
        dir_a = 2;
      } else if (buf[0] >=475) {
        dir_a = 0;
      } else if (buf[0] < 475) {
        dir_a = 1;
      }  
       
        // SET Servo_B ORIENTATION
      if(buf[1] > 525) {
        dir_b = 2;
      } else if (buf[1] >=475) {
        dir_b = 0;
      } else if (buf[1] < 475) {
        dir_b = 1;
      }  

        // SET Servo_C ORIENTATION
      if(buf[2] > 525) {
        dir_c = 2;
      } else if (buf[2] >=475) {
        dir_c = 0;
      } else if (buf[2] < 475) {
        dir_c = 1;
      }  

        // SET Servo_D ORIENTATION
      if(buf[3] > 525) {
        dir_d = 2;
      } else if (buf[3] >=475) {
        dir_d = 0;
      } else if (buf[3] < 475) {
        dir_d = 1;
      }  

// CODE FOR Servo_A MOVEMENT

      if(dir_a != 0) {
        if(dir_a == 1 && Deg_a <= 170){
          Deg_a += 10;          
          Pos_a = map(Deg_a,0,180,750,2250);
          Servo_a.write(Pos_a); // This library requires pulsewidth so using that
          dir_a = 0;
          
        } else if(dir_a == 2 && Deg_a >= 10) {
          Deg_a -= 10;
          Pos_a = map(Deg_a,0,180,750,2250);
          Servo_a.write(Pos_a);
          dir_a = 0;
        }
      } else {
        // Do nothing
        dir_a = 0;
      }

      
// CODE FOR Servo_B MOVEMENT

      if(dir_b != 0) {
        if(dir_b == 1 && Deg_b <= 170){
          Deg_b += 10;          
          Pos_b = map(Deg_b,0,180,750,2250);
          Servo_b.write(Pos_b); // This library requires pulsewidth so using that
          dir_b = 0;
          
        } else if(dir_b == 2 && Deg_b >= 10) {
          Deg_b -= 10;
          Pos_b = map(Deg_b,0,180,750,2250);
          Servo_b.write(Pos_b);
          dir_b = 0;
        }
      } else {
        // Do nothing
        dir_b = 0;
      }

// CODE FOR Servo_C MOVEMENT

      if(dir_c != 0) {
        if(dir_c == 1 && Deg_c <= 170){
          Deg_c += 10;          
          Pos_c = map(Deg_c,0,180,750,2250);
          Servo_c.write(Pos_c); // This library requires pulsewidth so using that
          dir_c = 0;
          
        } else if(dir_c == 2 && Deg_c >= 10) {
          Deg_c -= 10;
          Pos_c = map(Deg_c,0,180,750,2250);
          Servo_c.write(Pos_c);
          dir_b = 0;
        }
      } else {
        // Do nothing
        dir_c = 0;
      }

// CODE FOR Servo_D MOVEMENT

      if(dir_d != 0) {
        if(dir_d == 1 && Deg_d <= 170){
          Deg_d += 10;          
          Pos_d = map(Deg_d,0,180,750,2250);
          Servo_d.write(Pos_d); // This library requires pulsewidth so using that
          dir_d = 0;
          
        } else if(dir_d == 2 && Deg_d >= 10) {
          Deg_d -= 10;
          Pos_d = map(Deg_d,0,180,750,2250);
          Servo_d.write(Pos_d);
          dir_d = 0;
        }
      } else {
        // Do nothing
        dir_d = 0;
      }

/* Servo Calibrations 
 *  a-> 
 *  b-> (30,180)
 *  c-> (10,90)
 *  d-> (10-120)
 */



// Send a reply back to the originator client, check for error
      if (!RadioManager.sendtoWait(ReturnMessage, sizeof(ReturnMessage), from))
        Serial.println("sendtoWait failed");
    }
  }              
}
