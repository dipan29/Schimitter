#include "ServoTimer2.h"
//#include <Servo.h>

//Servo servo, servo2 ;
ServoTimer2 servo;
int value = 0;
int c_value = 750;

void setup() {
  Serial.begin(9600);
  servo.attach(4); 
  //servo2.attach(2); // Attach Servo to Pin 4
}

void loop() {
  
  Serial.println("Enter a Degree Value : - ");
  if(Serial.available() > 0) {
    //int incomingData = Serial.read();
    value = Serial.parseInt();
    if(value > 180 || value < 0) {
      Serial.println("Error! Enter value withing 0 - 180 Range");
      
    } else {
    }
      c_value = map(value,0,180,750,2250);
      Serial.print("Sending ");
      Serial.print(value, DEC);
      Serial.print(" Degrees | ");
      Serial.print(c_value);
      Serial.println(" to the servo.\n");
      servo.write(c_value);
      //servo2.write(value);
    
  }
  delay(1000);
}
