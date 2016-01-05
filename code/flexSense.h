/* ----------------------------------- Header for flex sensor -------------------------------------*/

#ifndef FlexSense_h
#define FlexSense_h

class FlexSense{
public:
	uint16_t getFlexPosLeft();
	uint16_t getFlexPosRight();
	uint16_t flexRight, flexLeft;
private:
	uint16_t setFlexPosLeft();
	uint16_t setFlexPosRight();
};

#endif	//FlexSense_h