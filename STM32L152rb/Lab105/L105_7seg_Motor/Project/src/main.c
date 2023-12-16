#include "stm32l1xx.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_exti.h"
#include "stm32l1xx_ll_tim.h"
#include "stdio.h"

#define TIMx_ARR			100
#define TIMx_PSC			2

//declare function timer oc gpio config
void GPIO_Config(void);
void TIMBase_Config(void);
void SystemClock_Config(void);
void L293D_Config(void);
void TIM_OC_Config(float);
void TIM_BASE_Config(void);
void TIM_OC_GPIO_Config(void);
void PA0_EXTI_Config(void);
void USRBTN_GPIO_Config(void);
char disp_str[7];
void seg7_display(char []);
void seg7_gpio_config();

int duty_cycle = 100;  //input duty_cycle duty

int main()
{
	SystemClock_Config();
	TIM_BASE_Config();
	TIM_OC_Config(duty_cycle);
	TIM_OC_GPIO_Config();
	L293D_Config();
	USRBTN_GPIO_Config();
	PA0_EXTI_Config();
	seg7_gpio_config();
	//first lcd display
	sprintf(disp_str, "%4d",duty_cycle); //fix for lcd3
	seg7_display(disp_str);
	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_7); 
	LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_4);
	//LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_5); 
	while(1){
		sprintf(disp_str, "%4d",duty_cycle);
		seg7_display(disp_str);
	};

}
void seg7_display(char str[4]){
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
void EXTI0_IRQHandler(void){ //interrupt function PA0
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) == SET){
		duty_cycle-=10;
		if(duty_cycle<1){
			duty_cycle = 100;
		}
		LL_TIM_OC_SetCompareCH2(TIM3,duty_cycle); //set duty_cycle duty
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
	}
}


void L293D_Config(void)
{
	LL_GPIO_InitTypeDef l293d_initstruct;
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	
	l293d_initstruct.Mode = LL_GPIO_MODE_OUTPUT;
	l293d_initstruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	l293d_initstruct.Pull = LL_GPIO_PULL_NO;
	l293d_initstruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	l293d_initstruct.Pin =  LL_GPIO_PIN_4 | LL_GPIO_PIN_7;
	LL_GPIO_Init(GPIOB,&l293d_initstruct);
	
};

void TIM_OC_GPIO_Config(void)
{
	LL_GPIO_InitTypeDef gpio_initstructure;
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	
	gpio_initstructure.Mode = LL_GPIO_MODE_ALTERNATE;
	gpio_initstructure.Alternate = LL_GPIO_AF_2;
	gpio_initstructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio_initstructure.Pin = LL_GPIO_PIN_5;
	gpio_initstructure.Pull = LL_GPIO_PULL_NO;
	gpio_initstructure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	LL_GPIO_Init(GPIOB, &gpio_initstructure);
	
}

void USRBTN_GPIO_Config(void){ //config GPIO for PA0 (USER)
	LL_GPIO_InitTypeDef gpio_initstructure;
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	
	gpio_initstructure.Mode = LL_GPIO_MODE_INPUT;
	gpio_initstructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio_initstructure.Pin = LL_GPIO_PIN_0;
	gpio_initstructure.Pull = LL_GPIO_PULL_NO;
	gpio_initstructure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	LL_GPIO_Init(GPIOA, &gpio_initstructure);

}

void PA0_EXTI_Config(void){
	LL_EXTI_InitTypeDef PA0_EXTI_InitStruct;
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA,LL_SYSCFG_EXTI_LINE0);
	PA0_EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_0;
	PA0_EXTI_InitStruct.LineCommand = ENABLE;
	PA0_EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	PA0_EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(&PA0_EXTI_InitStruct);
	
	NVIC_EnableIRQ((IRQn_Type)6);
	NVIC_SetPriority((IRQn_Type)6, 0);
	
}

void TIM_OC_Config(float duty_cycle)
{
	LL_TIM_OC_InitTypeDef tim_oc_initstructure;
	
	TIM_OC_GPIO_Config();
	TIM_BASE_Config();
	
	tim_oc_initstructure.OCState = LL_TIM_OCSTATE_DISABLE;
	tim_oc_initstructure.OCMode = LL_TIM_OCMODE_PWM1;
	tim_oc_initstructure.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	//set arr for dutyduty_cycle
	tim_oc_initstructure.CompareValue = LL_TIM_GetAutoReload(TIM3) ;
	LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH2, &tim_oc_initstructure);

	/*Start Output Compare in PWM Mode*/
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH2);
	LL_TIM_EnableCounter(TIM3);
}

void TIM_BASE_Config(void)
{
	LL_TIM_InitTypeDef timbase_initstructure;
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	//Time-base configure
	timbase_initstructure.ClockDivision = LL_TIM_CLOCKDIVISION_DIV2;
	timbase_initstructure.CounterMode = LL_TIM_COUNTERMODE_UP;
	timbase_initstructure.Autoreload = TIMx_ARR - 1;
	timbase_initstructure.Prescaler = TIMx_PSC - 1;
	LL_TIM_Init(TIM3, &timbase_initstructure);
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

