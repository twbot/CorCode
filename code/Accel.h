/* ----------------------------------- Header for Accelerometer -------------------------------------*/
#include "Arduino.h"

#ifndef Accel_h
#define Accel_h

/* -- Accelerometer addresses --*/
#define Accel_ADDR_ALT_HIGH 0x1D // Accelerometer address when ALT is connected to HIGH
#define Accel_ADDR_ALT_LOW 0x53 // Accelerometer address when ALT is connected to LOW

/* ------- Register names ------- */
#define Accel_DEVID 0x00
#define Accel_RESERVED1 0x01
#define Accel_THRESH_TAP 0x1d
#define Accel_OFSX 0x1e
#define Accel_OFSY 0x1f
#define Accel_OFSZ 0x20
#define Accel_DUR 0x21
#define Accel_LATENT 0x22
#define Accel_WINDOW 0x23
#define Accel_THRESH_ACT 0x24
#define Accel_THRESH_INACT 0x25
#define Accel_TIME_INACT 0x26
#define Accel_ACT_INACT_CTL 0x27
#define Accel_THRESH_FF 0x28
#define Accel_TIME_FF 0x29
#define Accel_TAP_AXES 0x2a
#define Accel_ACT_TAP_STATUS 0x2b
#define Accel_BW_RATE 0x2c
#define Accel_POWER_CTL 0x2d
#define Accel_INT_ENABLE 0x2e
#define Accel_INT_MAP 0x2f
#define Accel_INT_SOURCE 0x30
#define Accel_DATA_FORMAT 0x31
#define Accel_DATAX0 0x32
#define Accel_DATAX1 0x33
#define Accel_DATAY0 0x34
#define Accel_DATAY1 0x35
#define Accel_DATAZ0 0x36
#define Accel_DATAZ1 0x37
#define Accel_FIFO_CTL 0x38
#define Accel_FIFO_STATUS 0x39

#define Accel_BW_1600 0xF // 1111
#define Accel_BW_800 0xE // 1110
#define Accel_BW_400 0xD // 1101 
#define Accel_BW_200 0xC // 1100
#define Accel_BW_100 0xB // 1011 
#define Accel_BW_50  0xA // 1010 
#define Accel_BW_25  0x9 // 1001 
#define Accel_BW_12  0x8 // 1000 
#define Accel_BW_6  0x7 // 0111
#define Accel_BW_3  0x6 // 0110

/* ------- Interrup Pins ------- */
//INT1: 0
//INT2: 1
#define Accel_INT1_PIN 0x00
#define Accel_INT2_PIN 0x01

/* ------- Interrupt Bit Position ------- */
#define Accel_INT_DATA_READY_BIT 0x07
#define Accel_INT_SINGLE_TAP_BIT 0x06
#define Accel_INT_DOUBLE_TAP_BIT 0x05
#define Accel_INT_ACTIVITY_BIT  0x04
#define Accel_INT_INACTIVITY_BIT 0x03
#define Accel_INT_FREE_FALL_BIT 0x02
#define Accel_INT_WATERMARK_BIT 0x01
#define Accel_INT_OVERRUNY_BIT  0x00

#define Accel_OK  1 // no error
#define Accel_ERROR 0 // indicates error is present
#define Accel_NO_ERROR  0 // initial state
#define Accel_READ_ERROR 1 // problem reading accelerometer
#define Accel_BAD_ARG  2 // bad method argument

class Accel{
public:
 bool status;      // set when error occurs
 byte error_code;    // Initial state
 float gains[3];    // counts to Gs

 Accel();
 void init(int address);
 void powerOn();
 void readAccel(int* xyx);
 void readAccel(int* x, int* y, int* z);
 void get_Gxyz(float *xyz);

 void setTapThreshold(int tapThreshold);
 int getTapThreshold();
 void setAxisGains(float *_gains);
 void getAxisGains(float *_gains);
 void setAxisOffset(int x, int y, int z);
 void getAxisOffset(int* x, int* y, int*z);
 void setTapDuration(int tapDuration);
 int getTapDuration();
 void setDoubleTapLatency(int floatTapLatency);
 int getDoubleTapLatency();
 void setDoubleTapWindow(int floatTapWindow);
 int getDoubleTapWindow();
 void setActivityThreshold(int activityThreshold);
 int getActivityThreshold();
 void setInactivityThreshold(int inactivityThreshold);
 int getInactivityThreshold();
 void setTimeInactivity(int timeInactivity);
 int getTimeInactivity();
 void setFreeFallThreshold(int freeFallthreshold);
 int getFreeFallThreshold();
 void setFreeFallDuration(int freeFallDuration);
 int getFreeFallDuration();

 bool isActivityXEnabled();
 bool isActivityYEnabled();
 bool isActivityZEnabled();
 bool isInactivityXEnabled();
 bool isInactivityYEnabled();
 bool isInactivityZEnabled();
 bool isActivityAc();
 bool isInactivityAc();
 void setActivityAc(bool state);
 void setInactivityAc(bool state);

 bool getSuppressBit();
 void setSuppressBit(bool state);
 bool isTapDetectionOnX();
 void setTapDetectionOnX(bool state);
 bool isTapDetectionOnY();
 void setTapDetectionOnY(bool state);
 bool isTapDetectionOnZ();
 void setTapDetectionOnZ(bool state);

 void setActivityX(bool state);
 void setActivityY(bool state);
 void setActivityZ(bool state);
 void setInactivityX(bool state);
 void setInactivityY(bool state);
 void setInactivityZ(bool state);

 bool isActivitySourceOnX();
 bool isActivitySourceOnY();
 bool isActivitySourceOnZ();
 bool isTapSourceOnX();
 bool isTapSourceOnY();
 bool isTapSourceOnZ();
 bool isAsleep();

 bool isLowPower();
 void setLowPower(bool state);
 float getRate();
 void setRate(float rate);
 void set_bw(byte bw_code);
 byte get_bw_code(); 

 byte getInterruptSource();
 bool getInterruptSource(byte interruptBit);
 bool getInterruptMapping(byte interruptBit);
 void setInterruptMapping(byte interruptBit, bool interruptPin);
 bool isInterruptEnabled(byte interruptBit);
 void setInterrupt(byte interruptBit, bool state);

 void getRangeSetting(byte* rangeSetting);
 void setRangeSetting(int val);
 bool getSelfTestBit();
 void setSelfTestBit(bool selfTestBit);
 bool getSpiBit();
 void setSpiBit(bool spiBit);
 bool getInterruptLevelBit();
 void setInterruptLevelBit(bool interruptLevelBit);
 bool getFullResBit();
 void setFullResBit(bool fullResBit);
 bool getJustifyBit();
 void setJustifyBit(bool justifyBit);
 void printAllRegister();
 void writeTo(byte address, byte val);

private:
 void readFrom(byte address, int num, byte buff[]);
 void setRegisterBit(byte regAdress, int bitPos, bool state);
 bool getRegisterBit(byte regAdress, int bitPos); 
 byte _buff[6] ;  //6 bytes buffer for saving data read from the device
 int _dev_address;
};
void print_byte(byte val);

#endif	// Accel_h