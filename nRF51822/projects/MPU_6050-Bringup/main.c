#include <stdio.h>
#include "TOTEM_V1.h"
#include "mpu6050/MPU6050.h"
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"

#define NRF_LOG_MODULE_NAME "APP"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
uint16_t buffer[10]; //static buffer for algorithm
int position,i,tempPosR,tempPosF,maxBefore,maxAfter;
uint32_t printtimer,printtimer2,printtimer3,printtimer4;
int filter[4] = {3,1,1,1};
const int threshold = 30000;

//volatile int *timervalue;

void start_timer(void)
{
  NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;  // Set the timer in Counter Mode
  NRF_TIMER2->TASKS_CLEAR = 1;               // clear the task first to be usable for later
  NRF_TIMER2->PRESCALER = 6;
  NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;//0x01UL;										//Set counter to 16 bit resolution
  NRF_TIMER2->CC[0] = 25000;
  NRF_TIMER2->CC[1] = 5;

  // Enable interrupt on Timer 2
//	NRF_TIMER2->INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos) | (TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos);
//  NVIC_EnableIRQ(TIMER2_IRQn);


  NRF_TIMER2->TASKS_START = 1;               // Start timer
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
		//LEDS_INVERT(1<<LED_PIN);
	}
}
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
//	timervalue = 0x4000A000;
   // nrf_delay_ms(1000);

  //  mpu_result_t result;
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    LEDS_CONFIGURE(1<<(LED_PIN));
    NRF_LOG_INFO("TWI scanner.\r\n");
    NRF_LOG_FLUSH();

	nrf_gpio_port_dir_set(NRF_GPIO_PORT_SELECT_PORT1, NRF_GPIO_PORT_DIR_OUTPUT);
	start_timer();

//    result = mpu_init();
//    if(result != MPU_OK){
//        NRF_LOG_ERROR("Could not initialize MPU. Err: %d",result);
//        BROKEN();
//    }
    position = 0;

//    volatile int val = *timervalue;

    while (true)
    {
	    NRF_TIMER2->TASKS_CAPTURE[3] = 1;
    	NRF_TIMER2->TASKS_COUNT = 1;
    	printtimer = NRF_TIMER2->CC[3];

    	NRF_LOG_INFO(",%d\r\n",printtimer);
    	NRF_LOG_FLUSH();

//    	val = *timervalue;
//        LEDS_INVERT(1<<LED_PIN);
//
//        mpu_readAccelero((int8_t*)&buffer[position]);
//        detectFreeze();
//        NRF_LOG_INFO("\r\nX: %d\r\nY: %d\r\nZ: %d\r\n",sensorValues[0],sensorValues[1], sensorValues[2]);
//        NRF_LOG_INFO(",%d\r\n",buffer[position]);
//       NRF_LOG_FLUSH();
//        nrf_delay_ms(50);
//        position++;
//        if(position > 9){
//        	position = 0;
//        }

    }
}
