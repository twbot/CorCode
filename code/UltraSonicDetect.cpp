/* ----------------------------------- Class for ultrasonic sensor ------------------------------------- */
#include "UltraSonicDetect.h"

uint16_t UltraSonicDetect::inches() {
	digitalWrite(ultraSonicOutput, LOW);
	delayMicroseconds(2);
	digitalWrite(ultraSonicOutput, HIGH);
	delayMicroseconds(5);
	digitalWrite(ultraSonicOutput, LOW);

	duration = pulseIn(ultraSonicInput, HIGH);
	inches = microSecondsToInches(duration);

	Serial.print("Inches: ");	//DELETE WHEN DONE
	Serial.print(inches);		//DELETE WHEN DONE

	return inches;
}

uint16_t UltraSonicDetect::microSecondsToInches(uint16_t microseconds) {
  return microseconds / 73.746 / 2;
}

uint8_t UltraSonicDetect::barrier() {
	inches();
	if(inches <= 4){
		return 1;
	}
}

uint8_t UltraSonicDetect::clear(){
	inches();
	if(inches >= 4) {
		return 1;
	}
}