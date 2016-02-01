/* ----------------------------------- Header for ultrasonic sensor ------------------------------------- */

#include <stdint.h>

#ifndef  UltraSonicDetect
#define UltraSonicDetect 

class UltraSonicDetect{
public: 
	uint8_t ultraSonicInput, ultraSonicOutput;
	uint8_t barrier();
	uint8_t clear();
private:
	uint16_t inches, duration;
	uint16_t inches();
};

#endif	// UltraSonicDetect_h