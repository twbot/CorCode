/* ----------------------------------- Class for flex sensor -------------------------------------*/

#include "FlexSense.h"

uint16_t FlexSense::getFlexSenseLeft(){
  uint16_t flexPos = analogRead(flexPinLeft);
  flexPos = constrain(flexPos, 750, 840);
  Serial1.println("Flex Sensor Left: " + flexPos);	  //For debugging purposes; DELETE WHEN DONE
  return flexPos;
}

uint16_t FlexSense::getFlexSenseRight(){
  uint16_t flexPos = analogRead(flexPinRight);
  flexPos = constrain(flexPos, 750, 840);
  Serial1.println("Flex Sensor Right: " + flexPos);	  //For debugging purposes; DELETE WHEN DONE
  return flexPos;
}

uint16_t FlexSense::setFlexSenseRight(){
	flexRight = getFlexSenseRight();
}

uint16_t FlexSense::setFlexSenseLeft(){
	flexLeft = getFlexSenseLeft();
}