/* ----------------------------------- Class for Accelerometer -------------------------------------*/
#include "Accel.h"
#include <Wire.h>

#define TO_READ (6)   // num of bytes we are going to read each time (two bytes for each axis)

Accel::Accel() {
 status = ACCEL_OK;
 errorCode = ACCEL_NO_ERROR;

 gains[0] = 0.00376390;
 gains[1] = 0.00376009;
 gains[2] = 0.00349265;
}

void Accel::init(uint8_t address) {
 devAddress = address;
 powerOn();
}

/* ----------------------------------- Turning on the Accelerometer ----------------------------- */
void Accel::powerOn() {
 writeTo(ACCEL_POWER_CTL, 8);
}

/* ----------------------------------- Reads the acceleration into an array of three places ----------------------------- */
void Accel::readAccel(uint16_t *xyz){
 readAccel(xyz, xyz + 1, xyz + 2);
}

/* ----------------------------------- Reads the acceleration into three variables: x, y and z ----------------------------- */
void Accel::readAccel(uint16_t *x, uint16_t *y, uint16_t *z) {
 readFrom(ACCEL_DATAX0, TO_READ, buff); 
 //read the acceleration data from the Accel
 // each axis reading comes in 10 bit resolution, ie 2 bytes. Least Significat Byte first!!
 // thus we are converting both bytes into one int
 *x = (((uint32_t)buff[1]) << 8) | buff[0]; 
 *y = (((uint32_t)buff[3]) << 8) | buff[2];
 *z = (((uint32_t)buff[5]) << 8) | buff[4];
}

void Accel::get_Gxyz(uint32_t* xyz){
 uint16_t xyz_int[3];
 uint8_t i;
 readAccel(xyz_int);
 for(i=0; i<3; i++){
  xyz[i] = xyz_int[i] * gains[i];
}
}

// Writes val to address register on device
void Accel::writeTo(uint8_t address, uint8_t val) {
 Wire.beginTransmission(devAddress); // start transmission to device
 Wire.write(address);       // send register address
 Wire.write(val);         // send value to write
 Wire.endTransmission();     // end transmission
}

// Reads num bytes starting from address register on device in to buff array
void Accel::readFrom(uint8_t address, uint16_t num, uint8_t buff[]) {
 Wire.beginTransmission(devAddress); // start transmission to device
 Wire.write(address);       // sends address to read from
 Wire.endTransmission();     // end transmission

 Wire.beginTransmission(devAddress); // start transmission to device
 Wire.requestFrom(devAddress, num);  // request 6 bytes from device

 uint8_t i = 0;
 while(Wire.available()) {  
  buff[i] = Wire.read(); 
  i++;
}

 if(i != num){  // if device recieves less than 6 bits requested
  status = ACCEL_ERROR;
  errorCode = ACCEL_READ_ERROR;
}
 Wire.endTransmission();     // end transmission
}

// Gets the range setting and return it into rangeSetting
// it can be 2, 4, 8 or 16
void Accel::getRangeSetting(uint8_t* rangeSetting) {
 uint8_t b;
 readFrom(ACCEL_DATA_FORMAT, 1, &b);
 *rangeSetting = b & B00000011;
}

// Sets the range setting, possible values are: 2, 4, 8, 16
void Accel::setRangeSetting(uint16_t val) {
 uint8_t s;
 uint8_t b;

 switch (val) {
   case 2: 
   s = B00000000;
   break;
   case 4: 
   s = B00000001;
   break;
   case 8: 
   s = B00000010;
   break;
   case 16:
   s = B00000011;
   break;
   default:
   s = B00000000;
 }

 readFrom(ACCEL_DATA_FORMAT, 1, &b);
 s |= (b & B11101100);
 writeTo(ACCEL_DATA_FORMAT, s);
}

// gets the state of the SELF_TEST bit
uint8_t Accel::getSelfTestBit() {
 return getRegisterBit(ACCEL_DATA_FORMAT, 7);
}

// Sets the SELF-TEST bit
// if set to 1 it applies a self-test force to the sensor causing a shift in the output data
// if set to 0 it disables the self-test force
void Accel::setSelfTestBit(uint8_t selfTestBit) {
 setRegisterBit(ACCEL_DATA_FORMAT, 7, selfTestBit);
}

// Gets the state of the SPI bit
uint8_t Accel::getSpiBit() {
 return getRegisterBit(ACCEL_DATA_FORMAT, 6);
}

// Sets the SPI bit
// if set to 1 it sets the device to 3-wire mode
// if set to 0 it sets the device to 4-wire SPI mode
void Accel::setSpiBit(uint8_t spiBit) {
 setRegisterBit(ACCEL_DATA_FORMAT, 6, spiBit);
}

// Gets the state of the INT_INVERT bit
uint8_t Accel::getInterruptLevelBit() {
 return getRegisterBit(ACCEL_DATA_FORMAT, 5);
}

// Sets the INT_INVERT bit
// if set to 0 sets the interrupts to active high
// if set to 1 sets the interrupts to active low
void Accel::setInterruptLevelBit(uint8_t interruptLevelBit) {
 setRegisterBit(ACCEL_DATA_FORMAT, 5, interruptLevelBit);
}

// Gets the state of the FULL_RES bit
uint8_t Accel::getFullResBit() {
 return getRegisterBit(ACCEL_DATA_FORMAT, 3);
}

// Sets the FULL_RES bit
// if set to 1, the device is in full resolution mode, where the output resolution increases with the
//  g range set by the range bits to maintain a 4mg/LSB scal factor
// if set to 0, the device is in 10-bit mode, and the range buts determine the maximum g range
//  and scale factor
void Accel::setFullResBit(uint8_t fullResBit) {
 setRegisterBit(ACCEL_DATA_FORMAT, 3, fullResBit);
}

// Gets the state of the justify bit
uint8_t Accel::getJustifyBit() {
 return getRegisterBit(ACCEL_DATA_FORMAT, 2);
}

// Sets the JUSTIFY bit
// if sets to 1 selects the left justified mode
// if sets to 0 selects right justified mode with sign extension
void Accel::setJustifyBit(uint8_t justifyBit) {
 setRegisterBit(ACCEL_DATA_FORMAT, 2, justifyBit);
}

// Sets the THRESH_TAP byte value
// it should be between 0 and 255
// the scale factor is 62.5 mg/LSB
// A value of 0 may result in undesirable behavior
void Accel::setTapThreshold(uint16_t tapThreshold) {
 tapThreshold = min(max(tapThreshold,0),255);
 uint8_t b = uint8_t (tapThreshold);
 writeTo(ACCEL_THRESH_TAP, b); 
}

// Gets the THRESH_TAP byte value
// return value is comprised between 0 and 255
// the scale factor is 62.5 mg/LSB
uint16_t Accel::getTapThreshold() {
 uint8_t b;
 readFrom(ACCEL_THRESH_TAP, 1, &b); 
 return uint16_t (b);
}

// set/get the gain for each axis in Gs / count
void Accel::setAxisGains(uint32_t* gains){
 uint8_t i;
 for(i = 0; i < 3; i++){
  gains[i] = gains[i];
}
}
void Accel::getAxisGains(uint32_t* gains){
 uint8_t i;
 for(i = 0; i < 3; i++){
  gains[i] = gains[i];
}
}


// Sets the OFSX, OFSY and OFSZ bytes
// OFSX, OFSY and OFSZ are user offset adjustments in twos complement format with
// a scale factor of 15,6mg/LSB
// OFSX, OFSY and OFSZ should be comprised between
void Accel::setAxisOffset(uint16_t x, uint16_t y, uint16_t z) {
 writeTo(ACCEL_OFFSTX, uint8_t(x)); 
 writeTo(ACCEL_OFFSTY, uint8_t(y)); 
 writeTo(ACCEL_OFFSTZ, uint8_t(z)); 
}

// Gets the OFSX, OFSY and OFSZ bytes
void Accel::getAxisOffset(uint16_t* x, uint16_t* y, uint16_t* z) {
 uint8_t b;
 readFrom(ACCEL_OFFSTX, 1, &b); 
 *x = uint16_t(b);
 readFrom(ACCEL_OFFSTY, 1, &b); 
 *y = uint16_t(b);
 readFrom(ACCEL_OFFSTZ, 1, &b); 
 *z = uint16_t(b);
}

// Sets the DUR byte
// The DUR byte contains an unsigned time value representing the maximum time
// that an event must be above THRESH_TAP threshold to qualify as a tap event
// The scale factor is 625µs/LSB
// A value of 0 disables the tap/float tap funcitons. Max value is 255.
void Accel::setTapDuration(uint16_t tapDuration) {
 tapDuration = min(max(tapDuration,0),255);
 uint8_t b = uint8_t (tapDuration);
 writeTo(ACCEL_DUR, b); 
}

// Gets the duration byte
uint16_t Accel::getTapDuration() {
 uint8_t b;
 readFrom(ACCEL_DUR, 1, &b); 
 return uint16_t(b);
}

// Sets the latency (latent register) which contains an unsigned time value
// representing the wait time from the detection of a tap event to the start
// of the time window, during which a possible second tap can be detected.
// The scale factor is 1.25ms/LSB. A value of 0 disables the float tap function.
// It accepts a maximum value of 255.
void Accel::setDoubleTapLatency(uint16_t floatTapLatency) {
 uint8_t b = uint8_t(floatTapLatency);
 writeTo(ACCEL_LATENT, b); 
}

// Gets the Latent value
uint16_t Accel::getDoubleTapLatency() {
 uint8_t b;
 readFrom(ACCEL_LATENT, 1, &b); 
 return uint16_t(b);
}

// Sets the Window register, which contains an unsigned time value representing
// the amount of time after the expiration of the latency time (Latent register)
// during which a second valud tap can begin. The scale factor is 1.25ms/LSB. A
// value of 0 disables the float tap function. The maximum value is 255.
void Accel::setDoubleTapWindow(uint16_t floatTapWindow) {
 floatTapWindow = min(max(floatTapWindow,0),255);
 uint8_t b = uint8_t (floatTapWindow);
 writeTo(ACCEL_WINDOW, b); 
}

// Gets the Window register
uint16_t Accel::getDoubleTapWindow() {
 uint8_t b;
 readFrom(ACCEL_WINDOW, 1, &b); 
 return uint16_t(b);
}

// Sets the THRESH_ACT byte which holds the threshold value for detecting activity.
// The data format is unsigned, so the magnitude of the activity event is compared
// with the value is compared with the value in the THRESH_ACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the
// activity interrupt is enabled. The maximum value is 255.
void Accel::setActivityThreshold(uint16_t activityThreshold) {
 activityThreshold = min(max(activityThreshold,0),255);
 uint8_t b = uint8_t (activityThreshold);
 writeTo(ACCEL_THRESH_ACT, b); 
}

// Gets the THRESH_ACT byte
uint16_t Accel::getActivityThreshold() {
 uint8_t b;
 readFrom(ACCEL_THRESH_ACT, 1, &b); 
 return uint16_t(b);
}

// Sets the THRESH_INACT byte which holds the threshold value for detecting inactivity.
// The data format is unsigned, so the magnitude of the inactivity event is compared
// with the value is compared with the value in the THRESH_INACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the
// inactivity interrupt is enabled. The maximum value is 255.
void Accel::setInactivityThreshold(uint16_t inactivityThreshold) {
 inactivityThreshold = min(max(inactivityThreshold,0),255);
 uint8_t b = uint8_t (inactivityThreshold);
 writeTo(ACCEL_THRESH_INACT, b); 
}

// Gets the THRESH_INACT byte
uint16_t Accel::getInactivityThreshold() {
 uint8_t b;
 readFrom(ACCEL_THRESH_INACT, 1, &b); 
 return uint16_t(b);
}

// Sets the TIME_INACT register, which contains an unsigned time value representing the
// amount of time that acceleration must be less thant the value in the THRESH_INACT
// register for inactivity to be declared. The scale factor is 1sec/LSB. The value must
// be between 0 and 255.
void Accel::setTimeInactivity(uint16_t timeInactivity) {
 timeInactivity = min(max(timeInactivity,0),255);
 uint8_t b = uint8_t (timeInactivity);
 writeTo(ACCEL_TIME_INACT, b); 
}

// Gets the TIME_INACT register
uint16_t Accel::getTimeInactivity() {
 uint8_t b;
 readFrom(ACCEL_TIME_INACT, 1, &b); 
 return uint16_t(b);
}

// Sets the THRESH_FF register which holds the threshold value, in an unsigned format, for
// free-fall detection. The root-sum-square (RSS) value of all axes is calculated and
// compared whith the value in THRESH_FF to determine if a free-fall event occured. The
// scale factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the free-fall
// interrupt is enabled. The maximum value is 255.
void Accel::setFreeFallThreshold(uint16_t freeFallThreshold) {
 freeFallThreshold = min(max(freeFallThreshold,0),255);
 uint8_t b = uint8_t (freeFallThreshold);
 writeTo(ACCEL_THRESH_FF, b); 
}

// Gets the THRESH_FF register.
uint16_t Accel::getFreeFallThreshold() {
 uint8_t b;
 readFrom(ACCEL_THRESH_FF, 1, &b); 
 return uint16_t(b);
}

// Sets the TIME_FF register, which holds an unsigned time value representing the minimum
// time that the RSS value of all axes must be less than THRESH_FF to generate a free-fall
// interrupt. The scale factor is 5ms/LSB. A value of 0 may result in undesirable behavior if
// the free-fall interrupt is enabled. The maximum value is 255.
void Accel::setFreeFallDuration(uint16_t freeFallDuration) {
 freeFallDuration = min(max(freeFallDuration,0),255); 
 uint8_t b = uint8_t (freeFallDuration);
 writeTo(ACCEL_TIME_FF, b); 
}

// Gets the TIME_FF register.
uint16_t Accel::getFreeFallDuration() {
 uint8_t b;
 readFrom(ACCEL_TIME_FF, 1, &b); 
 return uint16_t(b);
}

uint8_t Accel::isActivityXEnabled() { 
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 6);
}
uint8_t Accel::isActivityYEnabled() { 
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 5);
}
uint8_t Accel::isActivityZEnabled() { 
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 4);
}
uint8_t Accel::isInactivityXEnabled() { 
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 2);
}
uint8_t Accel::isInactivityYEnabled() { 
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 1);
}
uint8_t Accel::isInactivityZEnabled() { 
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 0);
}

void Accel::setActivityX(uint8_t state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 6, state);
}
void Accel::setActivityY(uint8_t state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 5, state);
}
void Accel::setActivityZ(uint8_t state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 4, state);
}
void Accel::setInactivityX(uint8_t state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 2, state);
}
void Accel::setInactivityY(uint8_t state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 1, state);
}
void Accel::setInactivityZ(uint8_t state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 0, state);
}

uint8_t Accel::isActivityAc() {
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 7);
}
uint8_t Accel::isInactivityAc(){
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 3);
}

void Accel::setActivityAc(uint8_t state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 7, state);
}
void Accel::setInactivityAc(uint8_t state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 3, state);
}

uint8_t Accel::getSuppressBit(){
 return getRegisterBit(ACCEL_TAP_AXES, 3);
}
void Accel::setSuppressBit(uint8_t state) { 
 setRegisterBit(ACCEL_TAP_AXES, 3, state);
}

uint8_t Accel::isTapDetectionOnX(){
 return getRegisterBit(ACCEL_TAP_AXES, 2);
}
void Accel::setTapDetectionOnX(uint8_t state) { 
 setRegisterBit(ACCEL_TAP_AXES, 2, state);
}
uint8_t Accel::isTapDetectionOnY(){
 return getRegisterBit(ACCEL_TAP_AXES, 1);
}
void Accel::setTapDetectionOnY(uint8_t state) { 
 setRegisterBit(ACCEL_TAP_AXES, 1, state);
}
uint8_t Accel::isTapDetectionOnZ(){
 return getRegisterBit(ACCEL_TAP_AXES, 0);
}
void Accel::setTapDetectionOnZ(uint8_t state) { 
 setRegisterBit(ACCEL_TAP_AXES, 0, state);
}

uint8_t Accel::isActivitySourceOnX(){
 return getRegisterBit(ACCEL_ACT_TAP_STATUS, 6);
}
uint8_t Accel::isActivitySourceOnY(){
 return getRegisterBit(ACCEL_ACT_TAP_STATUS, 5);
}
uint8_t Accel::isActivitySourceOnZ(){
 return getRegisterBit(ACCEL_ACT_TAP_STATUS, 4);
}

uint8_t Accel::isTapSourceOnX(){
 return getRegisterBit(ACCEL_ACT_TAP_STATUS, 2);
}
uint8_t Accel::isTapSourceOnY(){
 return getRegisterBit(ACCEL_ACT_TAP_STATUS, 1);
}
uint8_t Accel::isTapSourceOnZ(){
 return getRegisterBit(ACCEL_ACT_TAP_STATUS, 0);
}

uint8_t Accel::isAsleep(){
 return getRegisterBit(ACCEL_ACT_TAP_STATUS, 3);
}

uint8_t Accel::isLowPower(){
 return getRegisterBit(ACCEL_BW_RATE, 4);
}
void Accel::setLowPower(uint8_t state) { 
 setRegisterBit(ACCEL_BW_RATE, 4, state);
}

uint32_t Accel::getRate(){
 uint8_t b;
 readFrom(ACCEL_BW_RATE, 1, &b);
 b &= B00001111;
 return (pow(2,((uint16_t) b)-6)) * 6.25;
}

void Accel::setRate(uint32_t rate){
 uint8_t b,s;
 uint16_t v = (uint16_t) (rate / 6.25);
 uint16_t r = 0;
 while (v >>= 1)
 {
  r++;
}
if (r <= 9) {
  readFrom(ACCEL_BW_RATE, 1, &b);
  s = (uint8_t) (r + 6) | (b & B11110000);
  writeTo(ACCEL_BW_RATE, s);
}
}

void Accel::set_bw(uint8_t bwCode){
 if((bwCode < ACCEL_BW_3) || (bwCode > Accel_BW_1600)){
  status = false;
  errorCode = ACCEL_BAD_ARG;
}
else{
  writeTo(ACCEL_BW_RATE, bwCode);
}
}

uint8_t Accel::getBwCode() {
 uint8_t bwCode;
 readFrom(ACCEL_BW_RATE, 1, &bwCode);
 return bwCode;
}

uint8_t Accel::getInterruptSource() {
 uint8_t b;
 readFrom(ACCEL_INT_SOURCE, 1, &b);
 return b;
}

uint8_t Accel::getInterruptSource(uint8_t interruptBit) {
 return getRegisterBit(ACCEL_INT_SOURCE,interruptBit);
}

uint8_t Accel::getInterruptMapping(uint8_t interruptBit) {
 return getRegisterBit(ACCEL_INT_MAP,interruptBit);
}

// Set the mapping of an interrupt to pin1 or pin2
// eg: setInterruptMapping(ACCEL_INT_DOUBLE_TAP_BIT,ACCEL_INT2_PIN);
void Accel::setInterruptMapping(uint8_t interruptBit, uint8_t interruptPin) {
 setRegisterBit(ACCEL_INT_MAP, interruptBit, interruptPin);
}

uint8_t Accel::isInterruptEnabled(uint8_t interruptBit) {
 return getRegisterBit(ACCEL_INT_ENABLE,interruptBit);
}

void Accel::setInterrupt(uint8_t interruptBit, uint8_t state) {
 setRegisterBit(ACCEL_INT_ENABLE, interruptBit, state);
}

void Accel::setRegisterBit(uint8_t regAdress, uint16_t bitPos, uint8_t state) {
 uint8_t b;
 readFrom(regAdress, 1, &b);
 if (state) {
  b |= (1 << bitPos); // forces nth bit of b to be 1. all other bits left alone.
}
else {
  b &= ~(1 << bitPos); // forces nth bit of b to be 0. all other bits left alone.
}
writeTo(regAdress, b); 
}

uint8_t Accel::getRegisterBit(uint8_t regAdress, uint16_t bitPos) {
 uint8_t b;
 readFrom(regAdress, 1, &b);
 return ((b >> bitPos) & 1);
}

//DEBUGGING; COMMENT OUT WHEN DONE
// print all register value to the serial ouptut, which requires it to be setup
// this can be used to manually to check the current configuration of the device
void Accel::printAllRegister() {
 uint8_t b;
 Serial.print("0x00: ");
 readFrom(0x00, 1, &b);
 print_byte(b);
 Serial.println("");
 uint8_t i;
 for (i=29;i<=57;i++){
  Serial.print("0x");
  Serial.print(i, HEX);
  Serial.print(": ");
  readFrom(i, 1, &b);
  print_byte(b);
  Serial.println("");  
}
}

//DEBUGGING; COMMENT OUT WHEN DONE
void print_byte(uint8_t val){
 Serial.print("B");
 for(uint8_t i=7; i>=0; i--){
  Serial.print(val >> i & 1, BIN);
}
}