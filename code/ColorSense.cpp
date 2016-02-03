/* ----------------------------------- Class for color sensor -------------------------------------*/

#include "ColorSense.h"                                               

void ColorSense::serialEvent3(){                                               //if the hardware serial port3 receives a char 
  inputChar = (uint8_t)Serial3.read();                                         //get the char we just received
  sensorString += inputChar;                                                   //add it to the input string
  if(inputChar == '\r') {                                                      //if the incoming character is a carriage return, set the flag 
    sensorStringComplete = 1;                                  
  }              
}

void ColorSense::getColor() { 
  serialEvent3();
  if (sensorStringComplete) {                                                 //when a string from the color sensor has been received in its entirety 
    commaIndex = sensorString.indexOf(',');
    commaIndex2 = sensorString.indexOf(',', commaIndex + 1);
    f = sensorString.substring(0, commaIndex);
    s = sensorString.substring(commaIndex + 1, commaIndex2);
    t = sensorString.substring(commaIndex2);
    sensorString = "";                                                         //clear the string
    sensorStringComplete = 0;                                                 //reset the flag
  }
}

void ColorSense::setColor(){
  getColor();
  r = f.toInt();
  g = s.toInt();
  b = t.toInt();
  Serial.println("Red: " + f + ", Green: " + s + ", Blue: " + t);     //TESTING; COMMENT OUT WHEN DONE
}

uint8_t ColorSense::yes(){
  setColor();
  if(r == 153 && g == 102 && b == 51){
    Serial.println("Object Detected");  //DELETE WHEN DONE
    return 1;
  }
  else if(r == 152 && g == 103 && b == 52){
    Serial.println("Object Detected");  //DELETE WHEN DONE
    return 1;
  }
  else if(r == 133 && g == 90 && b == 46){
    Serial.println("Object Detected");  //DELETE WHEN DONE
    return 1;
  }
  else if(r == 128 && g == 128 && b == 0){
    Serial.println("Object Detected");  //DELETE WHEN DONE
    return 1;
  }
  else if(r == 107 && g == 142 && b == 35){
    Serial.println("Object Detected");  //DELETE WHEN DONE
    return 1;
  }
  else if(r == 184 && g == 134 && b == 11){
    Serial.println("Object Detected");  //DELETE WHEN DONE
    return 1;
  }
  else if(r == 139 && g == 69 && b == 19){
    Serial.println("Object Detected");  //DELETE WHEN DONE
    return 1;
  }
  else if(r == 92 && g == 51 && b == 23){
    Serial.println("Object Detected");  //DELETE WHEN DONE
    return 1;
  }
}