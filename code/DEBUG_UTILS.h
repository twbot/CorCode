#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#ifdef 
#define DEBUG_PRINT(x) do{
	Serial.print(__FUNCTION__);
	Serial.print(" : ");
	Serial.print(__LINE__);
	Serial.print(" : ");
} while(0)
#endif