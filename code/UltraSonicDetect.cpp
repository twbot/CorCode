/* ----------------------------------- Class for ultrasonic sensor ------------------------------------- */
#include "UltraSonicDetect.h"

uint8_t UltraSonicDetect::getBytes(uint8_t b, uint8_t s){
	ultraOutput = b;
	ultraInput = s;
}
uint16_t UltraSonicDetect::m() {
	digitalWrite(ultraOutput, LOW);
	delayMicroseconds(2);
	digitalWrite(ultraOutput, HIGH);
	delayMicroseconds(5);
	digitalWrite(ultraOutput, LOW);

	duration = pulseIn(ultraInput, HIGH);
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