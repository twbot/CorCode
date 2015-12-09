/* ----------------------------------- Class for motor driver -------------------------------------*/

#include "MotorDrive.h"

void MotorDrive::motorMove (int motor, int speed, int direction){

  /* 
  * If direction equal to 1, motor turns counter-clockwise
  * If direction equal to 0, motor turns clockwise
  * motor: 0 for A, 1 for B
  * speed: 0 for minimum, 255 for maximum
  */

  digitalWrite(STYBY, HIGH);
  
  if(direction == countclkwise){
   inPin1 = HIGH;
   inPin2 = LOW; 
  }
  if(motor == 1){
  digitalWrite(AIn1, inPin1);
  digitalWrite(AIn2, inPin2);
  analogWrite(motorASpeed, speed);
  }
  else{
  digitalWrite(BIn1, inPin1);
  digitalWrite(BIn2, inPin2);
  analogWrite(motorBSpeed, speed);
  }
}

/* ----------------------------------- Function to stop motors -------------------------------------*/
void MotorDrive::stop(){
  digitalWrite(STYBY, LOW);
}