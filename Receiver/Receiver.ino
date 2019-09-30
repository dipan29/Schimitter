// Include RadioHead ReliableDatagram & NRF24 Libraries
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>

// Include dependant SPI Library 
#include <SPI.h>

// Define addresses for radio channels
#define CLIENT_ADDRESS 5   
#define SERVER_ADDRESS 9

// Motor A Connections (LEFT)
int left = 3;
int in1 = 2;
int in2 = 4;

// Motor B Connections (RIGHT)
int right = 5;
int in3 = 6;
int in4 = 7;
int xpos=0;
int ypos=0;
int dir=0;

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
      Serial.print(": MotorA = ");
      Serial.print(buf[0]);
      Serial.print(" MotorB = ");
      Serial.print(buf[1]);
      Serial.print(" Camera = ");
      Serial.println(buf[2]);

      xpos=buf[0]-123;
      ypos=buf[1]-129;
      Serial.println(xpos);
      Serial.println(ypos);

      //Motor Variables
      if(xpos ==0 && ypos ==0){
        analogWrite(right, 0);
        analogWrite(left, 0);
        Serial.println("zero zero condition");
        dir=2;
      }
      else{
        if(xpos == 0 && ypos > 0){
          //up arrow
          analogWrite(right, 255);
          analogWrite(left, 255);
          dir=1;
          Serial.println("up arrow condition");
       
        }
        else if(xpos == 0 && ypos < 0){
          //down arrow
          analogWrite(right, 255);
          analogWrite(left, 255);
          dir=0;
          Serial.println("down arrow condition");
       
        }
        else if(xpos < 0 && ypos == 0){
          //left arrow
          analogWrite(right, 255);
          analogWrite(left, 0);
          dir=1;
          Serial.println("left arrow condition");
       
        }
        else if(xpos > 0 && ypos == 0){
          //right arrow
          analogWrite(right, 0);
          analogWrite(left, 255);
          dir=1;
          Serial.println("right arrow condition");
        } 
        if(xpos < 0 && ypos > 0){
          //2nd Quadrant
          analogWrite(right, 255);
          analogWrite(left, 255+2*xpos);
          dir=1;
          Serial.println("2nd Quadrant condition");
       
        }
        else if(xpos < 0 && ypos < 0){
          //3rd Qudrant
          analogWrite(right, 255);
          analogWrite(left, 255+2*xpos);
          dir=0;
          Serial.println("3rd Qudrantcondition");
       
        }
        else if(xpos > 0 && ypos > 0){
          //1st Quadrant
          analogWrite(right, 255-5*xpos);
          analogWrite(left, 255);
          dir=1;
          Serial.println("1st Quadrantcondition");
       
        }
        else if(xpos > 0 && ypos < 0)
        {
          //4th Quadrant
          analogWrite(right, 255-2*xpos);
          analogWrite(left, 255);
          dir=0;
          Serial.println("4th Quadrant condition");
        } 
      }
      if(dir==1){
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
      }
      else if(dir==2){
        
         digitalWrite(in1, LOW);
         digitalWrite(in2, LOW);
         digitalWrite(in3, LOW);
         digitalWrite(in4, LOW);
      }
      else{
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        
      }   
      // Send a reply back to the originator client, check for error
      if (!RadioManager.sendtoWait(ReturnMessage, sizeof(ReturnMessage), from))
        Serial.println("sendtoWait failed");
    }
  }              
}
