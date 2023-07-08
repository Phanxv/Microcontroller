#include <stm32l1xx.h>

int sw;
int sw_cnt = 0;

int main()
{
	RCC->AHBENR |= (1<<0);
	GPIOA->MODER &= ~(3<<0);
	while(1)
	{
		sw = (GPIOA->IDR & 0x0001);
		if(sw == 0x0001)
		{
			sw_cnt = (sw_cnt + 1) % 255;
		}
	}
}