/****************************************************************************
* Tested on Arduino Mega with ITG-3200 Breakout               *
* SCL   -> pin 21   (no pull up resistors)               *
* SDA   -> pin 20   (no pull up resistors)               *
* CLK & GND -> pin GND                          *
* INT    -> not connected (but can be used)               *
* VIO & VDD -> pin 3.3V                           *
*****************************************************************************/
#ifndef Gyro_h
#define Gyro_h

#include "Arduino.h"
#include <Wire.h>

#define Gyro_ADDR_AD0_HIGH 0x69  //AD0=1 0x69 I2C address when AD0 is connected to HIGH (VCC) - default for sparkfun breakout
#define Gyro_ADDR_AD0_LOW  0x68  //AD0=0 0x68 I2C address when AD0 is connected to LOW (GND)
// "The LSB bit of the 7 bit address is determined by the logic level on pin 9. 
// This allows two ITG-3200 devices to be connected to the same I2C bus.
// One device should have pin9 (or bit0) LOW and the other should be HIGH." source: ITG3200 datasheet
// Note that pin9 (AD0 - I2C Slave Address LSB) may not be available on some breakout boards so check 
// the schematics of your breakout board for the correct address to use.


#define GYROSTART_UP_DELAY 70  // 50ms from gyro startup + 20ms register r/w startup

/* ---- Registers ---- */
#define WHO_AM_I  0x00 // RW  SETUP: I2C address  
#define SMPLRT_DIV  0x15 // RW  SETUP: Sample Rate Divider    
#define DLPF_FS  0x16 // RW  SETUP: Digital Low Pass Filter/ Full Scale range
#define INT_CFG  0x17 // RW  Interrupt: Configuration
#define INT_STATUS	0x1A // R  Interrupt: Status
#define TEMP_OUT	0x1B // R  SENSOR: Temperature 2 bytes
#define GYRO_XOUT	0x1D // R  SENSOR: Gyro X 2 bytes 
#define GYRO_YOUT	0x1F // R  SENSOR: Gyro Y 2 bytes
#define GYRO_ZOUT	0x21 // R  SENSOR: Gyro Z 2 bytes
#define PWR_MGM	0x3E // RW Power Management

/* ---- Bit Maps ---- */
#define DLPFFS_FS_SEL       0x18 // 00011000
#define DLPFFS_DLPF_CFG      0x07 // 00000111
#define INTCFG_ACTL        0x80 // 10000000
#define INTCFG_OPEN        0x40 // 01000000
#define INTCFG_LATCH_INT_EN    0x20 // 00100000
#define INTCFG_INT_ANYRD_2CLEAR  0x10 // 00010000
#define INTCFG_GYRO_RDY_EN     0x04 // 00000100
#define INTCFG_RAW_RDY_EN     0x01 // 00000001
#define INTSTATUS_GYRO_RDY     0x04 // 00000100
#define INTSTATUS_RAW_DATA_RDY  0x01 // 00000001
#define PWRMGM_HRESET       0x80 // 10000000
#define PWRMGM_SLEEP       0x40 // 01000000
#define PWRMGM_STBY_XG      0x20 // 00100000
#define PWRMGM_STBY_YG      0x10 // 00010000
#define PWRMGM_STBY_ZG      0x08 // 00001000
#define PWRMGM_CLK_SEL      0x07 // 00000111


/* ---- Register Parameters ---- */
// Sample Rate Divider
#define NOSRDIVIDER	0 // default  FsampleHz=SampleRateHz/(divider+1)
// Gyro Full Scale Range
#define RANGE2000	3  // default
// Digital Low Pass Filter BandWidth and SampleRate
#define BW256_SR8	0  // default  256Khz BW and 8Khz SR
#define BW188_SR1	1
#define BW098_SR1	2
#define BW042_SR1	3
#define BW020_SR1	4
#define BW010_SR1	5
#define BW005_SR1	6
// Interrupt Active logic lvl
#define ACTIVE_ONHIGH	0 // default
#define ACTIVE_ONLOW	1
// Interrupt drive type
#define PUSH_PULL	0 // default
#define OPEN_DRAIN	1
// Interrupt Latch mode
#define PULSE_50US	0 // default
#define UNTIL_INT_CLEARED	1
// Interrupt Latch clear method
#define READ_STATUSREG	0 // default
#define READ_ANYREG	1
// Power management
#define NORMAL	0 // default
#define STANDBY  1	//Standby for gyro
// Clock Source - user parameters
#define INTERNALOSC	0  // default
#define PLL_XGYRO_REF	1
#define PLL_YGYRO_REF	2
#define PLL_ZGYRO_REF	3
#define PLL_EXTERNAL32	4  // 32.768 kHz
#define PLL_EXTERNAL19	5  // 19.2 Mhz

/*-------------------------------------------TODO: ADD LOW POWER MODE FOR WHEN CHARGING ------------------------*/

class Gyro {
public:
 float gains[3]; 
 int offsets[3];
 float polarities[3];

 Gyro();
 
 // Gyro initialization
 void init(uint32_t address);
 void init(uint32_t address, uint8_t sRateDiv, uint8_t range, uint8_t filterBW, uint8_t clockSrc, uint8_t gyroReady, uint8_t rawDataReady);   
  
 // Who Am I
 uint8_t getDevAddr();
 void setDevAddr(uint32_t addr);
 // Sample Rate Divider
 uint8_t getSampleRateDiv();     
 void setSampleRateDiv(uint8_t sampleRate);
 // Digital Low Pass Filter BandWidth and SampleRate 
 uint8_t getFSRange();
 void setFSRange(uint8_t range); // RANGE2000
 uint8_t getFilterBW(); 
 void setFilterBW(uint8_t BW); // see register parameters above
 // Interrupt Configuration
 uint8_t isINTActiveOnLow();
 void setINTLogiclvl(uint8_t state); //ACTIVE_ONHIGH, ACTIVE_ONLOW
 // Interrupt drive type
 uint8_t isINTOpenDrain();
 void setINTDriveType(uint8_t state); //OPEN_DRAIN, PUSH_PULL
 // Interrupt Latch mode
 uint8_t isLatchUntilCleared();
 void setLatchMode(uint8_t state); //UNTIL_INT_CLEARED, PULSE_50US
 // Interrupt Latch clear method
 uint8_t isAnyRegClrMode();
 void setLatchClearMode(uint8_t state); //READ_ANYREG, READ_STATUSREG
 // INT pin triggers
 uint8_t isGyroReadyOn();     
 void setGyroReady(uint8_t state);
 uint8_t isRawDataReadyOn();
 void setRawDataReady(uint8_t state);   
 // Trigger Status
 uint8_t isIGyroReady();
 uint8_t isRawDataReady();
 // Gyro Sensors
 void readTemp(uint32_t* temp);
 void readGyroRaw(uint32_t* gyroXYZ);
 void readGyroRaw(uint32_t* gyroX, uint32_t* gyroY, uint32_t* gyroZ);
 void setRevPolarity(uint8_t xPol, uint8_t yPol, uint8_t zPol);   // true = Reversed false = default
 void setGains(uint32_t xGain, uint32_t yGain, uint32_t zGain);
 void setOffsets(int32_t xOffset, int32_t yOffset, int32_t zOffset);
 void zeroCalibrate(int32_t totSamples, int32_t sampleDelayMS);   // assuming gyroscope is stationary (updates XYZ offsets for zero)
 void readGyroRawCal(uint32_t* gyroX, uint32_t* gyroY, uint32_t* gyroZ);
 void readGyroRawCal(uint32_t* gyroXYZ);
 void readGyro(uint32_t* gyroXYZ); // includes gain and offset
 void readGyro(uint32_t* gyroX, uint32_t* gyroY, uint32_t* gyroZ); // includes gain and offset  
 // Power management
 void reset(); // after reset all registers have default values
 uint8_t isLowPower();
 void setPowerMode(uint8_t state); // NORMAL, STANDBY
 uint8_t isXgyroStandby();      
 uint8_t isYgyroStandby();
 uint8_t isZgyroStandby();
 void setXgyroStandby(uint8_t status); // NORMAL, STANDBY
 void setYgyroStandby(uint8_t status);
 void setZgyroStandby(uint8_t status);
 uint8_t getClockSource();
 void setClockSource(uint8_t clockSource); // see register parameters above
 
 void writemem(uint8_t addr, uint8_t val);
 void readmem(uint8_t addr, uint8_t nBytes, uint8_t buff1[]);
 
private:
 uint8_t devAddress;
 uint8_t buff[6];   
};

#endif	//Gyro_h