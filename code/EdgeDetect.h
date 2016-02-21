
#ifndef EdgeDetect
#define EdgeDetect 
class EdgeDetect{
	uint8_t outputR, outputL, inputR, inputL;
	getBytes();
	getEdgeRight();
	getEdgeLeft();
	setEdgeRight();
	setEdgeLeft();
public:
	volatile uint8_t uRight, uLeft;
	getBytes(uint8_t a, b, c, d);
	drop();
	clear();
};
#endif	//EdgeDetect_h


//Distance = ((Duration of high level)*(Sonic :340m/s))/2