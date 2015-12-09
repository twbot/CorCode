/* ----------------------------------- Class for ultrasonic sensor -------------------------------------*/

#include "FlexSense.h"

uint16_t flexPosRight;
uint16_t flexPosLeft;
void flexSense(){
  flexPosLeft = analogRead(flexPinLeft);
  flexPosLeft = constrain(flexPosLeft, 750, 840);

  flexPosRight = analogRead(flexPinRight);
  flexPosRight = constrain(flexPosRight, 750, 840);

  Serial1.print("Flex Sensor Left:  " + flexPosLeft);
  Serial1.print("Flex Sensor Right:  " + flexPosRight); 
}