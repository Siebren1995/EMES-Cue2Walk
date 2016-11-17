#ifndef __BOARD_H
#define __BOARD_H

/*this file contains board specific defines, such as debug LED pin*/
#define ANY_PORT 0 /*device only has 1 port, which is 0*/

/*LED*/
#define LED_PIN 21

/*MPU6050*/
#define GYRO_PIN_INT 29
#define GYRO_PIN_SCL 2
#define GYRO_PIN_SDA 3

/*Temp Sensor*/
#define TEMP_PIN_SCL GYRO_PIN_SCL
#define TEMP_PIN_SDA GYRO_PIN_SDA

/*SD-Card Slot*/
#define SD_PIN_CS   22
#define SD_PIN_DI   23
#define SD_PIN_SCLK 24
#define SD_PIN_DO   25

#endif /*board.h*/
