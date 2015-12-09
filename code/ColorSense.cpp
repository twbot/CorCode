/* ----------------------------------- Class for color sensor -------------------------------------*/

#include <stdint.h>
#include "ColorSense.h"

uint16_t r,g,b;

void ColorSense::getColor() { 

  if (sensor_stringcomplete) {                                            //if a string from the color sensor has been received in its entierty 
    uint16_t commaIndex = sensorstring.indexOf(',');
    uint16_t secondCommaIndex = sensorstring.indexOf(',', commaIndex + 1);
    String f = sensorstring.substring(0, commaIndex);
    String s = sensorstring.substring(commaIndex + 1, secondCommaIndex);
    String t = sensorstring.substring(secondCommaIndex);
    r = f.toInt();
    g = s.toInt();
    b = t.toInt();
    Serial.println("Red: " + f + ", Green: " + s + ", Blue: " + t);
    //Serial.println(sensorstring);                                       //send that string to to the PC's serial monitor
    sensorstring = "";                                                    //clear the string:
    sensor_stringcomplete = false;                                        //reset the flag used to tell if we have received a completed string from the color sensor
  }
}

uint8_t ColorSense::yes(){
  if(r == 153 && g == 102 && b == 51){
    printf("Object Detected");
    return 1;
  }
  else if(r == 152 && g == 103 && b == 52){
    printf("Object Detected");
    return 1;
  }
  else if(r == 133 && g == 90 && b == 46){
    printf("Object Detected");
    return 1;
  }
  else if(r == 128 && g == 128 && b == 0){
    printf("Object Detected");
    return 1;
  }
  else if(r == 107 && g == 142 && b == 35){
    printf("Object Detected");
    return 1;
  }
  else if(r == 184 && g == 134 && b == 11){
    printf("Object Detected");
    return 1;
  }
  else if(r == 139 && g == 69 && b == 19){
    printf("Object Detected");
    return 1;
  }
  else if(r == 92 && g == 51 && b == 23){
    printf("Object Detected");
    return 1;
  }
}