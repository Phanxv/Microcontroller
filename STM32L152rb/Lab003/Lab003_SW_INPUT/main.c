#include <stm32l1xx.h>

uint8_t sw;

int main(){
	RCC->AHBENR |= (1<<0);
	GPIOA->MODER &= ~(3<<0);
	while(1){
		sw = (GPIOA->IDR & 0x0001);
	}
}
