/* ----------------------------------- Class for ultrasonic sensor ------------------------------------- */
#include "UltraSonicDetect.h"

uint16_t UltraSonicDetect::m() {
	digitalWrite(ultraSonicOutput, LOW);
	delayMicroseconds(2);
	digitalWrite(ultraSonicOutput, HIGH);
	delayMicroseconds(5);
	digitalWrite(ultraSonicOutput, LOW);

	duration = pulseIn(ultraSonicInput, HIGH);
	m = msToMeters(duration);

	Serial.print("Meters: ");	//DELETE WHEN DONE
	Serial.print(m);		//DELETE WHEN DONE

	return m;
}

uint16_t UltraSonicDetect::msToMeters(uint16_t ms) {
  return ms / 73.746 / 2;
  //return ms /  
}

uint8_t UltraSonicDetect::barrier() {
	m();
	if(m <= 4){
		return 1;
	}
}

uint8_t UltraSonicDetect::clear(){
	m();
	if(m >= 4) {
		return 1;
	}
}