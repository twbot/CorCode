#ifndef  MotorDrive
#define MotorDrive 

class MotorDrive{
public:
	void motorMove(int motor, int speed, int direction);
	void stop();
};

#endif