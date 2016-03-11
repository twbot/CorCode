#include "Magnometer.h"

/*
I2C-Bus Specification, document number: 9398 393 40011;

Supports standard and fast modes, 100kHz and 400kHz, 
respectively, but does not support the high speed mode (Hs);

External pull-up resistors are required to support standard and fast speed modes;
2kohm used for fast mode;   10kohms for standard;

I2C addresses: 8-bit addresses: even-addresses--write-only: odd addresses--read-only;

This means that you can have up to 128 devices on the I2C bus, since a 7bit number can be from 0 to 127;
When sending out the 7 bit address, we still always send 8 bits;
The extra bit is used to inform the slave if the master is  writing to it or reading from it;
If the bit is zero the master is writing to the slave. If the bit is 1 the master is reading from the slave;
The 7 bit address is placed in the upper 7 bits of the byte and the Read/Write (R/W) bit is in the LSB (Least Significant Bit);

Use 0x1E to write to the Magnometer and 0x1F to read from it;

To write to a slave device: 
1. Send a start sequence
2. Send the I2C address of the slave with the R/W bit low (even address)
3. Send the internal register number you want to write to
4. Send the data byte
5. [Optionally, send any further data bytes]
6. Send the stop sequence
*/

Magnometer::Magnometer(){
 m_Scale = 1;
}

Magnometer::readData(){
    uint8_t* buffer = Read(DataRegisterBegin, 6);
}
 MagnometerRaw Magnometer::ReadRawAxis(){
 uint8_t* buffer = Read(DataRegisterBegin, 6);
 MagnetometerRaw raw = MagnetometerRaw();
 raw.XAxis = (buffer[0] << 8) | buffer[1];
 raw.ZAxis = (buffer[2] << 8) | buffer[3];
 raw.YAxis = (buffer[4] << 8) | buffer[5];
 return raw;
}

MagnetometerScaled Magnometer::ReadScaledAxis(){
 MagnetometerRaw raw = ReadRawAxis();
 MagnetometerScaled scaled = MagnetometerScaled();
 scaled.XAxis = raw.XAxis * m_Scale;
 scaled.ZAxis = raw.ZAxis * m_Scale;
 scaled.YAxis = raw.YAxis * m_Scale;
 return scaled;
}

int Magnometer::SetScale(float gauss){
    uint8_t regValue = 0x00;
    if(gauss == 0.88){
        regValue = 0x00;
        m_Scale = 0.73;
    }
    else if(gauss == 1.3){
        regValue = 0x01;
        m_Scale = 0.92;
    }
    else if(gauss == 1.9){
        regValue = 0x02;
        m_Scale = 1.22;
    }
    else if(gauss == 2.5){
        regValue = 0x03;
        m_Scale = 1.52;
    }
    else if(gauss == 4.0){
        regValue = 0x04;
        m_Scale = 2.27;
    }
    else if(gauss == 4.7){
        regValue = 0x05;
        m_Scale = 2.56;
    }
    else if(gauss == 5.6){
        regValue = 0x06;
        m_Scale = 3.03;
    }
    else if(gauss == 8.1){
        regValue = 0x07;
        m_Scale = 4.35;
    }
    else{
        return MAG_ERROR;
    }
    // Setting is in the top 3 bits of the register.
    regValue = regValue << 5;
    Write(ConfigurationRegisterB, regValue);
}

int Magnometer::SetMeasurementMode(uint8_t mode){
    Write(ModeRegister, mode);
}

void Magnometer::Write(int address, int data){
 Wire.beginTransmission(ADDRESS);
 Wire.write(address);
 Wire.write(data);
 Wire.endTransmission();
}

uint8_t* Magnometer::Read(int address, int length){
 Wire.beginTransmission(ADDRESS);
 Wire.write(address);
 Wire.endTransmission();
 
 Wire.beginTransmission();
 Wire.requestFrom(ADDRESS, length);

 uint8_t buffer[length];
 if(Wire.available() == length)
 {
     for(uint8_t i = 0; i < length; i++)
     {
         buffer[i] = Wire.read();
     }
 }
 Wire.endTransmission();

 return buffer;
}

char* Magnometer::GetErrorText(int errorCode){
    if(ErrorCode_1_Num == 1)
        return ErrorCode_1;
    
    return "Error not defined.";
}