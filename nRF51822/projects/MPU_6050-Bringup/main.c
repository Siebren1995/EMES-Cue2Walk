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
void BROKEN(){
    NRF_LOG_FLUSH();
    while(true){
        LEDS_INVERT(1<<LED_PIN);
        nrf_delay_ms(1000);
    }

}
int16_t sensorValues[3];
int main(void)
{
    mpu_result_t result;
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    LEDS_CONFIGURE(1<<(LED_PIN));
    NRF_LOG_INFO("TWI scanner.\r\n");
    NRF_LOG_FLUSH();

    result = mpu_init();
    if(result != MPU_OK){
        NRF_LOG_ERROR("Could not initialize MPU. Err: %d",result);
        BROKEN();
    }

    while (true)
    {
        LEDS_INVERT(1<<LED_PIN);
        //mpu_readRegister(WhoAmI,&sample_data,sizeof(sample_data));
        //NRF_LOG_INFO("Device returned: %d\r\n",sample_data);
//        if(mpu_readRegister(regGYROX,&x,sizeof(uint8_t))!=MPU_OK){
//            NRF_LOG_ERROR("Something went wrong while reading from MPU\n");
//            BROKEN();
//        }
        mpu_readAccelero((int8_t*)sensorValues);
//        NRF_LOG_INFO("\r\nX: %d\r\nY: %d\r\nZ: %d\r\n",sensorValues[0],sensorValues[1], sensorValues[2]);
        NRF_LOG_INFO(",%d\r\n",sensorValues[0]);
        NRF_LOG_FLUSH();
        nrf_delay_ms(50);

    }
}

/** @} */
