
#ifndef InductiveSense_h
#define InductiveSense_h

class InductiveSense {
public:
	uint8_t clear();
	uint8_t barrier();
private:
	uint8_t inductor;
	uint16_t value;
	getByte(uint8_t b);
};

#endif	//InductiveSense_h