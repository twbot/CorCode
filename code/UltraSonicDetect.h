/* ----------------------------------- Header for ultrasonic sensor ------------------------------------- */

#include <stdint.h>

#ifndef  UltraSonicDetect
#define UltraSonicDetect 

class UltraSonicDetect{
public: 
	uint8_t ultraSonicOutput;
	uint8_t ultraSonicInput;
	uint16_t inches();
private:
	uint16_t inches;
};

#endif	// UltraSonicDetect_h