/* ----------------------------------- Class for Accelerometer -------------------------------------*/
#include "Accel.h"
#include <Wire.h>

#define TO_READ (6)   // num of bytes we are going to read each time (two bytes for each axis)

Accel::Accel() {
 status = ACCEL_OK;
 error_code = ACCEL_NO_ERROR;

 gains[0] = 0.00376390;
 gains[1] = 0.00376009;
 gains[2] = 0.00349265;
}

void Accel::init(int address) {
 _dev_address = address;
 powerOn();
}

/* ----------------------------------- Turning on the Accelerometer ----------------------------- */
void Accel::powerOn() {
 //Turning on the Accelerometer
 //writeTo(ACCEL_POWER_CTL, 0);   
 //writeTo(ACCEL_POWER_CTL, 16);
 writeTo(ACCEL_POWER_CTL, 8);
}

/* ----------------------------------- Reads the acceleration into an array of three places ----------------------------- */
void Accel::readAccel(int *xyz){
 readAccel(xyz, xyz + 1, xyz + 2);
}

/* ----------------------------------- Reads the acceleration into three variables: x, y and z ----------------------------- */
void Accel::readAccel(int *x, int *y, int *z) {
 readFrom(ACCEL_DATAX0, TO_READ, _buff); //read the acceleration data from the Accel

 // each axis reading comes in 10 bit resolution, ie 2 bytes. Least Significat Byte first!!
 // thus we are converting both bytes into one int
 *x = (((int)_buff[1]) << 8) | _buff[0]; 
 *y = (((int)_buff[3]) << 8) | _buff[2];
 *z = (((int)_buff[5]) << 8) | _buff[4];
}

void Accel::get_Gxyz(float *xyz){
 int i;
 int xyz_int[3];
 readAccel(xyz_int);
 for(i=0; i<3; i++){
  xyz[i] = xyz_int[i] * gains[i];
 }
}

// Writes val to address register on device
void Accel::writeTo(byte address, byte val) {
 Wire.beginTransmission(_dev_address); // start transmission to device
 Wire.write(address);       // send register address
 Wire.write(val);         // send value to write
 Wire.endTransmission();     // end transmission
}

// Reads num bytes starting from address register on device in to _buff array
void Accel::readFrom(byte address, int num, byte _buff[]) {
 Wire.beginTransmission(_dev_address); // start transmission to device
 Wire.write(address);       // sends address to read from
 Wire.endTransmission();     // end transmission

 Wire.beginTransmission(_dev_address); // start transmission to device
 Wire.requestFrom(_dev_address, num);  // request 6 bytes from device

 int i = 0;
 while(Wire.available())     // device may send less than requested (abnormal)
 {
  _buff[i] = Wire.read();  // receive a byte
  i++;
 }
 if(i != num){
  status = ACCEL_ERROR;
  error_code = ACCEL_READ_ERROR;
 }
 Wire.endTransmission();     // end transmission
}

// Gets the range setting and return it into rangeSetting
// it can be 2, 4, 8 or 16
void Accel::getRangeSetting(byte* rangeSetting) {
 byte _b;
 readFrom(ACCEL_DATA_FORMAT, 1, &_b);
 *rangeSetting = _b & B00000011;
}

// Sets the range setting, possible values are: 2, 4, 8, 16
void Accel::setRangeSetting(int val) {
 byte _s;
 byte _b;

 switch (val) {
 case 2: 
  _s = B00000000;
  break;
 case 4: 
  _s = B00000001;
  break;
 case 8: 
  _s = B00000010;
  break;
 case 16:
  _s = B00000011;
  break;
 default:
  _s = B00000000;
 }
 readFrom(ACCEL_DATA_FORMAT, 1, &_b);
 _s |= (_b & B11101100);
 writeTo(ACCEL_DATA_FORMAT, _s);
}
// gets the state of the SELF_TEST bit
bool Accel::getSelfTestBit() {
 return getRegisterBit(ACCEL_DATA_FORMAT, 7);
}

// Sets the SELF-TEST bit
// if set to 1 it applies a self-test force to the sensor causing a shift in the output data
// if set to 0 it disables the self-test force
void Accel::setSelfTestBit(bool selfTestBit) {
 setRegisterBit(ACCEL_DATA_FORMAT, 7, selfTestBit);
}

// Gets the state of the SPI bit
bool Accel::getSpiBit() {
 return getRegisterBit(ACCEL_DATA_FORMAT, 6);
}

// Sets the SPI bit
// if set to 1 it sets the device to 3-wire mode
// if set to 0 it sets the device to 4-wire SPI mode
void Accel::setSpiBit(bool spiBit) {
 setRegisterBit(ACCEL_DATA_FORMAT, 6, spiBit);
}

// Gets the state of the INT_INVERT bit
bool Accel::getInterruptLevelBit() {
 return getRegisterBit(ACCEL_DATA_FORMAT, 5);
}

// Sets the INT_INVERT bit
// if set to 0 sets the interrupts to active high
// if set to 1 sets the interrupts to active low
void Accel::setInterruptLevelBit(bool interruptLevelBit) {
 setRegisterBit(ACCEL_DATA_FORMAT, 5, interruptLevelBit);
}

// Gets the state of the FULL_RES bit
bool Accel::getFullResBit() {
 return getRegisterBit(ACCEL_DATA_FORMAT, 3);
}

// Sets the FULL_RES bit
// if set to 1, the device is in full resolution mode, where the output resolution increases with the
//  g range set by the range bits to maintain a 4mg/LSB scal factor
// if set to 0, the device is in 10-bit mode, and the range buts determine the maximum g range
//  and scale factor
void Accel::setFullResBit(bool fullResBit) {
 setRegisterBit(ACCEL_DATA_FORMAT, 3, fullResBit);
}

// Gets the state of the justify bit
bool Accel::getJustifyBit() {
 return getRegisterBit(ACCEL_DATA_FORMAT, 2);
}

// Sets the JUSTIFY bit
// if sets to 1 selects the left justified mode
// if sets to 0 selects right justified mode with sign extension
void Accel::setJustifyBit(bool justifyBit) {
 setRegisterBit(ACCEL_DATA_FORMAT, 2, justifyBit);
}

// Sets the THRESH_TAP byte value
// it should be between 0 and 255
// the scale factor is 62.5 mg/LSB
// A value of 0 may result in undesirable behavior
void Accel::setTapThreshold(int tapThreshold) {
 tapThreshold = min(max(tapThreshold,0),255);
 byte _b = byte (tapThreshold);
 writeTo(ACCEL_THRESH_TAP, _b); 
}

// Gets the THRESH_TAP byte value
// return value is comprised between 0 and 255
// the scale factor is 62.5 mg/LSB
int Accel::getTapThreshold() {
 byte _b;
 readFrom(ACCEL_THRESH_TAP, 1, &_b); 
 return int (_b);
}

// set/get the gain for each axis in Gs / count
void Accel::setAxisGains(float *_gains){
 int i;
 for(i = 0; i < 3; i++){
  gains[i] = _gains[i];
 }
}
void Accel::getAxisGains(float *_gains){
 int i;
 for(i = 0; i < 3; i++){
  _gains[i] = gains[i];
 }
}


// Sets the OFSX, OFSY and OFSZ bytes
// OFSX, OFSY and OFSZ are user offset adjustments in twos complement format with
// a scale factor of 15,6mg/LSB
// OFSX, OFSY and OFSZ should be comprised between
void Accel::setAxisOffset(int x, int y, int z) {
 writeTo(ACCEL_OFSX, byte (x)); 
 writeTo(ACCEL_OFSY, byte (y)); 
 writeTo(ACCEL_OFSZ, byte (z)); 
}

// Gets the OFSX, OFSY and OFSZ bytes
void Accel::getAxisOffset(int* x, int* y, int*z) {
 byte _b;
 readFrom(ACCEL_OFSX, 1, &_b); 
 *x = int (_b);
 readFrom(ACCEL_OFSY, 1, &_b); 
 *y = int (_b);
 readFrom(ACCEL_OFSZ, 1, &_b); 
 *z = int (_b);
}

// Sets the DUR byte
// The DUR byte contains an unsigned time value representing the maximum time
// that an event must be above THRESH_TAP threshold to qualify as a tap event
// The scale factor is 625µs/LSB
// A value of 0 disables the tap/float tap funcitons. Max value is 255.
void Accel::setTapDuration(int tapDuration) {
 tapDuration = min(max(tapDuration,0),255);
 byte _b = byte (tapDuration);
 writeTo(ACCEL_DUR, _b); 
}

// Gets the DUR byte
int Accel::getTapDuration() {
 byte _b;
 readFrom(ACCEL_DUR, 1, &_b); 
 return int (_b);
}

// Sets the latency (latent register) which contains an unsigned time value
// representing the wait time from the detection of a tap event to the start
// of the time window, during which a possible second tap can be detected.
// The scale factor is 1.25ms/LSB. A value of 0 disables the float tap function.
// It accepts a maximum value of 255.
void Accel::setDoubleTapLatency(int floatTapLatency) {
 byte _b = byte (floatTapLatency);
 writeTo(ACCEL_LATENT, _b); 
}

// Gets the Latent value
int Accel::getDoubleTapLatency() {
 byte _b;
 readFrom(ACCEL_LATENT, 1, &_b); 
 return int (_b);
}

// Sets the Window register, which contains an unsigned time value representing
// the amount of time after the expiration of the latency time (Latent register)
// during which a second valud tap can begin. The scale factor is 1.25ms/LSB. A
// value of 0 disables the float tap function. The maximum value is 255.
void Accel::setDoubleTapWindow(int floatTapWindow) {
 floatTapWindow = min(max(floatTapWindow,0),255);
 byte _b = byte (floatTapWindow);
 writeTo(ACCEL_WINDOW, _b); 
}

// Gets the Window register
int Accel::getDoubleTapWindow() {
 byte _b;
 readFrom(ACCEL_WINDOW, 1, &_b); 
 return int (_b);
}

// Sets the THRESH_ACT byte which holds the threshold value for detecting activity.
// The data format is unsigned, so the magnitude of the activity event is compared
// with the value is compared with the value in the THRESH_ACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the
// activity interrupt is enabled. The maximum value is 255.
void Accel::setActivityThreshold(int activityThreshold) {
 activityThreshold = min(max(activityThreshold,0),255);
 byte _b = byte (activityThreshold);
 writeTo(ACCEL_THRESH_ACT, _b); 
}

// Gets the THRESH_ACT byte
int Accel::getActivityThreshold() {
 byte _b;
 readFrom(ACCEL_THRESH_ACT, 1, &_b); 
 return int (_b);
}

// Sets the THRESH_INACT byte which holds the threshold value for detecting inactivity.
// The data format is unsigned, so the magnitude of the inactivity event is compared
// with the value is compared with the value in the THRESH_INACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the
// inactivity interrupt is enabled. The maximum value is 255.
void Accel::setInactivityThreshold(int inactivityThreshold) {
 inactivityThreshold = min(max(inactivityThreshold,0),255);
 byte _b = byte (inactivityThreshold);
 writeTo(ACCEL_THRESH_INACT, _b); 
}

// Gets the THRESH_INACT byte
int Accel::getInactivityThreshold() {
 byte _b;
 readFrom(ACCEL_THRESH_INACT, 1, &_b); 
 return int (_b);
}

// Sets the TIME_INACT register, which contains an unsigned time value representing the
// amount of time that acceleration must be less thant the value in the THRESH_INACT
// register for inactivity to be declared. The scale factor is 1sec/LSB. The value must
// be between 0 and 255.
void Accel::setTimeInactivity(int timeInactivity) {
 timeInactivity = min(max(timeInactivity,0),255);
 byte _b = byte (timeInactivity);
 writeTo(ACCEL_TIME_INACT, _b); 
}

// Gets the TIME_INACT register
int Accel::getTimeInactivity() {
 byte _b;
 readFrom(ACCEL_TIME_INACT, 1, &_b); 
 return int (_b);
}

// Sets the THRESH_FF register which holds the threshold value, in an unsigned format, for
// free-fall detection. The root-sum-square (RSS) value of all axes is calculated and
// compared whith the value in THRESH_FF to determine if a free-fall event occured. The
// scale factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the free-fall
// interrupt is enabled. The maximum value is 255.
void Accel::setFreeFallThreshold(int freeFallThreshold) {
 freeFallThreshold = min(max(freeFallThreshold,0),255);
 byte _b = byte (freeFallThreshold);
 writeTo(ACCEL_THRESH_FF, _b); 
}

// Gets the THRESH_FF register.
int Accel::getFreeFallThreshold() {
 byte _b;
 readFrom(ACCEL_THRESH_FF, 1, &_b); 
 return int (_b);
}

// Sets the TIME_FF register, which holds an unsigned time value representing the minimum
// time that the RSS value of all axes must be less than THRESH_FF to generate a free-fall
// interrupt. The scale factor is 5ms/LSB. A value of 0 may result in undesirable behavior if
// the free-fall interrupt is enabled. The maximum value is 255.
void Accel::setFreeFallDuration(int freeFallDuration) {
 freeFallDuration = min(max(freeFallDuration,0),255); 
 byte _b = byte (freeFallDuration);
 writeTo(ACCEL_TIME_FF, _b); 
}

// Gets the TIME_FF register.
int Accel::getFreeFallDuration() {
 byte _b;
 readFrom(ACCEL_TIME_FF, 1, &_b); 
 return int (_b);
}

bool Accel::isActivityXEnabled() { 
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 6);
}
bool Accel::isActivityYEnabled() { 
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 5);
}
bool Accel::isActivityZEnabled() { 
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 4);
}
bool Accel::isInactivityXEnabled() { 
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 2);
}
bool Accel::isInactivityYEnabled() { 
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 1);
}
bool Accel::isInactivityZEnabled() { 
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 0);
}

void Accel::setActivityX(bool state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 6, state);
}
void Accel::setActivityY(bool state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 5, state);
}
void Accel::setActivityZ(bool state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 4, state);
}
void Accel::setInactivityX(bool state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 2, state);
}
void Accel::setInactivityY(bool state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 1, state);
}
void Accel::setInactivityZ(bool state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 0, state);
}

bool Accel::isActivityAc() {
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 7);
}
bool Accel::isInactivityAc(){
 return getRegisterBit(ACCEL_ACT_INACT_CTL, 3);
}

void Accel::setActivityAc(bool state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 7, state);
}
void Accel::setInactivityAc(bool state) { 
 setRegisterBit(ACCEL_ACT_INACT_CTL, 3, state);
}

bool Accel::getSuppressBit(){
 return getRegisterBit(ACCEL_TAP_AXES, 3);
}
void Accel::setSuppressBit(bool state) { 
 setRegisterBit(ACCEL_TAP_AXES, 3, state);
}

bool Accel::isTapDetectionOnX(){
 return getRegisterBit(ACCEL_TAP_AXES, 2);
}
void Accel::setTapDetectionOnX(bool state) { 
 setRegisterBit(ACCEL_TAP_AXES, 2, state);
}
bool Accel::isTapDetectionOnY(){
 return getRegisterBit(ACCEL_TAP_AXES, 1);
}
void Accel::setTapDetectionOnY(bool state) { 
 setRegisterBit(ACCEL_TAP_AXES, 1, state);
}
bool Accel::isTapDetectionOnZ(){
 return getRegisterBit(ACCEL_TAP_AXES, 0);
}
void Accel::setTapDetectionOnZ(bool state) { 
 setRegisterBit(ACCEL_TAP_AXES, 0, state);
}

bool Accel::isActivitySourceOnX(){
 return getRegisterBit(ACCEL_ACT_TAP_STATUS, 6);
}
bool Accel::isActivitySourceOnY(){
 return getRegisterBit(ACCEL_ACT_TAP_STATUS, 5);
}
bool Accel::isActivitySourceOnZ(){
 return getRegisterBit(ACCEL_ACT_TAP_STATUS, 4);
}

bool Accel::isTapSourceOnX(){
 return getRegisterBit(ACCEL_ACT_TAP_STATUS, 2);
}
bool Accel::isTapSourceOnY(){
 return getRegisterBit(ACCEL_ACT_TAP_STATUS, 1);
}
bool Accel::isTapSourceOnZ(){
 return getRegisterBit(ACCEL_ACT_TAP_STATUS, 0);
}

bool Accel::isAsleep(){
 return getRegisterBit(ACCEL_ACT_TAP_STATUS, 3);
}

bool Accel::isLowPower(){
 return getRegisterBit(ACCEL_BW_RATE, 4);
}
void Accel::setLowPower(bool state) { 
 setRegisterBit(ACCEL_BW_RATE, 4, state);
}

float Accel::getRate(){
 byte _b;
 readFrom(ACCEL_BW_RATE, 1, &_b);
 _b &= B00001111;
 return (pow(2,((int) _b)-6)) * 6.25;
}

void Accel::setRate(float rate){
 byte _b,_s;
 int v = (int) (rate / 6.25);
 int r = 0;
 while (v >>= 1)
 {
  r++;
 }
 if (r <= 9) {
  readFrom(ACCEL_BW_RATE, 1, &_b);
  _s = (byte) (r + 6) | (_b & B11110000);
  writeTo(ACCEL_BW_RATE, _s);
 }
}

void Accel::set_bw(byte bw_code){
 if((bw_code < ACCEL_BW_3) || (bw_code > Accel_BW_1600)){
  status = false;
  error_code = ACCEL_BAD_ARG;
 }
 else{
  writeTo(ACCEL_BW_RATE, bw_code);
 }
}

byte Accel::get_bw_code(){
 byte bw_code;
 readFrom(ACCEL_BW_RATE, 1, &bw_code);
 return bw_code;
}

byte Accel::getInterruptSource() {
 byte _b;
 readFrom(ACCEL_INT_SOURCE, 1, &_b);
 return _b;
}

bool Accel::getInterruptSource(byte interruptBit) {
 return getRegisterBit(ACCEL_INT_SOURCE,interruptBit);
}

bool Accel::getInterruptMapping(byte interruptBit) {
 return getRegisterBit(ACCEL_INT_MAP,interruptBit);
}

// Set the mapping of an interrupt to pin1 or pin2
// eg: setInterruptMapping(ACCEL_INT_DOUBLE_TAP_BIT,ACCEL_INT2_PIN);
void Accel::setInterruptMapping(byte interruptBit, bool interruptPin) {
 setRegisterBit(ACCEL_INT_MAP, interruptBit, interruptPin);
}

bool Accel::isInterruptEnabled(byte interruptBit) {
 return getRegisterBit(ACCEL_INT_ENABLE,interruptBit);
}

void Accel::setInterrupt(byte interruptBit, bool state) {
 setRegisterBit(ACCEL_INT_ENABLE, interruptBit, state);
}

void Accel::setRegisterBit(byte regAdress, int bitPos, bool state) {
 byte _b;
 readFrom(regAdress, 1, &_b);
 if (state) {
  _b |= (1 << bitPos); // forces nth bit of _b to be 1. all other bits left alone.
 }
 else {
  _b &= ~(1 << bitPos); // forces nth bit of _b to be 0. all other bits left alone.
 }
 writeTo(regAdress, _b); 
}

bool Accel::getRegisterBit(byte regAdress, int bitPos) {
 byte _b;
 readFrom(regAdress, 1, &_b);
 return ((_b >> bitPos) & 1);
}

// print all register value to the serial ouptut, which requires it to be setup
// this can be used to manually to check the current configuration of the device
void Accel::printAllRegister() {
 byte _b;
 Serial.print("0x00: ");
 readFrom(0x00, 1, &_b);
 print_byte(_b);
 Serial.println("");
 int i;
 for (i=29;i<=57;i++){
  Serial.print("0x");
  Serial.print(i, HEX);
  Serial.print(": ");
  readFrom(i, 1, &_b);
  print_byte(_b);
  Serial.println("");  
 }
}

void print_byte(byte val){
 int i;
 Serial.print("B");
 for(i=7; i>=0; i--){
  Serial.print(val >> i & 1, BIN);
 }
}