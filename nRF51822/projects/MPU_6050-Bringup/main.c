/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 * @defgroup tw_scanner main.c
 * @{
 * @ingroup nrf_twi_example
 * @brief TWI Sensor Example main file.
 *
 * This file contains the source code for a sample application using TWI.
 *
 */

#include <stdio.h>

#include "TOTEM_V1.h"
#include "mpu6050/MPU6050.h"
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_delay.h"


#define NRF_LOG_MODULE_NAME "APP"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"

/* TWI instance ID. */
#define TWI_INSTANCE_ID     0

 /* Number of possible TWI addresses. */
 #define TWI_ADDRESSES      127

/* TWI instance. */


/**
 * @brief TWI initialization.
 */



/**
 * @brief Function for main application entry.
 */
int main(void)
{
    //ret_code_t err_code;
    //uint8_t address;
    mpu_result_t result;
    //uint8_t WhoAmI = GYRO_REG_WHOAMI;
    uint8_t regGYROX = GYRO_REG_ACCEL_XOUT_H;
    //uint8_t sample_data =0;
    //bool detected_device = false;
//
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    LEDS_CONFIGURE(1<<(LED_PIN));
    NRF_LOG_INFO("TWI scanner.\r\n");
    NRF_LOG_FLUSH();

    result = mpu_init();
    if(result != MPU_OK){
        NRF_LOG_ERROR("Could not initialize MPU. Err: %d",result);
        for(;;);
    }
    result = mpu_testConnection();
    if(result != MPU_OK){
        NRF_LOG_ERROR("MPU did send proper response to WHOAMI request");
        for(;;);
    }

//
//    for (address = 1; address <= TWI_ADDRESSES; address++)
//    {
//        err_code = nrf_drv_twi_rx(&m_twi, address, &sample_data, sizeof(sample_data));
//        if (err_code == NRF_SUCCESS)
//        {
//            detected_device = true;
//            NRF_LOG_INFO("TWI device detected at address 0x%x.\r\n", address);
//        }
//        NRF_LOG_FLUSH();
//    }
//
    while (true)
    {
        regGYROX = GYRO_REG_GYRO_XOUT_H;
        //LEDS_INVERT(1<<LED_PIN);
        uint16_t x;
        //mpu_readRegister(WhoAmI,&sample_data,sizeof(sample_data));
        //NRF_LOG_INFO("Device returned: %d\r\n",sample_data);
        mpu_readRegister(regGYROX,(uint8_t*)&x,sizeof(uint8_t));
        regGYROX++;
        mpu_readRegister(regGYROX,(uint8_t*)(&x+1),sizeof(uint8_t));
        NRF_LOG_INFO("X value: %d\r\n",x);
        NRF_LOG_FLUSH();
        nrf_delay_ms(50);
        //mpu6050_init(0x69);
        /* Empty loop. */
    }
}

/** @} */
