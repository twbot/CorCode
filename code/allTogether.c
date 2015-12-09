#include <Servo.h>
#include <stdint.h>
#include <log.h>

#include "FreeSixIMU.h"
#include "FIMU_ADXL345.h"
#include "FIMU_ITG3200.h"
#include "ColorSense.h"
#include "ultrasonic.h"
#include "GasSense.h"
#include "FlexSense.h"

//DO NOT INSERT INTO DIGITAL PINS 0, 1, 14, 15, 16, 17

//Flex Pins (Analog)
const uint8_t flexPinLeft = 0;
const uint8_t flexPinRight = 1;

//Gas sensor (Analog)
const uint8_t gasSensor = 2;

//IMU (Analog)
const uint8_t SDA = 20;
const uint8_t SCL = 21;

//Ultrasonic (Digital)
const uint8_t ultraSonicOutput = 8;
const uint8_t ultraSonicInput = 7;

//Motor A (Digital)
const uint8_t motorASpeed = 6;                                                 //pwm
const uint8_t AIn1 = 22;
const uint8_t AIn2 = 23;

//Motor B (Digital)
const uint8_t motorBSpeed = 5;                                                 //pwm
const uint8_t BIn1 = 24;
const uint8_t BIn2 = 25;

//Motor Standby/off (Digital)
const uint8_t STYBY = 26;

//Servo Setup
Servo servo1;                                                                  //servo motor for shoulder of arm
Servo servo2;                                                                  //servo motor for elbow of arm
Servo miniServo1;                                                              //servo motor for claw

//Strings for color sensor
String inputstring = "";                                                       //a string to hold incoming data from the PC
String sensorstring = "";                                                      //a string to hold the data from the Atlas Scientific product
boolean input_stringcomplete = false;                                          //have we received all the data from the PC
boolean sensor_stringcomplete = false;                                         //have we received all the data from the Atlas Scientific product

// Variables
long duration;
uint8_t x = 0;
uint8_t clckwise = 0;
uint8_t countclkwise = 1;
uint8_t arr[] = {-1, 1};

// Boolean Expressions
boolean inPin1 =LOW;
boolean inPin2 = HIGH;

void setup() {

  Serial.begin(38400);
  Serial1.begin(9600);
  Serial3.begin(38400);
  inputstring.reserve(5);

  // UltraSonic 
  pinMode(ultraSonicOutput, OUTPUT);
  pinMode(ultraSonicInput, INPUT);
  
  // Motor A
  pinMode(motorASpeed, OUTPUT);
  pinMode(AIn1, OUTPUT);
  pinMode(AIn2, OUTPUT);
  
  // Motor B
  pinMode(motorBSpeed, OUTPUT);
  pinMode(BIn1, OUTPUT);
  pinMode(BIn2, OUTPUT);

  // Inertial Measurement Unit
  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);
  
  // Motor Sandby (Stop)
  pinMode(STYBY, OUTPUT);
  
  // Servo Motor Setups
  servo1.attach(9);
  servo2.attach(10);
  miniServo1.attach(11);

  //Instantiate Objects
  UltraSonicDetect ultrasonic = UltraSonicDetect();
  GasSense gas = GasSense();
  FreeSixIMU IMU = FreeSixIMU();
  ColorSense color = ColorSense();
}


void loop() {

  if(color.yes()){
    stop();
    armControl();
  }

  for(uint8_t y : arr){
    x = y;

    if (ultrasonic.inches() <= 4 || (flexPosLeft > 800 && flexPosRight < 800) || (flexPosLeft < 800 && flexPosRight > 800)){

      switch (x) {

        case 1:
        //turn right
        //detect orientation with IMU

        if (ultrasonic.inches() <= 4 || (flexPosLeft > 800 && flexPosRight < 800) || (flexPosLeft < 800 && flexPosRight > 800)) {
          //spin 180
          //detect orientation with IMU

          if (ultrasonic.inches() <= 4 || (flexPosLeft > 800 && flexPosRight < 800) || (flexPosLeft < 800 && flexPosRight > 800)) {
            //turn left
            //detect orientation with IMU
          }
          else if(ultrasonic.inches() >= 4 || (flexPosLeft > 800 && flexPosRight > 800)){
            motorMove(0, 128, countclkwise);
            motorMove(1, 128, clckwise);
            delay(//Determine size with odometry( or IMU)
              );

            //turn left
            //detect orientation with IMU
            delay(20);
            motorMove(0 ,128, countclkwise);
            motorMove(1, 128, clckwise);

            goto break;

          }
        }
        else if(ultrasonic.inches() >= 4 || (flexPosLeft > 800 && flexPosRight > 800)){
          motorMove(0, 128, countclkwise);
          motorMove(1, 128, clckwise);
          delay(//Determine size with odometry ( or IMU)
            );

          //turn right
          //detect orientation with IMU
        }
        break;

        case -1:
        // turn left
        // detect orientation with IMU

        if (ultrasonic <= 4 || (flexPosLeft > 800 && flexPosRight < 800) || (flexPosLeft < 800 && flexPosRight > 800)) {
          // spin 180
          // detect orientation with IMU

          if (ultrasonic <= 4 || (flexPosLeft > 800 && flexPosRight < 800) || (flexPosLeft < 800 && flexPosRight > 800)) {
            // turn left
            // detect orientation with IMU
          }
          else if(ultrasonic >= 4 || (flexPosLeft > 800 && flexPosRight > 800)){
            motorMove(0, 128, countclkwise);
            motorMove(1, 128, clckwise);
            delay(// Determine size with IMU
              );

            // turn left
            // detect orientation with IMU
            delay(20);
            motorMove(0 ,128, countclkwise);
            motorMove(1, 128, clckwise);

            goto break;

          }
        }
        else if(ultrasonic >= 4 || (flexPosLeft > 800 && flexPosRight > 800)){
          motorMove(0, 128, countclkwise);
          motorMove(1, 128, clckwise);
          delay(// Determine size with odometry ( or IMU)
            );
          // turn right
          // detect orientation with IMU
        }
        break;

        default:
        printf("ERROR: Fault in code");
        break;
      }
    }
  }
}   


void serialEvent() {                                              //if the hardware serial port0 receives a char              
  char input_char = (char)Serial.read();                          //get the char we just received
  inputstring += input_char;                                      //add the char to the inputString
  if(input_char == '\r') {                                         
    input_stringcomplete = true;                                  //if the incoming character is a carriage return, set the flag
  }                
}  


void serialEvent3(){                                               //if the hardware serial port3 receives a char 
  char input_char = (char)Serial3.read();                          //get the char we just received
  sensorstring += input_char;                                      //add it to the inputString
  if(input_char == '\r') {
    sensor_stringcomplete = true;                                  //if the incoming character is a carriage return, set the flag 
  }              
}


/* ----------------------------------- Fucntion to convert microseconds to inches -------------------------------------*/
long microSecondsToInches(long microseconds) {
  return microseconds / 73.746 / 2;
}


 /* ----------------------------------- Function for incrementing motor drive -------------------------------------*/
void motorDriveIncrement(int x) {

  motorMove(0, 25, 1);
  motorMove(0, 25, 0);
  delay(x/5);
  
  motorMove(0, 50, 1);
  motorMove(0, 50, 0);
  delay(x/5);
  
  motorMove(0, 75, 1);
  motorMove(0, 75, 0);
  delay(x/5);
  
  motorMove(0, 105, 1);
  motorMove(0, 105, 0);
  delay(x/5);
  
  motorMove(0, 128, 1);
  motorMove(0, 128, 0);
  delay(x/5);
}


void armControl(){
  for (int i = 0; i < 180; i+20)
  {
    servo1.write(i);
    wait(1);
  }

}
