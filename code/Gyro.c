/****************************************************************************
* Tested on Arduino Mega              *
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
}

void Gyro::init(uint32_t address) {
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

void Gyro::init(uint32_t address, uint8_t sRateDiv, uint8_t range, uint8_t filterBW, uint8_t clockSrc, uint8_t gyroReady, uint8_t rawDataReady) {
 devAddress = address;
 setSampleRateDiv(sRateDiv);
 setFSRange(range);
 setFilterBW(filterBW);
 setClockSource(clockSrc);
 setGyroReady(gyroReady);
 setRawDataReady(rawDataReady); 
 delay(GYROSTART_UP_DELAY); // startup 
}

uint8_t Gyro::getDevAddr() {
 /*readmem(WHO_AM_I, 1, &_buff[0]); 
 return _buff[0]; */
 return devAddress;
}

void Gyro::setDevAddr(uint32_t addr) {
 writemem(WHO_AM_I, addr); 
 devAddress = addr;
}

uint8_t Gyro::getSampleRateDiv() {
 readmem(SMPLRT_DIV, 1, &buff[0]);
 return buff[0];
}

void Gyro::setSampleRateDiv(uint8_t sampleRate) {
 writemem(SMPLRT_DIV, sampleRate);
}

uint8_t Gyro::getFSRange() {
 readmem(DLPF_FS, 1, &buff[0]);
 return ((buff[0] & DLPFFS_FS_SEL) >> 3);
}

void Gyro::setFSRange(uint8_t range) {
 readmem(DLPF_FS, 1, &buff[0]);  
 writemem(DLPF_FS, ((buff[0] & ~DLPFFS_FS_SEL) | (range << 3)) ); 
}

uint8_t Gyro::getFilterBW() { 
 readmem(DLPF_FS, 1, &buff[0]);
 return (buff[0] & DLPFFS_DLPF_CFG); 
}

void Gyro::setFilterBW(uint8_t BW) {  
 readmem(DLPF_FS, 1, &buff[0]);
 writemem(DLPF_FS, ((buff[0] & ~DLPFFS_DLPF_CFG) | BW)); 
}

uint8_t Gyro::isINTActiveOnLow() { 
 readmem(INT_CFG, 1, &buff[0]);
 return ((buff[0] & INTCFG_ACTL) >> 7);
}

void Gyro::setINTLogiclvl(uint8_t state) {
 readmem(INT_CFG, 1, &buff[0]);
 writemem(INT_CFG, ((buff[0] & ~INTCFG_ACTL) | (state << 7))); 
}

uint8_t Gyro::isINTOpenDrain() { 
 readmem(INT_CFG, 1, &buff[0]);
 return ((buff[0] & INTCFG_OPEN) >> 6);
}

void Gyro::setINTDriveType(uint8_t state) {
 readmem(INT_CFG, 1, &buff[0]);
 writemem(INT_CFG, ((buff[0] & ~INTCFG_OPEN) | state << 6)); 
}

uint8_t Gyro::isLatchUntilCleared() {  
 readmem(INT_CFG, 1, &buff[0]);
 return ((buff[0] & INTCFG_LATCH_INT_EN) >> 5);
}

void Gyro::setLatchMode(uint8_t state) {
 readmem(INT_CFG, 1, &buff[0]);
 writemem(INT_CFG, ((buff[0] & ~INTCFG_LATCH_INT_EN) | state << 5)); 
}

uint8_t Gyro::isAnyRegClrMode() {  
 readmem(INT_CFG, 1, &buff[0]);
 return ((buff[0] & INTCFG_INT_ANYRD_2CLEAR) >> 4);
}

void Gyro::setLatchClearMode(uint8_t state) {
 readmem(INT_CFG, 1, &buff[0]);
 writemem(INT_CFG, ((buff[0] & ~INTCFG_INT_ANYRD_2CLEAR) | state << 4)); 
}

uint8_t Gyro::isGyroReadyOn() {  
 readmem(INT_CFG, 1, &buff[0]);
 return ((buff[0] & INTCFG_GYRO_RDY_EN) >> 2);
}

void Gyro::setGyroReady(uint8_t state) {
 readmem(INT_CFG, 1, &buff[0]);
 writemem(INT_CFG, ((buff[0] & ~INTCFG_GYRO_RDY_EN) | state << 2)); 
}

uint8_t Gyro::isRawDataReadyOn() {
 readmem(INT_CFG, 1, &buff[0]);
 return (buff[0] & INTCFG_RAW_RDY_EN);
}

void Gyro::setRawDataReady(uint8_t state) {
 readmem(INT_CFG, 1, &buff[0]);
 writemem(INT_CFG, ((buff[0] & ~INTCFG_RAW_RDY_EN) | state)); 
}

uint8_t Gyro::isGyroReady() {
 readmem(INT_STATUS, 1, &buff[0]);
 return ((buff[0] & INTSTATUS_GYRO_RDY) >> 2);
}

uint8_t Gyro::isRawDataReady() {
 readmem(INT_STATUS, 1, &buff[0]);
 return (buff[0] & INTSTATUS_RAW_DATA_RDY);
}

void Gyro::readTemp(uint32_t* temp) {
 readmem(TEMP_OUT, 2, buff);
 *temp = 35 + (((buff[0] << 8) | buff[1]) + 13200) / 280.0;  // F=C*9/5+32 
}

void Gyro::readGyroRaw(uint32_t* gyroX, uint32_t* gyroY, uint32_t* gyroZ){
 readmem(GYRO_XOUT, 6, buff);
 *gyroX = ((buff[0] << 8) | buff[1]);
 *gyroY = ((buff[2] << 8) | buff[3]); 
 *gyroZ = ((buff[4] << 8) | buff[5]);
}

void Gyro::readGyroRaw(uint32_t* gyroXYZ){
 readGyroRaw(gyroXYZ, gyroXYZ+1, gyroXYZ+2);
}

void Gyro::setRevPolarity(uint8_t xPol, uint8_t yPol, uint8_t zPol) {
 polarities[0] = xPol ? -1 : 1;
 polarities[1] = yPol ? -1 : 1;
 polarities[2] = zPol ? -1 : 1;
}

void Gyro::setGains(uint32_t xGain, uint32_t yGain, uint32_t zGain) {
 gains[0] = xGain;
 gains[1] = yGain;
 gains[2] = zGain;
}

void Gyro::setOffsets(uint32_t xOffset, uint32_t yOffset, uint32_t zOffset) {
 offsets[0] = xOffset;
 offsets[1] = yOffset;
 offsets[2] = zOffset;
}

void Gyro::zeroCalibrate(uint32_t totSamples, uint32_t sampleDelayMS) {
 uint32_t xyz[3]; 
 uint8_t i;
 uint32_t tmpOffsets[] = {0,0,0};

 for (i = 0; i<totSamples; i++){
  delay(sampleDelayMS);
  readGyroRaw(xyz);
  tmpOffsets[0] += xyz[0];
  tmpOffsets[1] += xyz[1];
  tmpOffsets[2] += xyz[2]; 
 }
 setOffsets(-tmpOffsets[0] / totSamples, -tmpOffsets[1] / totSamples, -tmpOffsets[2] / totSamples);
}

void Gyro::readGyroRawCal(uint32_t* gyroX, uint32_t* gyroY, uint32_t* gyroZ) {
 readGyroRaw(gyroX, gyroY, gyroZ);
 *gyroX += offsets[0];
 *gyroY += offsets[1];
 *gyroZ += offsets[2];
}

void Gyro::readGyroRawCal(uint32_t* gyroXYZ) {
 readGyroRawCal(gyroXYZ, gyroXYZ+1, gyroXYZ+2);
}

void Gyro::readGyro(uint32_t* gyroX, uint32_t* gyroY, uint32_t* gyroZ){
 uint32_t x, y, z;
 readGyroRawCal(&x, &y, &z); // x,y,z will contain calibrated integer values from the sensor
 *gyroX = x / 14.375 * polarities[0] * gains[0];
 *gyroY = y / 14.375 * polarities[1] * gains[1];
 *gyroZ = z / 14.375 * polarities[2] * gains[2];
}

void Gyro::readGyro(uint32_t* gyroXYZ){
 readGyro(gyroXYZ, gyroXYZ+1, gyroXYZ+2);
}

void Gyro::reset() {   
 writemem(PWR_MGM, PWRMGM_HRESET); 
 delay(GYROSTART_UP_DELAY); //gyro startup 
}

uint8_t Gyro::isLowPower() {  
 readmem(PWR_MGM, 1, &buff[0]);
 return (buff[0] & PWRMGM_SLEEP) >> 6;
}
 
void Gyro::setPowerMode(uint8_t state) {
 readmem(PWR_MGM, 1, &buff[0]);
 writemem(PWR_MGM, ((buff[0] & ~PWRMGM_SLEEP) | state << 6)); 
}

uint8_t Gyro::isXgyroStandby() {
 readmem(PWR_MGM, 1, &buff[0]);
 return (buff[0] & PWRMGM_STBY_XG) >> 5;
}

uint8_t Gyro::isYgyroStandby() {
 readmem(PWR_MGM, 1, &buff[0]);
 return (buff[0] & PWRMGM_STBY_YG) >> 4;
}

uint8_t Gyro::isZgyroStandby() {
 readmem(PWR_MGM, 1, &buff[0]);
 return (buff[0] & PWRMGM_STBY_ZG) >> 3;
}

void Gyro::setXgyroStandby(uint8_t status) {
 readmem(PWR_MGM, 1, &buff[0]);
 writemem(PWR_MGM, ((buff[0] & PWRMGM_STBY_XG) | status << 5));
}

void Gyro::setYgyroStandby(uint8_t status) {
 readmem(PWR_MGM, 1, &buff[0]);
 writemem(PWR_MGM, ((buff[0] & PWRMGM_STBY_YG) | status << 4));
}

void Gyro::setZgyroStandby(uint8_t status) {
 readmem(PWR_MGM, 1, &buff[0]);
 writemem(PWR_MGM, ((buff[0] & PWRMGM_STBY_ZG) | status << 3));
}

uint8_t Gyro::getClockSource() { 
 readmem(PWR_MGM, 1, &buff[0]);
 return (buff[0] & PWRMGM_CLK_SEL);
}

void Gyro::setClockSource(uint8_t clockSource) {  
 readmem(PWR_MGM, 1, &buff[0]);
 writemem(PWR_MGM, ((buff[0] & ~PWRMGM_CLK_SEL) | clockSource)); 
}

void Gyro::writemem(uint8_t addr, uint8_t val) {
 Wire.beginTransmission(devAddress);  // start transmission to device 
 Wire.write(addr); // send register address
 Wire.write(val); // send value to write
 Wire.endTransmission(); // end transmission
}

void Gyro::readmem(uint8_t addr, uint8_t nBytes, uint8_t buff1[]) {
 Wire.beginTransmission(devAddress); // start transmission to device 
 Wire.write(addr); // sends register address to read from
 Wire.endTransmission(); // end transmission
 
 Wire.beginTransmission(devAddress); // start transmission to device 
 Wire.requestFrom(devAddress, nBytes);// send data # of bytes read
 uint8_t i = 0; 
 while (Wire.available()) {
  buff1[i] = Wire.read(); // receive DATA
  i++;
 }
 Wire.endTransmission(); // end transmission
}