#include "Induct.h"

void Induct::Induct(uint8_t b){
	inductor = b;
}

void Induct::sense(){
	uint16_t inductRead = analogRead(inductor);
	Serial.print("Value = ");		//TESTING; COMMENT OUT WHEN DONE
	Serial.print(inductRead);		//TESTING; COMMENT OUT WHEN DONE
	value = inductRead;
}

uint8_t Induct::barrier(){
	//FIND INDUCTANCE READINGS
}

uint8_t Induct::clear(){
	//FIND INDUCTANCE READINGS
}
