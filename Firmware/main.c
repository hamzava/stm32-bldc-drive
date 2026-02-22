#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "timer.h"
#include "uart.h"


void MODER(void);
void AFR(void);

int main(void){

	RCC->AHB1ENR |= (0b1111 << 0);	 // Enable GPIOA, GPIOB, GPIOC, GPIOD

    RCC->APB1ENR |= (0b11 << 0);		// Enable TIM2, TIM3
	RCC->APB1ENR |= (1U<<17);

    RCC->APB2ENR |= ((1U << 0) | (1U << 4) );		// Enable TIM1, USART1


    uart2_tx_init();

//    for(uint32_t i =0; i<10; i++){
//        printf("UART Initiated...\n\r");
//    }

    MODER();

    AFR();

    GPIOC->ODR |= (1U<<13);

	TIM2_init();

	TIM1_PWM();



	while(1){

		commutation();

		delay_us(40000000); // for teting purpose i want to see the voltage at all pins

	}

}

void MODER(void){

	GPIOA->MODER |= (1U<<15);		// PA7 set to AFR for TIM1_CH1N MODER7[15:14] OK
	GPIOA->MODER |= (1U<<17); 		// PA8 set to AFR for TIM1_CH1 MODER8[17:16] OK
	GPIOA->MODER |= (1U<<19); 		// PA9 set to AFR for TIM1_CH2 MODER9[19:18] OK
	GPIOA->MODER |= (1U<<21);		// PA10 set to AFR for TIM1_CH3 MODER10 [21:20] OK

	GPIOB->MODER |= (1U<<1); 		// PB0 set to AFR for TIM1_CH2N MODER0[0:1] OK
	GPIOB->MODER |= (1U<<3);		// PB1 set to AFR for TIM1_CH3N MODER1[3:2] OK


	/* 3. Set PC13 as Output */
	GPIOC->MODER |= (1U<<26); // Bit 26 - Set to 1
	GPIOC->MODER &=~ (1U<<27); // Bit 27 - Set to 0

//	GPIOC->MODER |= (1U<<13); 		// PC6 set to AFR for TIM3_CH1 MODER7[13:12] OK
//	GPIOC->MODER |= (1U<<15);		// PC7 set to AFR for TIM3_CH2 MODER7[15:14] OK
//	GPIOC->MODER |= (1U<<17); 		// PC8 set to AFR for TIM3_CH3 MODER8[17:16] OK
//
//	//PC13, PC14, PC15 as Input
//	GPIOC->MODER &=~ (0b11<<26); 	//PC13 MODER13	[27:26] OK
//	GPIOC->MODER &=~ (0b11<<28); 	//PC14 MODER14 [29:28] OK
//	GPIOC->MODER &=~ (0b11<<30); 	//PC15 MODER15 [31:30] OK

	GPIOA->PUPDR = 0;

	GPIOA->PUPDR |= (1U<<15);
	GPIOA->PUPDR |= (1U<<17);
	GPIOA->PUPDR |= (1U<<19);
	GPIOA->PUPDR |= (1U<<21);

	GPIOB->PUPDR |= (1U<<1);
	GPIOB->PUPDR |= (1U<<3);


}
void AFR(void){

	GPIOA->AFR[0] |= (1U<<28); 		// PA7 TIM1_CH1N AF01 AFRL7[31:28] OK
	GPIOA->AFR[1] |= (1U<<0); 		// PA8 TIM1_CH1 AF01 AFRH8[3:0] OK
	GPIOA->AFR[1] |= (1U<<4); 		// PA9 TIM1_CH2 AF01 AFRH9[7:4] OK
	GPIOA->AFR[1] |= (1U<<8); 		// PA10 TIM1_CH3 AF01 AFRH10[11:8] OK

	GPIOB->AFR[0] |= (1U<<0); 		// PB0 TIM1_CH2N AF01 AFRL0[3:0] OK
	GPIOB->AFR[0] |= (1U<<4); 		// PB1 TIM1_CH3N AF01 AFRL1[7:4] OK
//
//	GPIOC->AFR[0] |= (1U<<25); 		// PC6 TIM3_CH1 AF02 AFRL6[27:24] OK
//	GPIOC->AFR[0] |= (1U<<29); 		// PC7 TIM3_CH2 AF02 AFRL[31:28] OK
//	GPIOC->AFR[1] |= (1U<<1); 		// PC8 TIM3_CH3 AF02 AFRH8[3:0] OK

}

void led_toggle(void)
{
	GPIOC->ODR ^= (1U<<13); // Bit 5 - Toggle PA5
}

