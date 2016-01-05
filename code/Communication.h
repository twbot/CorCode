/* ----------------------------------- Header for Communication with IMU; DEBUGGING: DELETE WHEN DONE-------------------------------------*/

#ifndef Communication_h

#include "Arduino.h"

#define Communitation_h

void serialPrintFloatArr(float * arr, int length);
void serialFloatPrint(float f);
void writeArr(void * arr, uint8_t arr_length, uint8_t type_bytes);
void writeVar(void * val, uint8_t type_bytes);

#endif	// Communication_h