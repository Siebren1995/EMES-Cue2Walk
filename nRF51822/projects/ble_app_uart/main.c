/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
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
 *
 * @defgroup ble_sdk_uart_over_ble_main main.c
 * @{
 * @ingroup  ble_sdk_app_nus_eval
 * @brief    UART over BLE application main file.
 *
 * This file contains the source code for a sample application that uses the Nordic UART service.
 * This application uses the @ref srvlib_conn_params module.
 */

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "TOTEM_V1.h"
#include "nrf.h"


#include "app_button.h"
#include "app_util_platform.h"
#include "bsp.h"
#include "bsp_btn_ble.h"
#include "nrf_delay.h"

#include "ble_nus.h"
#include "bluetooth.h"
#include "ble_types.h"
#include "ble_advertising.h"
#define NRF_LOG_MODULE_NAME "APP"


#include "nrf_log.h"
#include "nrf_log_ctrl.h"
uint8_t trigger_send = 0;
ble_nus_t                        m_nus;                                      /**< Structure to identify the Nordic UART Service. */
uint16_t                         m_conn_handle = BLE_CONN_HANDLE_INVALID;    /**< Handle of the current connection. */

APP_TIMER_DEF(m_timer_peak_id);

uint16_t buffer[10]; //static buffer for algorithm
int position,i,tempPosR,tempPosF,maxBefore,maxAfter;
int filter[4] = {3,1,1,1};
int holdoff = 0;
const int threshold = 30000;
static void peakhandler(void * context){
	uint8_t getal;
	LEDS_INVERT(1<<LED_PIN);
	if(!holdoff){
		getal = 2;
	}
	else{
		getal = 0;

	}
	ble_nus_string_send(&m_nus,&getal,1);
	holdoff = 0;
}

void detectFreeze(){
	maxBefore = 0;
	maxAfter = 0;
	for(i = 0; i < 5; i++){
	/*check bounds of array*/
		tempPosF = position + i;
		tempPosR = position - i;
		if(tempPosR < 0){
			tempPosR += 10;
		}
		if(tempPosF > 9){
			tempPosR -= 10;
		}
		if(filter[i]*buffer[tempPosR] > maxBefore){
			maxBefore = buffer[tempPosR];
		}
		if(filter[i]*buffer[tempPosF] > maxAfter){
			maxAfter = buffer[tempPosF];
		}
	}
	if((5*buffer[position] > maxBefore) && (5*buffer[position] > maxAfter) && (buffer[position] > threshold)){

		holdoff++;
	}
}
void BROKEN(){
    NRF_LOG_FLUSH();
    while(true){
        LEDS_INVERT(1<<LED_PIN);
        nrf_delay_ms(1000);
    }

}

/**@brief Function for assert macro callback.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyse
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num    Line number of the failing ASSERT call.
 * @param[in] p_file_name File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}



/**@brief Function for handling the data from the Nordic UART Service.
 *
 * @details This function will process the data received from the Nordic UART BLE Service and send
 *          it to the UART module.
 *
 * @param[in] p_nus    Nordic UART Service structure.
 * @param[in] p_data   Data to be send to UART module.
 * @param[in] length   Length of the data.
 */
/**@snippet [Handling the data received over BLE] */
static void nus_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length)
{

    //NRF_LOG_INFO("Received: %s\r\n",(int)temp);
   // NRF_LOG_FLUSH();
    ble_nus_string_send(&m_nus, p_data, length);
}
/**@snippet [Handling the data received over BLE] */


/**@brief Application main function.
 */
int main(void)
{

    uint32_t err_code;
    mpu_result_t result;
    /*start the RTT logger*/
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    /*configure LED on board as output*/
    LEDS_CONFIGURE(1<<(LED_PIN));
    /*initialize some timer*/
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
    /*wait one second for the accelerosensor to come up*/
    /*this is needed when board is power cycled*/
    nrf_delay_ms(1000);
    /*try to initialize accelerosensor*/
    result = mpu_init();
    if(result != MPU_OK){
        NRF_LOG_ERROR("Could not initialize MPU. Err: %d",result);
        BROKEN();
    }

    /*Start bluetooth related functions*/
    ble_stack_init();
    gap_params_init();
    /*start Nordic UART service, with our own handler*/
    services_init((void*)nus_data_handler);
    advertising_init();
    conn_params_init();
    //NRF_LOG_INFO("Started BLE UART service\r\n");
    //NRF_LOG_FLUSH();
    err_code = app_timer_create(&m_timer_peak_id,APP_TIMER_MODE_REPEATED,peakhandler);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_start(m_timer_peak_id, APP_TIMER_TICKS(1333, APP_TIMER_PRESCALER), NULL);
    APP_ERROR_CHECK(err_code);
    err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);



    // Enter main loop.

    position = 0;
        while (true)
        {
        	/*Read accelero value from sensor into buffer*/
            mpu_readAccelero((int8_t*)&buffer[position]);
            /*detect Peak*/
            detectFreeze();
            /*write the accelero value to RTT*/
            //NRF_LOG_INFO(",%d\r\n",buffer[position]);
            //NRF_LOG_FLUSH();
            /*increment buffer position*/
            position++;
            /*check whether we are not going outside of buffer*/
            if(position > 9){
            	position = 0;
            }

        }
}

