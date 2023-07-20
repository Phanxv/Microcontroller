#include "stm32l1xx.h"

void delay(uint32_t t){
    while(t--);
}

int sw_cnt = 0;

int main(){
    RCC->AHBENR |= (1<<0);
    GPIOA->MODER &= ~(3<<0);
    while(1){
        if((GPIOA->IDR & (1<<0)) == 0x01){
            sw_cnt = (sw_cnt % 256) + 1;
            delay(0x0FFFFFFF);
        }
    }
}
