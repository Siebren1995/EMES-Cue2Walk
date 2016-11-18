#ifndef __MPU_6050_H
#define __MPU_6050_H

#include "nrf_drv_twi.h"
#include "TOTEM_V1.h"

/*configuration Registers*/
#define GYRO_REG_CONFIG_DIVIDER 0x19
#define GYRO_CONFIG             0x1A
#define GYRO_REG_CONFIG_GYRO    0x1B
#define GYRO_REG_CONFIG_ACCEL   0x1C
#define GYRO_REG_CONFIG_FIFO    0x23
#define GYRO_REG_CONFIG_PWR     0x6B

/*Accelero-Sensor output registers*/
#define GYRO_REG_ACCEL_XOUT_H   0x3B
#define GYRO_REG_ACCEL_XOUT_L   0x3C
#define GYRO_REG_ACCEL_YOUT_H   0x3D
#define GYRO_REG_ACCEL_YOUT_L   0x3E
#define GYRO_REG_ACCEL_ZOUT_H   0x3F
#define GYRO_REG_ACCEL_ZOUT_L   0x40

/*Gyro-Sensor output registers*/
#define GYRO_REG_GYRO_XOUT_H   0x43
#define GYRO_REG_GYRO_XOUT_L   0x44
#define GYRO_REG_GYRO_YOUT_H   0x45
#define GYRO_REG_GYRO_YOUT_L   0x46
#define GYRO_REG_GYRO_ZOUT_H   0x47
#define GYRO_REG_GYRO_ZOUT_L   0x48

#define GYRO_REG_WHOAMI    0x75

/*I2C settings*/
#define TWI_INSTANCE_ID     0

typedef enum mpu_result_s{
    MPU_OK = 0,
    MPU_ERR = -1,
} mpu_result_t;

mpu_result_t mpu_init(void);
mpu_result_t mpu_readRegister(uint8_t mpu_register, uint8_t* buffer, uint8_t buffersize);
mpu_result_t mpu_writeRegister(uint8_t mpu_register, uint8_t value, bool stopbit);

mpu_result_t mpu_testConnection(void);



#endif
