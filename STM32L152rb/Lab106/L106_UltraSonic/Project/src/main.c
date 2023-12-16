#include "stm32l1xx.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_tim.h"
#include "stdio.h"

void SystemClock_Config(void);
void ECHO_Config(void);
void TRIG_Config(void);
void TIMx_IC_Config(void);
void TIM2_IRQHandler(void);
void seg7_display(char str[5]);
void seg7_gpio_config();

uint16_t uwIC1 = 0;
uint16_t uwIC2 = 0;
uint16_t uwDiff = 0;
uint16_t uhICIndex = 0;

uint32_t TIM2CLK;
uint32_t PSC;
uint32_t IC1PSC;

float period = 0;
float distance = 0;

char disp_str[7];

int main()
{
	SystemClock_Config();
	TIMx_IC_Config();
	ECHO_Config();
	TRIG_Config();
	seg7_gpio_config();
	
	while(1)
	{
		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_2);
		LL_mDelay(1);
		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);
		if(uhICIndex == 2)
		{
			//Period calculation
			PSC = LL_TIM_GetPrescaler(TIM2) + 1;
			TIM2CLK = SystemCoreClock / PSC;
			IC1PSC = __LL_TIM_GET_ICPSC_RATIO(LL_TIM_IC_GetPrescaler(TIM2,LL_TIM_CHANNEL_CH2));
			
			period = (uwDiff*(PSC)*1.0) / (TIM2CLK*IC1PSC*1.0); //calculate uptime period
			distance = (period *170) * 100;
			sprintf(disp_str, "%2d",(int)distance);
			seg7_display(disp_str);
			uhICIndex = 0;
		}
	}

}

void seg7_display(char str[5]){
	/*0 = 2 10 11 12 13 14 = 0111 1100 0000 0100 = 7C04
		1 = 10 11 = 0000 1100 0000 0000 = 0C00
		2 = 2 10 15 13 12 = 1011 0100 0000 0100 = B404
		3 = 2 10 15 11 12 = 1001 1100 0000 0100 = 9C04
		4 = 14 15 10 11 = 1100 1100 0000 0000 = CC00
		5 = 2 14 15 11 12 = 1101 1000 0000 0100 = 6C04
		6 = 2 14 15 13 11 12 = 1111 1000 0000 0100 = F804
		7 = 2 10 11 = 0000 1100 0000 0100 = 0C04
		8 = 2 10 11 12 13 14 15 = 1111 1100 0000 0100 = FC04
		9 = 2 10 11 12 14 15 = 1101 1100 0000 0100 = DC04
	*/
	int i, j=1;
	for(i=0;i<4;i++){
		GPIOC->ODR &= ~0x000F;
		GPIOB->ODR &= ~0xFC04;
		if(str[i] == '0') GPIOB->ODR |= 0x7C04;
		else if(str[i] == '1') GPIOB->ODR |= 0x0C00;
		else if(str[i] == '2') GPIOB->ODR |= 0xB404;
		else if(str[i] == '3') GPIOB->ODR |= 0x9C04;
		else if(str[i] == '4') GPIOB->ODR |= 0xCC00;
		else if(str[i] == '5') GPIOB->ODR |= 0xD804;
		else if(str[i] == '6') GPIOB->ODR |= 0xF804;
		else if(str[i] == '7') GPIOB->ODR |= 0x0C04;
		else if(str[i] == '8') GPIOB->ODR |= 0xFC04;
		else if(str[i] == '9') GPIOB->ODR |= 0xDC04;
		else GPIOB->ODR &= ~0xFC04;
		GPIOC->ODR |= j;
		j*=2;
		LL_mDelay(1);
	}
}

void seg7_gpio_config(){
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13| LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

//trigger output mode
void TRIG_Config(void)
{
	LL_GPIO_InitTypeDef timic_gpio;
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	
	//GPIO PA0 config
	timic_gpio.Mode = LL_GPIO_MODE_OUTPUT;
	timic_gpio.Pull = LL_GPIO_PULL_NO;
	timic_gpio.Pin = LL_GPIO_PIN_2;
	timic_gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	timic_gpio.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	
	LL_GPIO_Init(GPIOA, &timic_gpio);
}

//echo alternate mode

void ECHO_Config(void)
{
	LL_GPIO_InitTypeDef timic_gpio;
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	
	//GPIO PA0 config
	timic_gpio.Mode = LL_GPIO_MODE_ALTERNATE;
	timic_gpio.Pull = LL_GPIO_PULL_NO;
	timic_gpio.Pin = LL_GPIO_PIN_1;
	timic_gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	timic_gpio.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	timic_gpio.Alternate = LL_GPIO_AF_1;
	
	LL_GPIO_Init(GPIOA, &timic_gpio);
	
}

void TIMx_IC_Config(void)
{
	LL_TIM_IC_InitTypeDef timic;
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	
	//TIM_IC Config CH2
	timic.ICActiveInput = LL_TIM_ACTIVEINPUT_DIRECTTI;
	timic.ICFilter = LL_TIM_IC_FILTER_FDIV1_N2;
	timic.ICPolarity = LL_TIM_IC_POLARITY_BOTHEDGE;
	timic.ICPrescaler = LL_TIM_ICPSC_DIV1;
	
	LL_TIM_IC_Init(TIM2,LL_TIM_CHANNEL_CH2 ,&timic);
	
	NVIC_SetPriority(TIM2_IRQn,0);
	
	NVIC_EnableIRQ(TIM2_IRQn);
	LL_TIM_EnableIT_CC2(TIM2);
	LL_TIM_CC_EnableChannel(TIM2,LL_TIM_CHANNEL_CH2);
	
	LL_TIM_EnableCounter(TIM2);
	
}

void TIM2_IRQHandler(void)
{
	if(LL_TIM_IsActiveFlag_CC2(TIM2) == SET)
	{
		LL_TIM_ClearFlag_CC2(TIM2);
		//Detect 1st rising edge
		if(uhICIndex == 0)
		{
			uwIC1 = LL_TIM_IC_GetCaptureCH2(TIM2);
			uhICIndex = 1;
		}
		//Detect 2nd rising edge
		else if(uhICIndex == 1)
		{
			uwIC2 = LL_TIM_IC_GetCaptureCH2(TIM2);
			
			if(uwIC2 > uwIC1)
					uwDiff = uwIC2 - uwIC1;
			else if(uwIC2 < uwIC1)
					uwDiff = ((LL_TIM_GetAutoReload(TIM2) - uwIC1) +uwIC2) + 1;
			uhICIndex = 2;
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
