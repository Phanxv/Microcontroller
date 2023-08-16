#include <stm32l1xx.h>
#define LINE1 8 
#define LINE2 9 
uint8_t sw = 0;
int main()
{
    RCC->AHBENR |= (1<<0) | (1<<1) | (1<<2);//Enable PA, PB, PC
    GPIOA->MODER &= ~(1<<16) | (1<<18);//set PA8, PA9 as input
    GPIOB->MODER &= ~(0xFFFFFFFF);
    GPIOB->MODER |= (1<<6) | (1<<8) | (1<<10); //set PB3,PB4, PB5 as output
    GPIOC->MODER &= ~(0xFFFFFFFF);
    GPIOC->MODER |= (1<<16) | (1<<18);     //set PC8, PC9 as output
    GPIOB->ODR &= ~(7<<3);         //PB3,PB4, PB5 write output to 1 (OFF) 

    RCC->APB2ENR |= (1<<0);

    //conf PA8 is interrupt
    SYSCFG->EXTICR[2] &= ~(15<<0);
    EXTI->IMR |= (1<<8);
    EXTI->FTSR |= (1<<8);

    //conf PA9 is interrupt
    SYSCFG->EXTICR[2] &= ~(15<<4);
    EXTI->IMR |= (1<<9);
    EXTI->FTSR |= (1<<9);

    //conf NVIC
    NVIC_EnableIRQ((IRQn_Type) 23);
    NVIC_SetPriority((IRQn_Type) 23, 0);


    while(1) {                     //loop start

        GPIOC->ODR &= ~(1<<LINE1);
        GPIOC->ODR |= (1<<LINE2);

        GPIOC->ODR &= ~(1<<LINE2);
        GPIOC->ODR |= (1<<LINE1);

    }
}

void EXTI9_5_IRQHandler(void) {
    //EXTI->PR |= (1<<8);
    if(EXTI->PR == 0x00000100){
        if((GPIOC->ODR & (1<<8))==0){
        //sw1
            sw = 1;
            GPIOB->ODR |= (1<<3) | (1<<4) | (1<<5) ;
            GPIOB->ODR &= ~(1<<3);
        }
        if((GPIOC->ODR & (1<<9))==0) {
            //sw3
            sw = 3;
            GPIOB->ODR |= (1<<3) | (1<<4) | (1<<5) ;
            GPIOB->ODR &= ~(1<<5);
        }
    }
    if(EXTI->PR == 0x00000200){
        if((GPIOC->ODR & (1<<8))==0){
        //sw2
            sw = 2;
            GPIOB->ODR |= (1<<3) | (1<<4) | (1<<5) ;
            GPIOB->ODR &= ~((1<<3) | (1<<4));
        }
        if((GPIOC->ODR & (1<<9))==0) {
        //sw4
            sw = 4;
            GPIOB->ODR |= (1<<3) | (1<<4) | (1<<5) ;
            GPIOB->ODR &= ~((1<<4) | (1<<5));
        }
    }
    EXTI->PR |= (1<<9);
		EXTI->PR |= (1<<8);
}
