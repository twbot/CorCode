/* ----------------------------------- Header for flex sensor -------------------------------------*/

#ifndef FlexSense_h
#define FlexSense_h

class FlexSense {
public:
	uint8_t clear();
	uint8_t drop();
private:
	uint8_t flexPinL, flexPinR;
	uint16_t flexRight, flexLeft;
	void getBytes(uint8_t b, uint8_t s);
	uint16_t getFlexPosLeft();
	uint16_t getFlexPosRight();
	void setFlexPosLeft();
	void setFlexPosRight();
};

#endif	//FlexSense_h