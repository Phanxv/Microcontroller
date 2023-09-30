#include "stm32l1xx.h"

#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_gpio.h"

#include "stm32l1xx_ll_adc.h"
#include "stm32l1xx_ll_lcd.h"
#include "stm32l152_glass_lcd.h"
#include "stm32l1xx_ll_usart.h"
#include <stdio.h>

void PA12_gpio_config();
void PC12_gpio_config();
void PD2_gpio_config();
void PA4_gpio_config();
void PA4_ADC_config();
void PB6_7_USART_Config(void);
void USART_Config(void);
void USART_SendString(uint8_t* str, uint8_t size);
void SystemClock_Config(void);

int adc_data = 0;
char disp_str[7];

int main(){
	SystemClock_Config();
	
	PA12_gpio_config();
	PC12_gpio_config();
	PD2_gpio_config();
	PA4_ADC_config();
	
	LCD_GLASS_Init();
	USART_Config();
	
	while(1)
	{
		LL_ADC_REG_StartConversionSWStart(ADC1);
		while(LL_ADC_IsActiveFlag_EOCS(ADC1) == 0);
		adc_data = LL_ADC_REG_ReadConversionData10(ADC1);
		sprintf(disp_str, "%d    ", adc_data);
		LCD_GLASS_DisplayString((uint8_t*)disp_str);
		USART_SendString((uint8_t*)disp_str, sizeof(disp_str));
		if(adc_data > 768){
			LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_12);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_12);
			LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_2);
		}
		if(adc_data <= 768 && adc_data > 512){
			LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_12);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_12);
			LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_2);
		}
		if(adc_data <= 512 && adc_data > 256){
			LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_12);
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_12);
			LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_2);
		}
		if(adc_data <= 256){
			LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_12);
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_12);
			LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_2);
		}
	}
}

void PA12_gpio_config(){
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void PC12_gpio_config(){
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void PD2_gpio_config(){
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
	
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
	LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void PA4_gpio_config(){
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void PA4_ADC_config(){
	LL_ADC_InitTypeDef ADC_config;
	PA4_gpio_config();
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
	
	ADC_config.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
	ADC_config.Resolution = LL_ADC_RESOLUTION_10B;
	ADC_config.LowPowerMode = LL_ADC_LP_AUTOWAIT_NONE;
	ADC_config.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE;
	LL_ADC_Init(ADC1, &ADC_config);
	
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_4);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_4, LL_ADC_SAMPLINGTIME_48CYCLES);
	
	LL_ADC_Enable(ADC1);
}

void PB6_7_USART_Config(void){
	LL_GPIO_InitTypeDef GPIO_conf;
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	
	GPIO_conf.Pin = LL_GPIO_PIN_7;
	GPIO_conf.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_conf.Pull = LL_GPIO_PULL_UP;
	GPIO_conf.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_conf.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_conf.Alternate = LL_GPIO_AF_7;
	LL_GPIO_Init(GPIOA, &GPIO_conf);
	
	GPIO_conf.Pin = LL_GPIO_PIN_6;
	LL_GPIO_Init(GPIOA, &GPIO_conf);
}

void USART_Config(void){
	LL_USART_InitTypeDef USART_conf;
	PB6_7_USART_Config();
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
	
	USART_conf.BaudRate = 115200;
	USART_conf.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_conf.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_conf.Parity = LL_USART_PARITY_NONE;
	USART_conf.StopBits = LL_USART_STOPBITS_1;
	USART_conf.OverSampling = LL_USART_OVERSAMPLING_16;
	USART_conf.TransferDirection = LL_USART_DIRECTION_TX_RX;
	
	LL_USART_Init(USART1, &USART_conf);
	LL_USART_Enable(USART1);
}

void USART_SendString(uint8_t* str, uint8_t size){
	uint8_t i = 0;
	while(i<size){
		while(LL_USART_IsActiveFlag_TXE(USART1) == RESET);
		LL_USART_TransmitData8(USART1, str[i]);
		++i;
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