//
//  RobotControl.c
//  
//
//  Created by Tristan Brodeur on 10/22/15.
//
//

#include <stdio.h>
#include <string>

int x = 0;
int arr[] = {-1, 1};

int main(){

for(int i = 0; i <10; i++){
    for(int y : arr){
        x = y;
    switch (x) {
        case 1:
        printf("x = %d\n", x);
            /*
            if (ultraSonicDetect <= 4 && IRDetect == false){
                //turn right
                //detect orientation with IMU
                
                if (ultraSonicDetect <= 4 && IRDetect = false) {
                    //spin 180
                    //detect orientation with IMU
                    
                    if (ultraSonicDetect <= 4 && IRDetect = false) {
                        //turn left
                        //detect orientation with IMU
                    }
                    else if(ultraSonicDetect >= 4 && IRDetect = true){
                        motorMove(0, 128, 1);
                        motorMove(1, 128, 0);
                        delay(//Determine size with odometry( or IMU)
                              );
                        
                        //turn left
                        //detect orientation with IMU
                        delay(20);
                        motorMove(0 ,128, 1);
                        motorMove(1, 128, 0);
                        
                        goto break;
                        
                    }
                }
                else if(ultraSonicDetect >= 4 && IRDetect = true){
                    motorMove(0, 128, 1);
                    motorMove(1, 128, 0);
                    delay(//Determine size with odometry ( or IMU)
                          );
                    
                    //turn right
                    //detect orientation with IMU
                }
            }
            */
            break;
        case -1:
        printf("x =  %d\n", x);
        /*
            if (ultraSonicDetect <= 4 && IRDetect == false){
                //turn left
                //detect orientation with IMU
                
                if (ultraSonicDetect <= 4 && IRDetect = false) {
                    //spin 180
                    //detect orientation with IMU
                    
                    if (ultraSonicDetect <= 4 && IRDetect = false) {
                        //turn left
                        //detect orientation with IMU
                    }
                    else if(ultraSonicDetect >= 4 && IRDetect = true){
                        motorMove(0, 128, 1);
                        motorMove(1, 128, 0);
                        delay(//Determine size with odometry( or IMU)
                              );
                        
                        //turn left
                        //detect orientation with IMU
                        delay(20);
                        motorMove(0 ,128, 1);
                        motorMove(1, 128, 0);
                        
                        break action;
                        
                    }
                }
                else if(ultraSonicDetect >= 4 && IRDetect = true){
                    motorMove(0, 128, 1);
                    motorMove(1, 128, 0);
                    delay(//Determine size with odometry ( or IMU)
                          );
                    //turn right
                    //detect orientation with IMU
                }
            }
            */
            break;

        default:
            printf("Error in code");
            break;
        }
    }  
}
}

                