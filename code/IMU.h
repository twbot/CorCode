/* ----------------------------------- Header for Inertial Measurement Unit -------------------------------------*/

#include <Wire.h>
#include "Arduino.h"

#include <Accel.h>
#define IMU_ACCEL_ADDR ACCEL_ADDR_ALT_LOW // SDO connected to GND
#include <Gyro.h>
#define IMU_GYRO_ADDR GYRO_ADDR_AD0_LOW // AD0 connected to GND

#ifndef IMU_h
#define IMU_h

#define twoKpDef  (2.0f * 0.5f) // 2 * proportional gain
#define twoKiDef  (2.0f * 0.1f) // 2 * integral gain

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

class IMU {
public:
    IMU();
    void init();
    void init(uint8_t fastmode);
    void init(uint16_t accAddr, uint16_t gyroAddr, uint16_t fastmode);
    void getRawValues(int * rawValues);
    void getValues(uint32_t * values);
    void getQ(uint32_t * q);
    void getEuler(uint32_t * angles);
    void getYawPitchRoll(uint32_t * ypr);
    void getAngles(uint32_t * angles);
    
    Accel accel;
    Gyro gyro;
    
    uint16_t* rawAcc, rawGyro;
    
private:
    void AHRSupdate(uint32_t gx, uint32_t gy, uint32_t gz, uint32_t ax, uint32_t ay, uint32_t az);
    //uint32_t q0, q1, q2, q3; // quaternion elements representing the estimated orientation
    uint32_t iq0, iq1, iq2, iq3;
    uint32_t exInt, eyInt, ezInt;  // scaled integral error
    volatile uint32_t twoKp;      // 2 * proportional gain (Kp)
    volatile uint32_t twoKi;      // 2 * integral gain (Ki)
    volatile uint32_t q0, q1, q2, q3; // quaternion of sensor frame relative to auxiliary frame
    volatile uint32_t integralFBx,  integralFBy, integralFBz;
    uint32_t lastUpdate, now; // sample period expressed in milliseconds
    uint32_t sampleFreq; // half the sample period expressed in seconds
    uint16_t startLoopTime;
};

uint32_t invSqrt(uint32_t number);

#endif //IMU_h
