//
//  flexSense.c
//  COR
//
//  Created by Tristan Brodeur on 10/18/15.
//
//

#include "flexSense.h"


void start(){
//Back up
motorMove(0, 128, 0);
motorMove(1, 128, 1);

//Spin 180
motorMove(0, 128, 0);
motorMove(1, 128, 0);
//detect orientation with accelerometer

}


void main(){
    
while(ultraSonicDetect >= 4) {
    motorMove(0, 128, 1);
    motorMove(1, 128, 0);
}

 
void action(){
    if (ultraSonicDetect <= 4){
    //turn right
    //detect orientation with accelerometer
    
    if (ultraSonicDetect <= 4) {
        //spin 180
        //detect orientation with accelerometer
            
        //set x = -1
        x = -1;
            
        if (ultraSonicDetect <= 4) {
            //turn left
            
            }
        else if(ultraSonicDetect >= 4){
            motorMove(0, 128, 1);
            motorMove(1, 128, 0);
            delay(//Determine size);
            
            //turn left
            motorMove(0 ,128, 1);
            motorMove(1, 128, 0);
            
            }
        }
        
     else if(ultraSonicDetect >= 4){
        motorMove(0, 128, 1);
        motorMove(1, 128, 0);
        delay(//Determine size);
        
        //Set x = 1;
        //turn right
              while(ultraSonicDetect >= 4){
                  motorMove(0, 128, 1);
                  motorMove(1, 128, 0);
              }
        }
}
              }


int ultraSonicDetect(){
    digitalWrite(ultraSonicOutput, LOW);
    delayMicroseconds(2);
    digitalWrite(ultraSonicOutput, HIGH);
    delayMicroseconds(5);
    digitalWrite(ultraSonicOutput, LOW);
    
    duration = pulseIn(ultraSonicInput, HIGH);
    inches = microSecondsToInches(duration);
    
    Serial.print("Inches: ");
    Serial.print(inches);
    
    return inches;
}


