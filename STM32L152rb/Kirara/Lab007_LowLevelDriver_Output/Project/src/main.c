#include "stm32l1xx.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_tim.h"
#include "stm32l1xx_ll_usart.h"
#include "string.h"

void SystemClock_Config(void);
void TIMBase_Config(void);//TIM2, TIM3
void GPIO_USART_Configure(void);//PA9, PA10
void USART_Config(void);//USART1
void USART_SendString(uint8_t*, uint8_t );
void GPIO_Config(void);//PA0-3 and PB4-7 for motor, PB8 for switch, PA4-5 for buzzer

int freq_to_tim(int f);
void forward(); 
void turnLeft();
void turnRight();
void backward();
void resetMotor();

uint16_t cntTim2 = 0;
uint16_t cntTim3 = 0;
uint8_t recv_buffer[255];
uint8_t idx_buffer = 0;
uint8_t recv_path[255];
uint8_t idx_path = 0;
uint8_t run_path = 0;
uint8_t state = 0;
uint16_t freq = 400;

int main()
{
	SystemClock_Config();
	TIMBase_Config();
	USART_Config();
	GPIO_Config();

	while(1)
	{
		if(LL_USART_IsActiveFlag_RXNE(USART1) != RESET)
		{
			recv_buffer[idx_buffer++] = LL_USART_ReceiveData8(USART1);
			idx_buffer = idx_buffer % 255;
			if(strstr((const char*)recv_buffer, "\r\n") != NULL)
			{
				if(strcmp((const char*)recv_buffer,"SET PATH\r\n") == 0)
				{
					state = 1;
					/*while(state == 1)*/
					if(state == 1)
					{
						//memset(recv_path, NULL, sizeof(recv_path));
						idx_path = 0;
						memset(recv_buffer, NULL, sizeof(recv_buffer));
						idx_buffer = 0;
						while(LL_USART_IsActiveFlag_RXNE(USART1) == RESET);
						recv_buffer[idx_buffer++] = LL_USART_ReceiveData8(USART1);
						idx_buffer = idx_buffer % 255;
						if(strstr((const char*)recv_buffer, "\r\n") != NULL)
						{
							if(strcmp((const char*)recv_buffer,"F\r\n") == 0)
							{
								recv_path[idx_path++] = 'F';
							}
							else if(strcmp((const char*)recv_buffer,"B\r\n") == 0)
							{
								recv_path[idx_path++] = 'B';
							}
							else if(strcmp((const char*)recv_buffer,"L\r\n") == 0)
							{
								recv_path[idx_path++] = 'L';
							}
							else if(strcmp((const char*)recv_buffer,"R\r\n") == 0)
							{
								recv_path[idx_path++] = 'R';
							}
							else if(strcmp((const char*)recv_buffer,"END\r\n") == 0)
							{
								state = 0;
							}
							else
							{
								--idx_buffer;
							}
							memset(recv_buffer, NULL, sizeof(recv_buffer));
							idx_buffer = 0;
						}
					}
				}
				else if(strcmp((const char*)recv_buffer,"SHOW PATH\r\n") == 0)
				{
					USART_SendString(recv_path, sizeof(recv_path));
				}
				else if(strcmp((const char*)recv_buffer,"START\r\n") == 0)
				{
					run_path = 1;
				}
				memset(recv_buffer, NULL, sizeof(recv_buffer));
				idx_buffer = 0;
			}
		}
		
		cntTim2 = LL_TIM_GetCounter(TIM2);
		if(cntTim2 >= 1000 & run_path == 1){
			LL_TIM_SetCounter(TIM2, 0);
			if(recv_path[state] != NULL){
				if(recv_path[state] == 'F'){
					forward();
				}
				else if(recv_path[state] == 'B'){
					backward();
				}
				else if(recv_path[state] == 'L'){
					turnLeft();
				}
				else if(recv_path[state] == 'R'){
					turnRight();
				}
				++state;
			}
			else{
				resetMotor();
				state = 0;
			}
		}
		
		cntTim3 = LL_TIM_GetCounter(TIM3);
		if(cntTim3 >= freq_to_tim(freq)/2){
			LL_TIM_SetCounter(TIM3, 0);
			if(recv_path[state-1] == 'L' | recv_path[state-1] == 'R'){
				LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_4);
				LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_5);
			}
		}
	}
}

int freq_to_tim(int f){
	return (1000000/f);
}

void forward(){
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_2);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_3);
	
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
}

void backward(){
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_3);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);
	
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_5);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
}

void turnLeft(){
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_2);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_3);
	
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_5);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
}

void turnRight(){
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_3);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);
	
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
}

void resetMotor(){
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_3);
	
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
}

void TIMBase_Config(void){
	LL_TIM_InitTypeDef timbase_initstructure;
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	
	timbase_initstructure.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	timbase_initstructure.CounterMode = LL_TIM_COUNTERMODE_UP;
	timbase_initstructure.Autoreload = 32000 - 1;
	
	timbase_initstructure.Prescaler = 32000 - 1;
	LL_TIM_Init(TIM2, &timbase_initstructure);
	LL_TIM_EnableCounter(TIM2);
	
	timbase_initstructure.Prescaler = 32 - 1;
	LL_TIM_Init(TIM3, &timbase_initstructure);
	LL_TIM_EnableCounter(TIM3);
}

void GPIO_USART_Configure(void){
	LL_GPIO_InitTypeDef gpio_conf;
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	
	gpio_conf.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio_conf.Pull = LL_GPIO_PULL_UP;
	gpio_conf.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	gpio_conf.Mode = LL_GPIO_MODE_ALTERNATE;
	gpio_conf.Alternate = LL_GPIO_AF_7;
	
	gpio_conf.Pin = LL_GPIO_PIN_9;
	LL_GPIO_Init(GPIOA, &gpio_conf);
	
	gpio_conf.Pin = LL_GPIO_PIN_10;
	LL_GPIO_Init(GPIOA, &gpio_conf);
}

void USART_Config(void){
	LL_USART_InitTypeDef usart_conf;
	GPIO_USART_Configure();
	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
	usart_conf.BaudRate = 115200;
	usart_conf.DataWidth = LL_USART_DATAWIDTH_8B;
	usart_conf.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	usart_conf.Parity = LL_USART_PARITY_NONE;
	usart_conf.StopBits = LL_USART_STOPBITS_1;
	usart_conf.OverSampling = LL_USART_OVERSAMPLING_16;
	usart_conf.TransferDirection = LL_USART_DIRECTION_TX_RX;
	LL_USART_Init(USART1, &usart_conf);
	LL_USART_Enable(USART1);
}

void USART_SendString(uint8_t* str, uint8_t size){
	uint8_t i = 0;
	while(i < size)
	{
		while(LL_USART_IsActiveFlag_TXE(USART1) == RESET);
		LL_USART_TransmitData8(USART1, str[i]);
		++i;
	}
}

void GPIO_Config(void){
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void SystemClock_Config(void){
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