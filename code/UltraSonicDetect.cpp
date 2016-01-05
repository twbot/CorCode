/* ----------------------------------- Class for ultrasonic sensor ------------------------------------- */
#include "UltraSonicDetect.h"

uint8_t ultraSonicInput, ultraSonicOutput;
uint16_t inches, duration;

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

/* ---- Function to convert microseconds to inches ---*/
uint16_t microSecondsToInches(uint16_t microseconds) {
  return microseconds / 73.746 / 2;
}
