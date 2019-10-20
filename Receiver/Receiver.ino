// Include RadioHead ReliableDatagram & NRF24 Libraries
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>

// Include dependant SPI Library 
#include <SPI.h>

//Include Servo Library
//#include <Servo.h>
#include <ServoTimer2.h>

ServoTimer2 camServo;

// Define addresses for radio channels
#define CLIENT_ADDRESS 1   
#define SERVER_ADDRESS 2

// Motor A Connections (LEFT)
int left = 3;
int in1 = 2;
int in2 = 4;

// Motor B Connections (RIGHT)
int right = 5;
int in3 = 6;
int in4 = 7;


int xpos=0; //0 Stop; 1 Left; 2 Right
int ypos=0; //0 Stop; 1 Back; 2 Forward
int dir=0;
int camdir = 0; // 0 - No movement; 1 Left; 2 Right
int camDeg = 90; // (0-180) i.e. Servo Position
int camPos = 0;

// Create an instance of the radio driver
RH_NRF24 RadioDriver;

// Sets the radio driver to NRF24 and the server address to 2
RHReliableDatagram RadioManager(RadioDriver, SERVER_ADDRESS);

// Define a message to return if values received
uint8_t ReturnMessage[] = "JoyStick Data Received"; 

// Define the Message Buffer
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];

int bufi[3];

void setup()
{
  // Setup Serial Monitor
  Serial.begin(115200);
  
  // Set all the motor control pins to outputs
  pinMode(left, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  camServo.attach(9); // Set Servo to Pin 9 of Arduino
  
  // Initialize RadioManager with defaults - 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!RadioManager.init())
    Serial.println("init failed");

  // Initiate the motors with Zero
  analogWrite(right, 0);
  analogWrite(left, 0);
} 

void loop()
{
  
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
      Serial.print(": MotorA = ");
      Serial.print(buf[0]);
      Serial.print(" MotorB = ");
      Serial.print(buf[1]);
      Serial.print(" Camera = ");
      Serial.println(buf[2]);

      //SET xpos & ypos <== buf[1] ; buf[0]

      if(buf[1] > 160) {
        xpos = 2;
      } else if (buf[1] >=110) {
        xpos = 0;
      } else if (buf[1] < 110) {
        xpos = 1;
      }

      if(buf[0] > 160) {
        ypos = 2;
      } else if (buf[0] >=110) {
        ypos = 0;
      } else if (buf[0] < 110) {
        ypos = 1;
      }

      // SET CAMERA ORIENTATION
      if(buf[2] > 130) {
        camdir = 2;
      } else if (buf[2] >=118) {
        camdir = 0;
      } else if (buf[2] < 118) {
        camdir = 1;
      }
      
      // CODE FOR CAMERA MOVEMENT

      if(camdir != 0) {
        if(camdir == 1 && camDeg <= 160){
          camDeg += 10;          
          camPos = map(camDeg,0,180,750,2250);
          camServo.write(camPos); // This library requires pulsewidth so using that
          camdir = 0;
          
        } else if(camdir == 2 && camDeg >= 20) {
          camDeg -= 10;
          camPos = map(camDeg,0,180,750,2250);
          camServo.write(camPos);
          camdir = 0;
        }
      } else {
        // Do nothing
        camdir = 0;
      }

      // Place Servo Value
      
      //Code for Motor Movement

      if(xpos == 0 && ypos == 0) {
        // Stop Condition Always
        analogWrite(left,0);
        analogWrite(right,0);
        digitalWrite(in1,LOW);
        digitalWrite(in2,LOW);
        digitalWrite(in3,LOW);
        digitalWrite(in4,LOW);
      } else if (xpos == 0 && ypos == 2) {
        // Go Forward
        analogWrite(left,255);
        analogWrite(right,255);
        digitalWrite(in1,LOW);
        digitalWrite(in2,HIGH);
        digitalWrite(in3,LOW);
        digitalWrite(in4,HIGH);
      } else if (xpos ==0 && ypos == 1) {
        // Go Backward
        analogWrite(left,255);
        analogWrite(right,255);
        digitalWrite(in1,HIGH);
        digitalWrite(in2,LOW);
        digitalWrite(in3,HIGH);
        digitalWrite(in4,LOW);
      } else if (xpos == 2 && ypos == 2) {
        // Go Forward Right
        analogWrite(left,255);
        analogWrite(right,127);
        digitalWrite(in1,LOW);
        digitalWrite(in2,HIGH);
        digitalWrite(in3,LOW);
        digitalWrite(in4,HIGH);
      } else if (xpos == 1 && ypos == 2) {
        // Go Forward Left
        analogWrite(left,127);
        analogWrite(right,255);
        digitalWrite(in1,LOW);
        digitalWrite(in2,HIGH);
        digitalWrite(in3,LOW);
        digitalWrite(in4,HIGH);
      } else if (xpos == 1 && ypos == 1) {
        // Go Backward Left
        analogWrite(left,127);
        analogWrite(right,255);
        digitalWrite(in1,HIGH);
        digitalWrite(in2,LOW);
        digitalWrite(in3,HIGH);
        digitalWrite(in4,LOW);
      } else if (xpos == 2 && ypos == 1) {
        // Go Backward Right
        analogWrite(left,255);
        analogWrite(right,127);
        digitalWrite(in1,HIGH);
        digitalWrite(in2,LOW);
        digitalWrite(in3,HIGH);
        digitalWrite(in4,LOW);
      } else if (xpos == 2 && ypos == 0) {
        // Go Right
        analogWrite(left,255);
        analogWrite(right,0);
        digitalWrite(in1,LOW);
        digitalWrite(in2,HIGH);
        digitalWrite(in3,LOW);
        digitalWrite(in4,LOW);
      } else if (xpos == 1 && ypos == 0) {
        // Go Left
        analogWrite(left,0);
        analogWrite(right,255);
        digitalWrite(in1,LOW);
        digitalWrite(in2,LOW);
        digitalWrite(in3,LOW);
        digitalWrite(in4,HIGH);
      } else {
        // STOP
        analogWrite(left,0);
        analogWrite(right,0);
        digitalWrite(in1,LOW);
        digitalWrite(in2,LOW);
        digitalWrite(in3,LOW);
        digitalWrite(in4,LOW);
      }
        
      // Send a reply back to the originator client, check for error
      if (!RadioManager.sendtoWait(ReturnMessage, sizeof(ReturnMessage), from))
        Serial.println("sendtoWait failed");
    } else {
        analogWrite(left,0);
        analogWrite(right,0);
        digitalWrite(in1,LOW);
        digitalWrite(in2,LOW);
        digitalWrite(in3,LOW);
        digitalWrite(in4,LOW);
    }
  } else {
            
  }
  //delay(100);
}
