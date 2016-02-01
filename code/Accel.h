/* ----------------------------------- Header for Accelerometer -------------------------------------*/
#include "Arduino.h"

#ifndef Accel_h
#define Accel_h

/* -- Accelerometer addresses --*/
#define Accel_ADDR_ALT_HIGH 0x1D // Accelerometer address when ALT is connected to HIGH
#define Accel_ADDR_ALT_LOW 0x53 // Accelerometer address when ALT is connected to LOW

/* ------- Register names ------- */
#define Accel_ID 0x00
#define Accel_RESERVED1 0x01
#define Accel_THRESH_TAP 0x1d
#define Accel_OFFSTX 0x1e
#define Accel_OFFSTY 0x1f
#define Accel_OFFSTZ 0x20
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

//DEBUGGING; COMMENT OUT WHEN DONE
#define Accel_OK  1 // no error
#define Accel_ERROR 0 // indicates error is present
#define Accel_NO_ERROR  0 // initial state
#define Accel_READ_ERROR 1 // problem reading accelerometer
#define Accel_BAD_ARG  2 // bad method argument

class Accel{
public:
 uint8_t status;      // set when error occurs
 uint8_t errorCode;    // Initial state
 uint32_t gains[3];    // counts to Gs

 Accel();
 void init(uint16_t address);
 void powerOn();
 void readAccel(uint16_t* xyx);
 void readAccel(uint16_t* x, uint16_t* y, uint16_t* z);
 void get_Gxyz(uint32_t* xyz);

 void setTapThreshold(uint16_t tapThreshold);
 uint16_t getTapThreshold();
 void setAxisGains(uint32_t* gains);
 void getAxisGains(uint32_t* gains);
 void setAxisOffset(uint16_t x, uint16_t y, uint16_t z);
 void getAxisOffset(uint16_t* x, uint16_t* y, uint16_t* z);
 void setTapDuration(uint16_t tapDuration);
 uint16_t getTapDuration();
 void setDoubleTapLatency(uint16_t floatTapLatency);
 uint16_t getDoubleTapLatency();
 void setDoubleTapWindow(uint16_t floatTapWindow);
 uint16_t getDoubleTapWindow();
 void setActivityThreshold(uint16_t activityThreshold);
 uint16_t getActivityThreshold();
 void setInactivityThreshold(uint16_t inactivityThreshold);
 uint16_t getInactivityThreshold();
 void setTimeInactivity(uint16_t timeInactivity);
 uint16_t getTimeInactivity();
 void setFreeFallThreshold(uint16_t freeFallthreshold);
 uint16_t getFreeFallThreshold();
 void setFreeFallDuration(uint16_t freeFallDuration);
 uint16_t getFreeFallDuration();

 uint8_t isActivityXEnabled();
 uint8_t isActivityYEnabled();
 uint8_t isActivityZEnabled();
 uint8_t isInactivityXEnabled();
 uint8_t isInactivityYEnabled();
 uint8_t isInactivityZEnabled();
 uint8_t isActivityAc();
 uint8_t isInactivityAc();
 void setActivityAc(uint8_t state);
 void setInactivityAc(uint8_t state);

 uint8_t getSuppressBit();
 void setSuppressBit(uint8_t state);
 uint8_t isTapDetectionOnX();
 void setTapDetectionOnX(uint8_t state);
 uint8_t isTapDetectionOnY();
 void setTapDetectionOnY(uint8_t state);
 uint8_t isTapDetectionOnZ();
 void setTapDetectionOnZ(uint8_t state);

 void setActivityX(uint8_t state);
 void setActivityY(uint8_t state);
 void setActivityZ(uint8_t state);
 void setInactivityX(uint8_t state);
 void setInactivityY(uint8_t state);
 void setInactivityZ(uint8_t state);

 uint8_t isActivitySourceOnX();
 uint8_t isActivitySourceOnY();
 uint8_t isActivitySourceOnZ();
 uint8_t isTapSourceOnX();
 uint8_t isTapSourceOnY();
 uint8_t isTapSourceOnZ();
 uint8_t isAsleep();

 uint8_t isLowPower();
 void setLowPower(uint8_t state);
 uint32_t getRate();
 void setRate(uint32_t rate);
 void set_bw(uint8_t bwCode);
 uint8_t getBwCode(); 

 uint8_t getInterruptSource();
 uint8_t getInterruptSource(uint8_t interruptBit);
 uint8_t getInterruptMapping(uint8_t interruptBit);
 void setInterruptMapping(uint8_t interruptBit, uint8_t interruptPin);
 uint8_t isInterruptEnabled(uint8_t interruptBit);
 void setInterrupt(uint8_t interruptBit, uint8_t state);

 void getRangeSetting(uint8_t* rangeSetting);
 void setRangeSetting(uint16_t val);
 uint8_t getSelfTestBit();
 void setSelfTestBit(uint8_t selfTestBit);
 uint8_t getSpiBit();
 void setSpiBit(uint8_t spiBit);
 uint8_t getInterruptLevelBit();
 void setInterruptLevelBit(uint8_t interruptLevelBit);
 uint8_t getFullResBit();
 void setFullResBit(uint8_t fullResBit);
 uint8_t getJustifyBit();
 void setJustifyBit(uint8_t justifyBit);
 void printAllRegister();
 void writeTo(uint8_t address, uint8_t val);

private:
 void readFrom(uint8_t address,uint16_t num, uint8_t buff[]);
 void setRegisterBit(uint8_t regAdress,uint16_t bitPos, uint8_t state);
 uint8_t getRegisterBit(uint8_t regAdress,uint16_t bitPos); 
 uint8_t buff[6] ;  //6 uint8_ts buffer for saving data read from the device
 uint8_t devAddress;
};
void print_uint8_t(uint8_t val);

#endif	// Accel_h