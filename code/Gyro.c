/****************************************************************************
* Tested on Arduino Mega with ITG-3200 Breakout               *
* SCL   -> pin 21   (no pull up resistors)               *
* SDA   -> pin 20   (no pull up resistors)               *
* CLK & GND -> pin GND                           *
* INT    -> not connected (but can be used)               *
* VIO & VDD -> pin 3.3V                           *
*****************************************************************************/
#include "Gyro.h"


Gyro::Gyro() {
 setGains(1.0,1.0,1.0);
 setOffsets(0.0,0.0,0.0);
 setRevPolarity(0,0,0);
 //Wire.begin();    
}

void Gyro::init(unsigned int address) {
 // Uncomment or change your default gyro initialization
 
 // fast sample rate - divisor = 0 filter = 0 clocksrc = 0, 1, 2, or 3 (raw values)
 init(address, NOSRDIVIDER, RANGE2000, BW256_SR8, PLL_XGYRO_REF, true, true);
 
 // slow sample rate - divisor = 0 filter = 1,2,3,4,5, or 6 clocksrc = 0, 1, 2, or 3 (raw values)
 //init(NOSRDIVIDER, RANGE2000, BW010_SR1, INTERNALOSC, true, true);
 
 // fast sample rate 32Khz external clock - divisor = 0 filter = 0 clocksrc = 4 (raw values)
 //init(NOSRDIVIDER, RANGE2000, BW256_SR8, PLL_EXTERNAL32, true, true);
 
 // slow sample rate 32Khz external clock - divisor = 0 filter = 1,2,3,4,5, or 6 clocksrc = 4 (raw values)
 //init(NOSRDIVIDER, RANGE2000, BW010_SR1, PLL_EXTERNAL32, true, true);
}

void Gyro::init(unsigned int address, byte _SRateDiv, byte _Range, byte _filterBW, byte _ClockSrc, bool _ITGReady, bool _INTRawDataReady) {
 _dev_address = address;
 setSampleRateDiv(_SRateDiv);
 setFSRange(_Range);
 setFilterBW(_filterBW);
 setClockSource(_ClockSrc);
 setITGReady(_ITGReady);
 setRawDataReady(_INTRawDataReady); 
 delay(GYROSTART_UP_DELAY); // startup 
}

byte Gyro::getDevAddr() {
 /*readmem(WHO_AM_I, 1, &_buff[0]); 
 return _buff[0]; */
 return _dev_address;
}

void Gyro::setDevAddr(unsigned int _addr) {
 writemem(WHO_AM_I, _addr); 
 _dev_address = _addr;
}

byte Gyro::getSampleRateDiv() {
 readmem(SMPLRT_DIV, 1, &_buff[0]);
 return _buff[0];
}

void Gyro::setSampleRateDiv(byte _SampleRate) {
 writemem(SMPLRT_DIV, _SampleRate);
}

byte Gyro::getFSRange() {
 readmem(DLPF_FS, 1, &_buff[0]);
 return ((_buff[0] & DLPFFS_FS_SEL) >> 3);
}

void Gyro::setFSRange(byte _Range) {
 readmem(DLPF_FS, 1, &_buff[0]);  
 writemem(DLPF_FS, ((_buff[0] & ~DLPFFS_FS_SEL) | (_Range << 3)) ); 
}

byte Gyro::getFilterBW() { 
 readmem(DLPF_FS, 1, &_buff[0]);
 return (_buff[0] & DLPFFS_DLPF_CFG); 
}

void Gyro::setFilterBW(byte _BW) {  
 readmem(DLPF_FS, 1, &_buff[0]);
 writemem(DLPF_FS, ((_buff[0] & ~DLPFFS_DLPF_CFG) | _BW)); 
}

bool Gyro::isINTActiveOnLow() { 
 readmem(INT_CFG, 1, &_buff[0]);
 return ((_buff[0] & INTCFG_ACTL) >> 7);
}

void Gyro::setINTLogiclvl(bool _State) {
 readmem(INT_CFG, 1, &_buff[0]);
 writemem(INT_CFG, ((_buff[0] & ~INTCFG_ACTL) | (_State << 7))); 
}

bool Gyro::isINTOpenDrain() { 
 readmem(INT_CFG, 1, &_buff[0]);
 return ((_buff[0] & INTCFG_OPEN) >> 6);
}

void Gyro::setINTDriveType(bool _State) {
 readmem(INT_CFG, 1, &_buff[0]);
 writemem(INT_CFG, ((_buff[0] & ~INTCFG_OPEN) | _State << 6)); 
}

bool Gyro::isLatchUntilCleared() {  
 readmem(INT_CFG, 1, &_buff[0]);
 return ((_buff[0] & INTCFG_LATCH_INT_EN) >> 5);
}

void Gyro::setLatchMode(bool _State) {
 readmem(INT_CFG, 1, &_buff[0]);
 writemem(INT_CFG, ((_buff[0] & ~INTCFG_LATCH_INT_EN) | _State << 5)); 
}

bool Gyro::isAnyRegClrMode() {  
 readmem(INT_CFG, 1, &_buff[0]);
 return ((_buff[0] & INTCFG_INT_ANYRD_2CLEAR) >> 4);
}

void Gyro::setLatchClearMode(bool _State) {
 readmem(INT_CFG, 1, &_buff[0]);
 writemem(INT_CFG, ((_buff[0] & ~INTCFG_INT_ANYRD_2CLEAR) | _State << 4)); 
}

bool Gyro::isITGReadyOn() {  
 readmem(INT_CFG, 1, &_buff[0]);
 return ((_buff[0] & INTCFG_ITG_RDY_EN) >> 2);
}

void Gyro::setITGReady(bool _State) {
 readmem(INT_CFG, 1, &_buff[0]);
 writemem(INT_CFG, ((_buff[0] & ~INTCFG_ITG_RDY_EN) | _State << 2)); 
}

bool Gyro::isRawDataReadyOn() {
 readmem(INT_CFG, 1, &_buff[0]);
 return (_buff[0] & INTCFG_RAW_RDY_EN);
}

void Gyro::setRawDataReady(bool _State) {
 readmem(INT_CFG, 1, &_buff[0]);
 writemem(INT_CFG, ((_buff[0] & ~INTCFG_RAW_RDY_EN) | _State)); 
}

bool Gyro::isITGReady() {
 readmem(INT_STATUS, 1, &_buff[0]);
 return ((_buff[0] & INTSTATUS_ITG_RDY) >> 2);
}

bool Gyro::isRawDataReady() {
 readmem(INT_STATUS, 1, &_buff[0]);
 return (_buff[0] & INTSTATUS_RAW_DATA_RDY);
}

void Gyro::readTemp(float *_Temp) {
 readmem(TEMP_OUT,2,_buff);
 *_Temp = 35 + (((_buff[0] << 8) | _buff[1]) + 13200) / 280.0;  // F=C*9/5+32 
}

void Gyro::readGyroRaw(int *_GyroX, int *_GyroY, int *_GyroZ){
 readmem(GYRO_XOUT, 6, _buff);
 *_GyroX = ((_buff[0] << 8) | _buff[1]);
 *_GyroY = ((_buff[2] << 8) | _buff[3]); 
 *_GyroZ = ((_buff[4] << 8) | _buff[5]);
}

void Gyro::readGyroRaw(int *_GyroXYZ){
 readGyroRaw(_GyroXYZ, _GyroXYZ+1, _GyroXYZ+2);
}

void Gyro::setRevPolarity(bool _Xpol, bool _Ypol, bool _Zpol) {
 polarities[0] = _Xpol ? -1 : 1;
 polarities[1] = _Ypol ? -1 : 1;
 polarities[2] = _Zpol ? -1 : 1;
}

void Gyro::setGains(float _Xgain, float _Ygain, float _Zgain) {
 gains[0] = _Xgain;
 gains[1] = _Ygain;
 gains[2] = _Zgain;
}

void Gyro::setOffsets(int _Xoffset, int _Yoffset, int _Zoffset) {
 offsets[0] = _Xoffset;
 offsets[1] = _Yoffset;
 offsets[2] = _Zoffset;
}

void Gyro::zeroCalibrate(unsigned int totSamples, unsigned int sampleDelayMS) {
 int xyz[3]; 
 float tmpOffsets[] = {0,0,0};

 for (int i = 0;i < totSamples;i++){
  delay(sampleDelayMS);
  readGyroRaw(xyz);
  tmpOffsets[0] += xyz[0];
  tmpOffsets[1] += xyz[1];
  tmpOffsets[2] += xyz[2]; 
 }
 setOffsets(-tmpOffsets[0] / totSamples, -tmpOffsets[1] / totSamples, -tmpOffsets[2] / totSamples);
}

void Gyro::readGyroRawCal(int *_GyroX, int *_GyroY, int *_GyroZ) {
 readGyroRaw(_GyroX, _GyroY, _GyroZ);
 *_GyroX += offsets[0];
 *_GyroY += offsets[1];
 *_GyroZ += offsets[2];
}

void Gyro::readGyroRawCal(int *_GyroXYZ) {
 readGyroRawCal(_GyroXYZ, _GyroXYZ+1, _GyroXYZ+2);
}

void Gyro::readGyro(float *_GyroX, float *_GyroY, float *_GyroZ){
 int x, y, z;
 
 readGyroRawCal(&x, &y, &z); // x,y,z will contain calibrated integer values from the sensor
 *_GyroX = x / 14.375 * polarities[0] * gains[0];
 *_GyroY = y / 14.375 * polarities[1] * gains[1];
 *_GyroZ = z / 14.375 * polarities[2] * gains[2];
}

void Gyro::readGyro(float *_GyroXYZ){
 readGyro(_GyroXYZ, _GyroXYZ+1, _GyroXYZ+2);
}

void Gyro::reset() {   
 writemem(PWR_MGM, PWRMGM_HRESET); 
 delay(GYROSTART_UP_DELAY); //gyro startup 
}

bool Gyro::isLowPower() {  
 readmem(PWR_MGM, 1, &_buff[0]);
 return (_buff[0] & PWRMGM_SLEEP) >> 6;
}
 
void Gyro::setPowerMode(bool _State) {
 readmem(PWR_MGM, 1, &_buff[0]);
 writemem(PWR_MGM, ((_buff[0] & ~PWRMGM_SLEEP) | _State << 6)); 
}

bool Gyro::isXgyroStandby() {
 readmem(PWR_MGM, 1, &_buff[0]);
 return (_buff[0] & PWRMGM_STBY_XG) >> 5;
}

bool Gyro::isYgyroStandby() {
 readmem(PWR_MGM, 1, &_buff[0]);
 return (_buff[0] & PWRMGM_STBY_YG) >> 4;
}

bool Gyro::isZgyroStandby() {
 readmem(PWR_MGM, 1, &_buff[0]);
 return (_buff[0] & PWRMGM_STBY_ZG) >> 3;
}

void Gyro::setXgyroStandby(bool _Status) {
 readmem(PWR_MGM, 1, &_buff[0]);
 writemem(PWR_MGM, ((_buff[0] & PWRMGM_STBY_XG) | _Status << 5));
}

void Gyro::setYgyroStandby(bool _Status) {
 readmem(PWR_MGM, 1, &_buff[0]);
 writemem(PWR_MGM, ((_buff[0] & PWRMGM_STBY_YG) | _Status << 4));
}

void Gyro::setZgyroStandby(bool _Status) {
 readmem(PWR_MGM, 1, &_buff[0]);
 writemem(PWR_MGM, ((_buff[0] & PWRMGM_STBY_ZG) | _Status << 3));
}

byte Gyro::getClockSource() { 
 readmem(PWR_MGM, 1, &_buff[0]);
 return (_buff[0] & PWRMGM_CLK_SEL);
}

void Gyro::setClockSource(byte _CLKsource) {  
 readmem(PWR_MGM, 1, &_buff[0]);
 writemem(PWR_MGM, ((_buff[0] & ~PWRMGM_CLK_SEL) | _CLKsource)); 
}

void Gyro::writemem(uint8_t _addr, uint8_t _val) {
 Wire.beginTransmission(_dev_address);  // start transmission to device 
 Wire.write(_addr); // send register address
 Wire.write(_val); // send value to write
 Wire.endTransmission(); // end transmission
}

void Gyro::readmem(uint8_t _addr, uint8_t _nbytes, uint8_t __buff[]) {
 Wire.beginTransmission(_dev_address); // start transmission to device 
 Wire.write(_addr); // sends register address to read from
 Wire.endTransmission(); // end transmission
 
 Wire.beginTransmission(_dev_address); // start transmission to device 
 Wire.requestFrom(_dev_address, _nbytes);// send data n-bytes read
 uint8_t i = 0; 
 while (Wire.available()) {
  __buff[i] = Wire.read(); // receive DATA
  i++;
 }
 Wire.endTransmission(); // end transmission
}