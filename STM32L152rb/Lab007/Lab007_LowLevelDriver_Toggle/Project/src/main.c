#include "stm32l1xx.h"

#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_gpio.h"

int main(){
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	while(1)
	{
		LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_6);
		LL_mDelay(1000);
	}
}