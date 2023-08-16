#include "stm32l1xx.h"

#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_gpio.h"

#include "stm32l1xx_ll_lcd.h"
#include "stm32l152_glass_lcd.h"
#include "stdio.h"

void SystemClock_Config();
char disp_str[7];
	
int main(){
	int current_btn, last_btn, state=0; //declare variable storing input value
	LL_GPIO_InitTypeDef GPIO_PA0_InitStruct; //declare structure for config GPIO PA0

	//Enable clock signal for GPIOA and config clock for system
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); 
	SystemClock_Config();
	
	//Configuring pin PA0
	GPIO_PA0_InitStruct.Mode = LL_GPIO_MODE_INPUT; //setup pin as input mode
	GPIO_PA0_InitStruct.Pin = LL_GPIO_PIN_0; //select pin 0 of GPIO PA
	GPIO_PA0_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL; //set output type to push-pull
	GPIO_PA0_InitStruct.Pull = LL_GPIO_PULL_NO; //set no pull
	GPIO_PA0_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH; //set frequency to HIGH frequency mode
	
	//use the initial structure to initilize the GPIO
	LL_GPIO_Init(GPIOA, &GPIO_PA0_InitStruct);

	//Initialize LCD screen
	LCD_GLASS_Init();
	
	while(1)
	{
		last_btn = current_btn;
		current_btn = LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0); //read input from pin PA0 and storing it in usr_btn variable
		//condition
		if (last_btn == 1 && current_btn == 0){ //check for button state with some debouncing logic
			state = !state;
			if(state == 1){
				sprintf(disp_str, "LD:ON "); //display LD:ON
				LCD_GLASS_DisplayString((uint8_t*)disp_str);
			}
			else{
				sprintf(disp_str, "LD:OFF"); //display LD:OFF
				LCD_GLASS_DisplayString((uint8_t*)disp_str);
			}
		}
	}
}

void SystemClock_Config(void)
{
  /* Enable ACC64 access and set FLASH latency */ 
  LL_FLASH_Enable64bitAccess();; 
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  /* Set Voltage scale1 as MCU will run at 32MHz */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  
  /* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
  while (LL_PWR_IsActiveFlag_VOSF() != 0)
  {
  };
  
  /* Enable HSI if not already activated*/
  if (LL_RCC_HSI_IsReady() == 0)
  {
    /* HSI configuration and activation */
    LL_RCC_HSI_Enable();
    while(LL_RCC_HSI_IsReady() != 1)
    {
    };
  }
  
	
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_3);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 32MHz                             */
  /* This frequency can be calculated through LL RCC macro                          */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ (HSI_VALUE, LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_3); */
  LL_Init1msTick(32000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(32000000);
}
