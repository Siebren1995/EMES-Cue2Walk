
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
uint16_t buffer[10]; //static buffer for algorithm
int position;
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
    position = 0;
    while (true)
    {
        LEDS_INVERT(1<<LED_PIN);

        mpu_readAccelero((int8_t*)buffer[position]);
//        NRF_LOG_INFO("\r\nX: %d\r\nY: %d\r\nZ: %d\r\n",sensorValues[0],sensorValues[1], sensorValues[2]);
        NRF_LOG_INFO(",%d\r\n",buffer[position]);
        NRF_LOG_FLUSH();
        //nrf_delay_ms(50);
        position++;
        if(position > 9){
        	position = 0;
        }

    }
}

/** @} */
