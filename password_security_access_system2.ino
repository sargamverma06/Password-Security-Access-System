// Names: Sargam Verma
// Date: 10/04/2023
// Progect: Password Security Access System using Arduino

//To import libraries
#include <Servo.h>
#include <Wire.h>

//Defining leds
int greenLed = 10;
int redLed = 9;

//Defining servo
Servo myservo;
int pinServo = 4;

//Defining buzzer
int buzzer = 5;

//Variables
int count = 3;

//the setup function runs once

void setup(){
  Wire.begin(4);//Start I2C communication as slave with address 4
  Wire.onReceive(receiveEvent);//Set the receiveEvent function to be called when data is received
  myservo.attach(pinServo);//Attach the servo 
  Serial.begin(9600);//Start serial communication
  
  //setup of leds
  pinMode(greenLed, OUTPUT); 
  pinMode(redLed, OUTPUT);
  pinMode(buzzer, OUTPUT); 
}
 //loop function runs forever

void loop(){
  Serial.println(count);
  if(count == 3){
    myservo.write(0);
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, LOW);
    delay(500);
  }
  
  else if(count == 0){
    access();
  }
  
  else if(count == 1){
  	denied();
  }
}

void receiveEvent(int receive){
  count = Wire.read();
  if(count == 0)
  {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    myservo.write(90);
  }
}

void access(){
  myservo.write(90);
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  delay(500);
  count == 3;
}

void denied(){
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, HIGH);    
  myservo.write(0);  
  tone(buzzer,950,500);
  delay(200);
  tone(buzzer,700,400);
  delay(500);
}
