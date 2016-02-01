#include "Induct.h"

void Induct::getByte(uint8_t b){
	inductor = b;
}

void Induct::sense(){
	uint16_t inductRead = analogRead(inductor);
	Serial.print("Value = ");		//DEBUGGING; DELETE WHEN DONE
	Serial.print(inductRead);		//DEBUGGING; DELETE WHEN DONE
	value = inductRead;
}

void Induct::barrier(){
	//FIND INDUCTANCE READINGS
}