#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"


#define GPIOAEN (1U<<0)
#define UART2	(1U<<17)
#define CR1_TE	(1U<<3)
#define CR1_UE	(1U<<13)
#define SR_TXE	(1U<<7)


#define SR_TXE	(1U<<7)

void uart2_write(int ch);
void uart2_tx_init(void);


int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}


void uart2_tx_init(void)
{
	/** Configure UART GPIO Pin **/

		/** Enable clock access to GPIOA **/
		RCC->AHB1ENR |= GPIOAEN;

		/** Set PA2(UART TX) Mode to alternate function mode **/
		GPIOA->MODER |= (1U<<5) ; // Set bit to 1
		GPIOA->MODER &=~ (1U<<4) ; // Set bit to 0

		/** Set PA2 Alternate Function Type to UART_TX (AF07) **/
		GPIOA->AFR[0] |= (1U<<8);
		GPIOA->AFR[0] |= (1U<<9);
		GPIOA->AFR[0] |= (1U<<10);
		GPIOA->AFR[0] &=~ (1U<<11);


		/** Configure UART module**/
		/** Enable Clock Access to UART**/
		RCC->APB1ENR |= UART2;
		/** Configure Baud Rate**/
		//uart_set_baudrate(USART2, APB1_CLK,UART_BAUDRATE);
		USART2->BRR=0x0681; // Hard Coded Value for 9600 @ 16Mhz
		/** Configure Transfer Direction**/
		 USART2->CR1 = CR1_TE; // See UART Control Register - TE Transmitter Enable
		// No Need to Set Stop Bit, Parity Bit, Start Bit. The CR1 is already set to all  0 except TE
		 /** Enable UART Module**/
		 USART2->CR1 |= CR1_UE; // OR Operator used to add to TE
}

void uart2_write(int ch)
{
	/* Make sure the transmit data register is empty*/
	while(!(USART2->SR & SR_TXE))		// if TXE isn't set to 1, then return True else false.
	{

	}

	/* Write to transmit data register*/
	USART2->DR = (ch & 0xFF);
}



