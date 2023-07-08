#include <stm32l1xx.h>

uint8_t sw;

int main()
{
	RCC->AHBENR |= (1<<0) | (1<<1);
	GPIOB->MODER |= (1<<14) | (1<<12);
	GPIOA->MODER &= ~(3<<0);
	while(1)
	{
		sw = (GPIOA->IDR & 0x0001);
		if(sw == 0x0001)
		{
			GPIOB->ODR |= (1<<6);
			GPIOB->ODR |= (1<<7);
		}
		else 
		{
			GPIOB->ODR &= ~(1<<6);
			GPIOB->ODR &= ~(1<<7);
		}
	}
}
