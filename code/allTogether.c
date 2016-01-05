#include <Servo.h>
#include <stdint.h>
#include <log.h>
#include <Wire.h>

#include "IMU.h"
#include "Accel.h"
#include "Gyro.h"
#include "ColorSense.h"
#include "UltraSonicDetect.h"
#include "GasSense.h"
#include "FlexSense.h"
#include "Communication.h"

/* ------------ DO NOT INSERT INTO DIGITAL PINS 0, 1, 14, 15, 16, 17 ---------------*/

/* -- Flex Pins (Analog) --*/
const uint8_t flexPinLeft = 0;
const uint8_t flexPinRight = 1;

/* -- Gas sensor (Analog) --*/
const uint8_t gasSensor = 2;

/* -- IMU (Analog) --*/
const uint8_t SDA = 20;
const uint8_t SCL = 21;

/* -- Ultrasonic (Digital) --*/
const uint8_t ultraSonicOutput = 8;
const uint8_t ultraSonicInput = 7;

/* -- Motor A (Digital) --*/
const uint8_t motorASpeed = 5;                                                 //pwm
const uint8_t AIn1 = 22;                                                        
const uint8_t AIn2 = 23;

/* -- Motor B (Digital) --*/
const uint8_t motorBSpeed = 6;                                                 //pwm
const uint8_t BIn1 = 24;                                                        
const uint8_t BIn2 = 25;

/* -- Motor Standby/off (Digital) --*/
const uint8_t STYBY = 26;

/* -- Servo Setup --*/
Servo servoShoulder;                                                           //servo motor for shoulder of arm
Servo servoElbow;                                                              //servo motor for elbow of arm
Servo servoHand;                                                               //servo motor for claw

/* -- Variables --*/
long duration;
uint8_t x = 0;
uint8_t clckwise = 0;
uint8_t countclkwise = 1;
uint8_t arr[] = {-1, 1};
uint32_t q[4];

// Boolean Expressions for motors
uint8_t inPin1 =LOW;
uint8_t inPin2 = HIGH;

//Instantiate Objects
UltraSonicDetect ultrasonic = UltraSonicDetect();
GasSense gas = GasSense();
IMU imu = IMU();
ColorSense color = ColorSense();
FlexSense cliffSense = FlexSense();

void setup() {

  Serial.begin(38400);    //For debugging purposes; DELETE WHEN DONE
  Serial1.begin(9600);    //For debugging purposes; DELETE WHEN DONE
  Serial3.begin(38400);   
  Serial.begin(115200);   //For debugging purposes; DELETE WHEN DONE
  inputstring.reserve(5); //For debugging purposes; DELETE WHEN DONE

  Wire.begin();

  //Initialize IMU
  imu.init();

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
  servoShoulder.attach(9);
  servoElbow.attach(10);
  servoHand.attach(11);
}


void loop() {

  cliffSense.setFlexPosLeft();
  cliffSense.setFlexPosRight();
  color.getColor();

  if(color.yes() ){
    stop();
    armControl();
  }

  for(uint8_t y : arr){
    x = y;

    if (ultrasonic.inches() <= 4 || (flexLeft > 800 && flexRight < 800) || (flexLeft < 800 && flexRight > 800)){

      switch (x) {

        case 1:
        //turn right
        //detect orientation with IMU
        //spin 180
        //detect orientation with IMU

          if (ultrasonic.inches() <= 4 || (flexLeft > 800 && flexRight < 800) || (flexLeft < 800 && flexRight > 800)) {
            //turn left
            //detect orientation with IMU
          }
          else if(ultrasonic.inches() >= 4 || (flexLeft > 800 && flexRight > 800)){
            motorMove(0, 128, countclkwise);
            motorMove(1, 128, clckwise);
            delay(//Determine size with or IMU
              );

            //turn left
            //detect orientation with IMU
            delay(20);
            motorMove(0 ,128, countclkwise);
            motorMove(1, 128, clckwise);

            goto break;
        }
        break;

        case -1:
        // turn left
        // detect orientation with IMU
        // spin 180
        // detect orientation with IMU

          if (ultrasonic <= 4 || (flexLeft > 800 && flexRight < 800) || (flexLeft < 800 && flexRight > 800)) {
            // turn left
            // detect orientation with IMU
          }
          else if(ultrasonic >= 4 || (flexLeft > 800 && flexRight > 800)){
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
        break;

        default:
        //If robot is not moving or able to move
        //Send alert to phone
        //Find way back 
        break;
      }
    }
     else if(ultrasonic.inches() >= 4 || (flexLeft > 800 && flexRight > 800)){
          motorDriveIncrement(5);
          //turn right
          //detect orientation with IMU
  }
}   

 /* ---- Function for incrementing motor drive ----*/
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

  motorMove(0, 128, 1);
  motorMove(0, 128, 0));
}


void armControl(){
  for (int i = 0; i < 180; i+20)
  {
    servoElbow.write(i);
    wait(1);
  }
}
