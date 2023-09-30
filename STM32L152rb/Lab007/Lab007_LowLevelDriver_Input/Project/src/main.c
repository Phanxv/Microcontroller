//Include driver's header files
#include "stm32l1xx.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_gpio.h"

int main(){
	int usr_btn; //declare variable storing input value
	LL_GPIO_InitTypeDef GPIO_PA0_InitStruct, GPIO_PB6_InitStruct, GPIO_PB7_InitStruct; //declare structure for config GPIO PA0, GPIO PB6 and GPIO PB7
	
	//Enable clock signal for GPIOA and GPIOB
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); 
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	
	//Configuring pin PA0
	GPIO_PA0_InitStruct.Mode = LL_GPIO_MODE_INPUT; //setup pin as input mode
	GPIO_PA0_InitStruct.Pin = LL_GPIO_PIN_8; //select pin 0 of GPIO PA
	GPIO_PA0_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL; //set output type to push-pull
	GPIO_PA0_InitStruct.Pull = LL_GPIO_PULL_NO; //set no pull
	GPIO_PA0_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH; //set frequency to HIGH frequency mode
	//Configuring pin PB6
	GPIO_PB6_InitStruct.Mode = LL_GPIO_MODE_OUTPUT; //setup pin as output mode
	GPIO_PB6_InitStruct.Pin = LL_GPIO_PIN_6; //select pin 6 of GPIO PB
	GPIO_PB6_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL; //set output type to push-pull
	GPIO_PB6_InitStruct.Pull = LL_GPIO_PULL_NO; //set no pull
	GPIO_PB6_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH; //set frequency to HIGH frequency mode
	//Configuring pin PB7
	GPIO_PB7_InitStruct.Mode = LL_GPIO_MODE_OUTPUT; //setup pin as output mode
	GPIO_PB7_InitStruct.Pin = LL_GPIO_PIN_7; //select pin 7 of GPIO PB
	GPIO_PB7_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL; //set output type to push-pull
	GPIO_PB7_InitStruct.Pull = LL_GPIO_PULL_NO; //set no pull
	GPIO_PB7_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH; //set frequency to HIGH frequency mode
	
	//use the initial structure to initilize the GPIO
	LL_GPIO_Init(GPIOB, &GPIO_PA0_InitStruct);
	LL_GPIO_Init(GPIOB, &GPIO_PB6_InitStruct);
	LL_GPIO_Init(GPIOB, &GPIO_PB7_InitStruct);
	
	//loop
	while(1)
	{
		usr_btn = LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_8); //read input from pin PA0 and storing it in usr_btn variable
		//condition
		if (usr_btn == 1){ //if usr_btn read 1
			//toggle PB6, PB7
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7);
		}
		else {
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
		}
	}
}