/* ----------------------------------- Class for ultrasonic sensor -------------------------------------*/
#include "UltraSonic.h"

uint8_t UltraSonicDetect::inches() {
	uint8_t inches;
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