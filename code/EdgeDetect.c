#include "EdgeDetect.h"

EdgeDetect::getBytes(a, b, c, d){
	inputR = a;
	outputR = b;
	inputL = c;
	outputL = d;
}

uint8_t EdgeDetect::getEdgeRight(){
	digitalWrite(outputR, HIGH);
	delay(60);
	digitalWrite(outputR, LOW);
	delay(60);

	uint16_t read = digitalRead(inputR, HIGH);
	uint8_t meters = msToMeters(read);
	return meters;
}

uint8_t EdgeDetect::getEdgeLeft(){
	digitalWrite(outputL, HIGH);
	delay(60);
	digitalWrite(outputL, LOW);
	delay(60);

	uint16_t read = digitalRead(inputL, HIGH);
	uint8_t meters = msToMeters(read);
	return meters;
}

void EdgeDetect::setEdgeRight(){
	uRight = getEdgeRight();
}

void EdgeDetect::setEdgeLeft(){
	uLeft = getEdgeLeft();
}

uint8_t EdgeDetect::drop(){
	if(uRight <= 4  || uLeft <= 4){
		return 1;
	}
}

uint8_t EdgeDetect::clear(){
	if (uRight >= 4 || uLeft >= 4){
		return 1;
	}
}


