
#ifndef Magnometer
#define Magnometer 

#define ADDRESS 0x1E
#define CONFIG_REG_A 0x00
#define CONFIG_REG_B 0x01
#define MOD_REG 0x02
#define DAT_OUT_X_MSB 0x03
#define DAT_OUT_X_LSB 0x04
#define DAT_OUT_Z_MSB 0x05
#define DAT_OUT_Z_LSB 0x06
#define DAT_OUT_Y_MSB 0x07
#define DAT_OUT_Y_LSB 0x08

#define MAG_OK 1
#define MAG_ERROR 0

class Magnometer
{
	getAxis();
public:
};

#endif	//Magnometer_h