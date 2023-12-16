#include "stm32l1xx.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_usart.h"
#include "stm32l1xx_ll_lcd.h"
#include "stm32l152_glass_lcd.h"
#include "stm32l1xx_ll_tim.h"
#include "stm32l1xx_ll_exti.h"
#include "stdio.h"

/*for 10ms update event*/
#define TIMx_ARR			100
#define TIMx_PSC			2 

/*Macro function for ARR calculation*/
#define ARR_CALCULATE(N) (SystemCoreClock) / ((TIMx_PSC) * (N))

void SystemClock_Config(void);
void TIM_BASE_Config();
void TIM_OC_GPIO_Config(void);
void TIM_OC_Config(float);
void TIM_CNT_config(void);
void L1293d_init();
void USRBTN_GPIO_Config(void);
void PA0_EXTI_Config(void);

int prev_stat, usr_btn_stat;
int duty_cycle = 100;

int main(void)
{
  SystemClock_Config();
	L1293d_init();
	TIM_CNT_config(); 
	USRBTN_GPIO_Config();
	PA0_EXTI_Config();
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
	TIM_OC_Config(duty_cycle);
	while(1){}
}

void EXTI0_IRQHandler(void){
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) == SET){
		duty_cycle-=20;
		if(duty_cycle<1){
			duty_cycle = 100;
		}
		LL_TIM_OC_SetCompareCH1(TIM3,duty_cycle); //set duty_cycle duty
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
	}
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
	NVIC_EnableIRQ((IRQn_Type) 6);
	NVIC_SetPriority((IRQn_Type) 6, 0);
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

void L1293d_init(){
	LL_GPIO_InitTypeDef l1293d_initstruct;
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	
	l1293d_initstruct.Mode = LL_GPIO_MODE_OUTPUT;
	l1293d_initstruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	l1293d_initstruct.Pull = LL_GPIO_PULL_NO;
	l1293d_initstruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	l1293d_initstruct.Pin = LL_GPIO_PIN_5 | LL_GPIO_PIN_7;
	LL_GPIO_Init(GPIOB, &l1293d_initstruct);
}

void TIM_CNT_config(){ //config TIMER2 for counting time
	LL_TIM_InitTypeDef timbase_initstructure;
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	//set counter 1 sec
	timbase_initstructure.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1; /*Select clock division*/
	timbase_initstructure.CounterMode = LL_TIM_COUNTERMODE_UP; /*select counter mode to count up*/
	timbase_initstructure.Autoreload = 1000 - 1; /*set ARR*/
	timbase_initstructure.Prescaler = 32000 - 1; /*set PSC*/
	
	LL_TIM_Init(TIM2, &timbase_initstructure);

	LL_TIM_EnableCounter(TIM2); /*Enable counter TIMER2*/
}
void TIM_BASE_Config()
{
	LL_TIM_InitTypeDef timbase_initstructure;
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	//Time-base configure
	timbase_initstructure.ClockDivision = LL_TIM_CLOCKDIVISION_DIV2;
	timbase_initstructure.CounterMode = LL_TIM_COUNTERMODE_UP;
	timbase_initstructure.Autoreload = TIMx_ARR - 1;
	timbase_initstructure.Prescaler =  TIMx_PSC- 1;
	LL_TIM_Init(TIM3, &timbase_initstructure);
	
	LL_TIM_EnableCounter(TIM3); 
}


void TIM_OC_GPIO_Config(void)
{
	LL_GPIO_InitTypeDef gpio_initstructure;
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	
	gpio_initstructure.Mode = LL_GPIO_MODE_ALTERNATE;
	gpio_initstructure.Alternate = LL_GPIO_AF_2;
	gpio_initstructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio_initstructure.Pin = LL_GPIO_PIN_4;
	gpio_initstructure.Pull = LL_GPIO_PULL_NO;
	gpio_initstructure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	LL_GPIO_Init(GPIOB, &gpio_initstructure);
}

void TIM_OC_Config(float duty_cycle)
{
	LL_TIM_OC_InitTypeDef tim_oc_initstructure;
	
	TIM_OC_GPIO_Config();
	TIM_BASE_Config();
	
	tim_oc_initstructure.OCState = LL_TIM_OCSTATE_DISABLE;
	tim_oc_initstructure.OCMode = LL_TIM_OCMODE_PWM1;
	tim_oc_initstructure.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	tim_oc_initstructure.CompareValue = LL_TIM_GetAutoReload(TIM3);
	LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH1, &tim_oc_initstructure);

	/*Start Output Compare in PWM Mode*/
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);
	LL_TIM_EnableCounter(TIM3);
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

