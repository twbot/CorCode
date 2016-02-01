/* ----------------------------------- Class for flex sensor -------------------------------------*/

#include "FlexSense.h"


void FlexSense::getBytes(uint8_t b, uint8_t s){
  flexPinL = b;
  flexPinR = s; 
}

uint16_t FlexSense::getFlexSenseLeft(){
  uint16_t flexPos = analogRead(flexPinL);
  flexPos = constrain(flexPos, 750, 840);
  Serial1.println("Flex Sensor Left: " + flexPos);	  //For debugging purposes; DELETE WHEN DONE
  return flexPos;
}

uint16_t FlexSense::getFlexSenseRight(){
  uint16_t flexPos = analogRead(flexPinR);
  flexPos = constrain(flexPos, 750, 840);
  Serial1.println("Flex Sensor Right: " + flexPos);	  //For debugging purposes; DELETE WHEN DONE
  return flexPos;
}

void FlexSense::setFlexSenseRight(){
	flexRight = getFlexSenseRight();
}

void FlexSense::setFlexSenseLeft(){
	flexLeft = getFlexSenseLeft();
}

uint8_t FlexSense::clear(){
  setFlexSenseLeft();
  setFlexSenseRight();
  if(flexRight > 800 && flexLeft > 800) {
    return 1;
  }
}

uint8_t FlexSense::drop(){
  setFlexSenseRight();
  setFlexSenseLeft();
  if(flexRight < 800 || flexLeft < 800) {
    return 1;
  }
}