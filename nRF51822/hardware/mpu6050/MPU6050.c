/*
 * MPU6050.c
 *
 *  Created on: Nov 18, 2016
 *      Author: diederik
 */
#include "MPU6050.h"
#include "app_util_platform.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

#define MPU_READ(data,size) nrf_drv_twi_rx(&m_twi, GYRO_ADDRESS,data,size)
#define MPU_WRITE(data,size,stop) nrf_drv_twi_tx(&m_twi, GYRO_ADDRESS,data,size,stop)
#define MPU_WRITE_DATA(data,size,stop) nrf_drv_twi_tx(&m_twi, GYRO_ADDRESS-1,data,size,stop)

const nrf_drv_twi_config_t twi_config = {
   .scl                = GYRO_PIN_SCL,
   .sda                = GYRO_PIN_SDA,
   .frequency          = NRF_TWI_FREQ_100K,
   .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
   .clear_bus_init     = false
};




mpu_result_t mpu_init (void)
{

uint8_t reg = GYRO_REG_CONFIG_PWR;
uint8_t data = 0x0;
    if(nrf_drv_twi_init(&m_twi, &twi_config, NULL, NULL) != NRF_SUCCESS){
        return MPU_ERR;
    }
    else{
        nrf_drv_twi_enable(&m_twi);
        if(mpu_testConnection()!= MPU_OK){
            return MPU_ERR;
        }
        else {
            /*set divider*/
            mpu_writeRegister(reg,data,true);
            mpu_readRegister(reg,&data,sizeof(data));
            NRF_LOG_INFO("Power register value: %d\r\n",data);
            reg = GYRO_REG_CONFIG_DIVIDER;
            mpu_readRegister(reg,&data,sizeof(data));
            NRF_LOG_INFO("divider register value: %d\r\n",data);
            NRF_LOG_FLUSH();
            /*set config registers*/

            /*set the FIFO registers*/

            /*configure interrupts*/

            /*read*/
            return MPU_OK;
        }

    }
}
mpu_result_t mpu_readRegister(uint8_t mpu_register, uint8_t* buffer, uint8_t buffersize){

    if(MPU_WRITE(&mpu_register,sizeof(mpu_register),false) != NRF_SUCCESS){
        return MPU_ERR;
    }
    else if(MPU_READ(buffer,buffersize)!= NRF_SUCCESS){
        return MPU_ERR;
    }
    else {
        return MPU_OK;
    }
}
mpu_result_t mpu_writeRegister(uint8_t mpu_register, uint8_t value, bool stopbit){
    uint16_t input = (mpu_register<<8)| (value);
    if(MPU_WRITE_DATA((uint8_t*)&input,sizeof(input),stopbit)!= NRF_SUCCESS){
        return MPU_ERR;
    }
    else {
        return MPU_OK;
    }
}
/*read WHOAMI*/
mpu_result_t mpu_testConnection(void){
    uint8_t result = 0;
    uint8_t mpu_register = GYRO_REG_WHOAMI;
    MPU_WRITE(&mpu_register,sizeof(mpu_register),true);
    MPU_READ(&result,sizeof(result));
    if(result == 0x68){
        return MPU_OK;
    }
    else{
        return MPU_ERR;
    }
}


