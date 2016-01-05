/* ----------------------------------- Header for color sensor -------------------------------------*/
#include <stdint.h>

#ifndef ColorSense_h
#define ColorSense_h

class ColorSense{
public:
	void getColor();
	uint8_t yes();
private:
	uint8_t inputChar;
	uint8_t sensorStringComplete;
	uint16_t r,g,b;
	uint16_t commaIndex, commaIndex2;
	String f,s,t;
	String sensorString = ""; 
	void setColor();
};

#endif //ColorSense_h