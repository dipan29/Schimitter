/* Servo Calibrations 
 *  a-> 
 *  b-> (30,180)
 *  c-> (10,90)
 *  d-> (10-120)
 */

// Servo Configuration
int pos_max[4] = {180,180,180,90};
int pos_min[4] = {0,0,0,0};
int pos_mid[4] = {90,90,90,45};

int curr_pos[4] = {90,90,90,45}; // SET MEAN POSITIONS HERE 
int curr_pos_val[4] ; // For storing PWM Values

// Include RadioHead ReliableDatagram & NRF24 Libraries
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>

// Include dependant SPI Library 
#include <SPI.h>

//Include Servo Library
#include <ServoTimer2.h>

ServoTimer2 Servo0; // Base
ServoTimer2 Servo1; // Elbow Circular
ServoTimer2 Servo2; // Wrist UpDown
ServoTimer2 Servo3; // Grip

// Define addresses for radio channels
#define CLIENT_ADDRESS 2   
#define SERVER_ADDRESS 7


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
  Serial.begin(9600);
  
  Servo0.attach(2); // Set Servo to Pin 3 of Arduino
  Servo1.attach(4); // Set Servo to Pin 5 of Arduino
  Servo3.attach(3); // Set Servo to Pin 6 of Arduino
  Servo2.attach(5); // Set Servo to Pin 9 of Arduino

  // Set Servos to Mean position
  for(int i = 0; i <4 ; i++) {
    curr_pos_val[i] = map(curr_pos[i],0,180,750,2250);
  }
  // This library requires pulsewidth so using that
  Servo0.write(curr_pos_val[0]);
  Servo1.write(curr_pos_val[1]);
  Servo2.write(curr_pos_val[2]);
  Servo3.write(curr_pos_val[3]); 

  // Servo Position Setting Done
  
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
    uint8_t len = 
    sizeof(buf);
    uint8_t from;
    if (RadioManager.recvfromAck(buf, &len, &from))
    {

      //Serial Print the values of joystick
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": Servo_A = ");
      Serial.print(buf[0]);
      Serial.print(" | Servo_B = ");
      Serial.print(buf[1]);
      Serial.print(" | Servo_C = ");
      Serial.println(buf[2]);
      Serial.print(" | Servo_D = ");
      Serial.println(buf[3]);

      // Update Servo Position Condition Based
      // Servo A / 0
      if (buf[0] >= 140 && cur_pos[0] <= (pos_max[0] - 15) {
        curr_pos[0] += 15;
        curr_pos_val[0] = map(curr_pos[0],0,180,750,2250);
        Servo0.write(curr_pos_value[0]);        
      } else if (buf[0] <= 110 && cur_pos[0] >= (pos_min[0] + 15) {
        curr_pos[0] -= 15;
        curr_pos_val[0] = map(curr_pos[0],0,180,750,2250);
        Servo0.write(curr_pos_value[0]);
      }
      // Servo B / 1
      if (buf[1] >= 140 && cur_pos[1] <= (pos_max[1] - 15) {
        curr_pos[1] += 15;
        curr_pos_val[1] = map(curr_pos[1],0,180,750,2250);
        Servo1.write(curr_pos_value[1]);        
      } else if (buf[1] <= 110 && cur_pos[1] >= (pos_min[1] + 15) {
        curr_pos[1] -= 15;
        curr_pos_val[1] = map(curr_pos[1],0,180,750,2250);
        Servo1.write(curr_pos_value[1]);
      }
      // Servo C / 2
      if (buf[2] >= 140 && cur_pos[2] <= (pos_max[2] - 15) {
        curr_pos[2] += 15;
        curr_pos_val[2] = map(curr_pos[2],0,180,750,2250);
        Servo2.write(curr_pos_value[2]);        
      } else if (buf[2] <= 110 && cur_pos[2] >= (pos_min[0] + 15) {
        curr_pos[2] -= 15;
        curr_pos_val[2] = map(curr_pos[2],0,180,750,2250);
        Servo2.write(curr_pos_value[2]);
      }
      // Servo D / 3
      if (buf[3] >= 140 && cur_pos[3] <= (pos_max[3] - 15) {
        curr_pos[3] += 15;
        curr_pos_val[3] = map(curr_pos[3],0,180,750,2250);
        Servo3.write(curr_pos_value[3]);        
      } else if (buf[3] <= 110 && cur_pos[3] >= (pos_min[3] + 15) {
        curr_pos[3] -= 15;
        curr_pos_val[3] = map(curr_pos[3],0,180,750,2250);
        Servo3.write(curr_pos_value[3]);
      }
      

// Send a reply back to the originator client, check for error
      if (!RadioManager.sendtoWait(ReturnMessage, sizeof(ReturnMessage), from))
        Serial.println("sendtoWait failed");
    }
  }              
}
