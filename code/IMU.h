/* ----------------------------------- Header for Inertial Measurement Unit -------------------------------------*/

#include <Wire.h>
#include "Arduino.h"

#include <Accel.h>
#define IMU_ACCEL_ADDR ACCEL_ADDR_ALT_LOW // SDO connected to GND
#include <Gyro.h>
#define IMU_GYRO_DEF_ADDR GYRO_ADDR_AD0_LOW // AD0 connected to GND

#ifndef IMU_h
#define IMU_h

#define FIMU_BMA180_DEF_ADDR BMA180_ADDRESS_SDO_LOW

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
    void init(int acc_addr, int gyro_addr, bool fastmode);
    void getRawValues(int * raw_values);
    void getValues(float * values);
    void getQ(float * q);
    void getEuler(float * angles);
    void getYawPitchRoll(float * ypr);
    void getAngles(float * angles);
    
    Accel accel;
    Gyro gyro;
    
    int* raw_acc, raw_gyro, raw_magn;
    
private:
    void AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
    //float q0, q1, q2, q3; // quaternion elements representing the estimated orientation
    uint64_t iq0, iq1, iq2, iq3;
    uint64_t exInt, eyInt, ezInt;  // scaled integral error
    volatile uint64_t twoKp;      // 2 * proportional gain (Kp)
    volatile uint64_t twoKi;      // 2 * integral gain (Ki)
    volatile uint64_t q0, q1, q2, q3; // quaternion of sensor frame relative to auxiliary frame
    volatile uint64_t integralFBx,  integralFBy, integralFBz;
    unsigned long lastUpdate, now; // sample period expressed in milliseconds
    uint64_t sampleFreq; // half the sample period expressed in seconds
    int startLoopTime;
};

float invSqrt(float number);

#endif //IMU_h
