#include <Servo.h>
#include <stdint.h>
#include <log.h>
#include <Wire.h>
#include <avr/interrupt.h>

//#include "Accel.h"
//#include "Gyro.h"
#include "ColorSense.h"
#include "UltraSonicDetect.h"
#include "GasSense.h"
//#include "FlexSense.h"
#include "Communication.h"
#include "InductiveSense.h"

/* ------------ DO NOT INSERT INTO DIGITAL PINS 0, 1, 14, 15, 16, 17 ---------------*/
/* ------------ DELETE ALL SERIAL.BEGIN STATEMENTS; TOO MUCH MEMORY USAGE (EXCEPTION: BAUD RATE 9600) ----------------*/
/* ------------ INTERRUPT PINS FOR ATMEGA2560; INT0: 2(1), INT1: 3(2), INT2: 21(3), INT3: 22(4), INT4: 19(5), INT5: 18(6) ---------------*/

/* -- Flex Pins (Analog) --*/
//const uint8_t flexPinLeft = 0;
//const uint8_t flexPinRight = 1;

/* -- Gas sensor (Analog) --*/
const uint8_t gasSensor = 2;

/* -- Inductance Sensor (Analog) -- */
const uint8_t indSensor = 3;

/* -- MAG (Analog) --*/
const uint8_t SDA = 20;
const uint8_t SCL = 21;

/* -- ColorSense (Digital) --*/
const uint8_t colSensor = 9;

/* -- Touch Button (Digital) -- */
const uint8_t button = 10;

/* -- Ultrasonic (Digital) --*/
const uint8_t objDOutput = 8;
const uint8_t objDInput = 7;

/* -- Motor A (Digital) --*/
const uint8_t motorASpeed = 5;                                                 //pwm
const uint8_t AIn1 = 23;                                                        
const uint8_t AIn2 = 24;

/* -- Motor B (Digital) --*/
const uint8_t motorBSpeed = 6;                                                 //pwm
const uint8_t BIn1 = 25;                                                        
const uint8_t BIn2 = 26;

/* -- Motor Standby/off (Digital) --*/
const uint8_t STYBY = 27;

/* -- LED (Digital) --*/
const uint8_t led = 28;

/* -- Edge Detect -- */
const uint8_t iR;
const uint8_t oR;
const uint8_t iL;
const uint8_t oL;

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
UltraSonicDetect ultra = UltraSonicDetect();
GasSense gasSense = GasSense();
ColorSense colorSense = ColorSense();
//FlexSense flexSense = FlexSense();
InductiveSense induct = InductiveSense();
Magnometer mag = Magnometer();

void setup() {

  Serial.begin(38400);    //For debugging purposes; DELETE WHEN DONE
  Serial1.begin(9600);    
  Serial3.begin(38400);   
  Serial.begin(115200);   //For debugging purposes; DELETE WHEN DONE

  Wire.begin();

  //Color Sensor
  pinMode(colSensor, INPUT);

  // Object Detect
  pinMode(objDOutput, OUTPUT);
  pinMode(objDInput, INPUT);

  //Edge Detect (Right)
  pinMode(edjDLOutput, OUTPUT);
  pinMode(edjDLOutput, OUTPUT);

  //Edge Detect (Left)
  pinMode(edjDROutput, OUTPUT);
  pinMode(edjDRInput, INPUT);
  
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

  //Initialize magnometer
  mag.init();

  //Sensor Pin Receive
  induct.getByte(indSensor);
  ultra.getBytes(objDOutput, objDInput);
  edge.getBytes( ir, oR, iL, oL);
}

void ISR::colorDetect(){
  color.yes();
  stop();
}

ISR(INT0_vect){
  if (edgeDetect.drop()){
    stop();
  }
}

ISR(INT1_vect){
  if(color.yes()){
    stop();
    //INSERT ARM CONTROL HERE
  }
}
//CREATE INTERRUPT FOR WHEN ROBOT DETECTS OBJECT

while(1){

/*  if(color.yes() ){
    stop();
    armControl();
  }
*/
  for(uint8_t y : arr){
    x = y;

    if (ultra.barrier() || edge.drop() || ){

      switch (x) {
        case 1:
        //turn right
        //detect orientation with MAG
        //spin 180
        //detect orientation with MAG

          if (ultra.barrier() || edge.drop()) {
            //turn left
            //detect orientation with MAG
          }
          else if (ultra.clear() || edge.clear()){
            motorMove(0, 128, countclkwise);
            motorMove(1, 128, clckwise);
            delay(//Determine size with or MAG
              );

            //turn left
            //detect orientation with MAG
            delay(20);
            motorMove(0 ,128, countclkwise);
            motorMove(1, 128, clckwise);

            goto break;
        }
        break;

        case -1:
        // turn left
        // detect orientation with MAG
        // spin 180
        // detect orientation with MAG

          if (ultra.barrier()|| edge.drop();) {
            // turn left
            // detect orientation with MAG
          }
          else if(ultra.clear() || edge.clear()){
            motorMove(0, 128, countclkwise);
            motorMove(1, 128, clckwise);
            delay(// Determine size with MAG
              );

            // turn left
            // detect orientation with MAG
            delay(20);
            motorMove(0 ,128, countclkwise);
            motorMove(1, 128, clckwise);

            goto break;
        }
        break;

        default:
        //If robot is not moving or able to move
        //Send alert to phone
        //Find way back to charging station
        break;
      }
    }
     else if(ultra.clear() || edge.clear()){
          motorDriveIncrement(5);
          //turn right
          //detect orientation with MAG
  }
}   

 /* ---- Function for incrementing motor drive ----*/
/* ----- TRANSFER -----*/
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

/* ------PUT IN OWN CLASS && CHANGE ------*/

void armControl(){
  for (int i = 0; i < 180; i+20)
  {
    servoElbow.write(i);
    wait(1);
  }
}
