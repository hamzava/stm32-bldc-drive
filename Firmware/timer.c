#include "stm32f4xx.h"
#include "timer.h"
#include "main.h"
#include <stdio.h>

#define TIM3_SR_CC1IF	(1U<<1)
#define TIM3_SR_CC2IF	(1U<<2)
#define TIM3_SR_CC3IF	(1U<<3)


void TIM1_PWM(void){

    // ---------------------- Reset TIM1 ----------------------

    RCC->APB2RSTR |= RCC_APB2RSTR_TIM1RST;
    RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM1RST;

	// ---------------------- TIMER BASE CONFIGURATION ----------------------
	TIM1->PSC = 0;          // Prescaler = 0 -> Timer clock = 16 MHz (no division)
	TIM1->ARR = 800 - 1;    // Auto Reload = 799 -> 16 MHz / 800 = 20 kHz PWM frequency
	TIM1->CNT = 0;          // Clear counter to start from 0

	// ---------------------- CHANNEL 1 CONFIGURATION ----------------------
	TIM1->CCMR1 &= ~(0b111 << 4);   // Clear bits [6:4] to ensure OC1M is empty
	TIM1->CCMR1 |=  (0b100 << 4);   // Set OC1M = 100 (Force Inactive))
	TIM1->CCMR1 |=  (1U << 3);      // Set OC1PE = 1 (Preload enable)

	TIM1->CCER &= ~	(1U << 1);      // Clear CC1P = 0 -> Active High
	TIM1->CCER |= 	(1U << 3);   	// CC1NP = 1 -> Complementary output active low

	// ---------------------- CHANNEL 2 CONFIGURATION ----------------------

	TIM1->CCMR1 &= ~(0b111 << 12);  // Clear bits [14:12]
	TIM1->CCMR1 |=  (0b100 << 12);  // Set OC2M = 100 (Force Inactive)
	TIM1->CCMR1 |=  (1U << 11);     // Set OC2PE = 1 (Preload enable)

	TIM1->CCER &= ~(1U << 5);  		// Clear CC2P = 0 -> Active High
	TIM1->CCER |= (1U << 7);   		// CC2NP = 1 -> Complementary output active low

	// ---------------------- CHANNEL 3 CONFIGURATION ------------------------
	TIM1->CCMR2 &= ~(0b111 << 4);   // Clear bits [6:4]
	TIM1->CCMR2 |=  (0b100 << 4);   // Set OC3M = 100 (Force Inactive)
	TIM1->CCMR2 |=  (1U << 3);      // Set OC3PE = 1 (Preload enable)

	TIM1->CCER &= ~(1U << 9);       // Clear CC3P = 0 -> Active High
	TIM1->CCER |= (1U << 11);  		// CC3NP = 1 -> Complementary output active low

	// ---------------------- BDTR CONFIGURATION ----------------------
	TIM1->BDTR = 0;
	TIM1->BDTR |= (1U << 15);       // Set MOE = 1 (Output signals enabled)
	TIM1->BDTR |= (1U << 11);       // Set OSSR = 1 (Off-State Selection for Run mode)
	TIM1->BDTR |= (0x20 << 0);  	// 2µs dead time for 16MHz

	// ---------------------- CR1 OPTIONAL SETTINGS ----------------------
	TIM1->CR1 &=~ (0b11<<5); 		// CMS = 00
	TIM1->CR1 &=~ (1U<<4);

	// ---------------------- CR2 OPTIONAL SETTINGS ----------------------
	TIM1->CR2 |= (1U << 2);			// CCUS = 1 -> capture/compare control bits are preloaded
	TIM1->CR2 |= (1U << 0);			// CCPC = 1 -> CCxE, CCxNE and OCxM bits are preloaded
	// ---------------------- SET DUTY CYCLES ----------------------
	TIM1->CCR1 = 200;               // 20% Duty Cycle for Channel 1
	TIM1->CCR2 = 200;               // 20% Duty Cycle for Channel 2
	TIM1->CCR3 = 200;               // 20% Duty Cycle for Channel 3

	// ---------------------- FINAL ENABLE SEQUENCE ----------------------


	TIM1->CR1 |= (1U << 7);         // ARPE = 1 -> Auto-Reload Preload Enable

    TIM1->EGR = TIM_EGR_UG;			// Generate update to load preload registers

	TIM1->CR1 |= (1U << 0);         // CEN = 1 -> Enable Counter, start PWM generation

	TIM1->SR &= ~TIM_SR_UIF;		// Clear the UG Flag in the SR

	TIM1->EGR = TIM_EGR_COMG;


}

void TIM2_init(void)
{
    // Make sure timer is off before configuring
    TIM2->CR1 = 0;

    // Prescaler: divide 16 MHz system clock down to 1 MHz → 1 tick = 1 µs
    TIM2->PSC = 15;

    // Auto-reload: max value (32-bit timer)
    TIM2->ARR = 0xFFFFFFFF;

    // Force an update event to load the prescaler value
    TIM2->EGR = TIM_EGR_UG;

    // Enable the counter
    TIM2->CR1 |= TIM_CR1_CEN; // FIX THIS LINE!!!
}

void TIM4_init(void)
{
    // Make sure timer is off before configuring
    TIM4->CR1 = 0;

    // Prescaler: divide 16 MHz system clock down to 1 MHz → 1 tick = 1 µs
    TIM4->PSC = 15;

    // Auto-reload: max value (32-bit timer)
    TIM4->ARR = 0xFFFFFFFF;

    // Force an update event to load the prescaler value
    TIM4->EGR = TIM_EGR_UG;

    // Enable the counter
    TIM4->CR1 |= (1U<<0); // FIX THIS LINE!!!
}



//
//// Timer 3 for Input Capture //
//void TIM3_Input_Capture_init(void)
//{
//	// ---------------------- TIMER BASE CONFIGURATION ----------------------
//	// PSC [15:0] = Prescaler value
//	// Timer frequency = 16 MHz / (16000) = 1 MHz tick rate
//	TIM3->PSC = 16-1;     // Set prescaler for 1 kHz timer frequency
//
//	// ARR [15:0] = Auto Reload value
//	// Timer will count from 0 to 0xFFFF (65535)
//	TIM3->ARR = 0xFFFF;        // Max count for full 16-bit range
//
//	// CNT [15:0] = Counter value
//	// Clear counter before enabling the timer
//	TIM3->CNT = 0;             // Reset counter to 0
//
//	TIM3->SR = 0; // Clear Status Register
//
//	// ---------------------- INPUT CAPTURE CONFIGURATION ----------------------
//	// CC1S [1:0] = 01 → CC1 channel is configured as input, IC1 is mapped to TI1
//	TIM3->CCMR1 &= ~(0b11 << 0);   // Clear bits [1:0] (CC1S) to ensure empty
//	TIM3->CCMR1 |=  (0b01 << 0);   // Set CC1S = 01 (CC1 channel as input mapped to TI1)
//
//	// CC2S [9:8] = 01 → CC2 channel is configured as input, IC2 is mapped to TI2
//	TIM3->CCMR1 &= ~(0b11 << 8);   // Clear bits [9:8] (CC2S) to ensure empty
//	TIM3->CCMR1 |=  (0b01 << 8);   // Set CC2S = 01 (CC1 channel as input mapped to TI1)
//
//	// CC3S [1:0] = 01 → CC3 channel is configured as input, IC3 is mapped to TI3
//	TIM3->CCMR2 &= ~(0b11 << 0);   // Clear bits [1:0] (CC3S) to ensure empty
//	TIM3->CCMR2 |=  (0b01 << 0);   // Set CC3S = 01 (CC3 channel as input mapped to TI1)
//
//
//	// ---------------------- CAPTURE EDGE & ENABLE ----------------------
//	// CC1E [0] = 1 → Capture enabled on channel 1
//	// CC1P [1][3] = 1 → Capture on both edges
//	TIM3->CCER |=  (1U << 0);      // Set CC1E = 1 → Enable input capture
//	TIM3->CCER |= ((1U<<1)|(1U<<3)); // Set CC1P & CC1NP = 1 → Capture on both edges
//
//	// CC2E [4] = 1 → Capture enabled on channel 2
//	// CC2P [5][7] = 1 → Capture on both edges
//	TIM3->CCER |=  (1U << 4);      // Set CC2E = 1 → Enable input capture
//	TIM3->CCER |= ((1U<<5)|(1U<<7)); // Set CC2P & CC2NP = 1 → Capture on both edges
//
//	// CC3E [0] = 1 → Capture enabled on channel 3
//	// CC3P [9][11] = 1 → Capture on both edges
//	TIM3->CCER |=  (1U << 8);      // Set CC3E = 1 → Enable input capture
//	TIM3->CCER |= ((1U<<9)|(1U<<11)); // Set CC3P & CC3NP = 1 → Capture on both edges
//
//	// ---------------------- INTERRUPT CONFIGURATION ----------------------
//	// CC1IE [1] = 1 → Enable interrupt on capture event
//	TIM3->DIER |= (1U << 1);       // Enable capture interrupt on channel 1
//
//	// ---------------------- NVIC INTERRUPT ENABLE ----------------------
//	// Enable TIM3 interrupt line in the Nested Vector Interrupt Controller
//	//NVIC_EnableIRQ(TIM3_IRQn);     // Allow TIM3 interrupt to trigger handler
//
//	// ---------------------- START TIMER ----------------------
//	// CEN [0] = 1 → Counter enable
//	TIM3->CR1 |= (1U << 0);        // Enable timer to start counting
//}
//
//
//void TIM3_IRQHandler(void){
//
//	if (TIM3->SR & TIM3_SR_CC1IF)
//	{
//		TIM3->SR &=~ TIM3_SR_CC1IF; // Phase A Interrupt Flag - Edge Observed at Phase A
//
//		printf("Received BEMF at TIM3 Channel 1...\r\n");
//
//		if(GPIOC->IDR & (1U<<6)){ // C Rising Edge
//			printf("BEMF C was Rising Edge...\r\n");
//			//commutation(4);
//		}
//		else{						// C Falling Edge
//			printf("BEMF C was Falling Edge...\r\n");
//			//commutation(1);
//		}
//
//	}
//	else if(TIM3->SR & TIM3_SR_CC2IF)
//	{
//		TIM3->SR &=~ TIM3_SR_CC2IF; // Phase B Interrupt Flag - Edge Observed at Phase B
//
//		printf("Received BEMF at TIM3 Channel 2...\r\n");
//
//		if(GPIOC->IDR&(1U<<7)){	// B Rising Edge
//			printf("BEMF B was Rising Edge...\r\n");
//			//commutation(2);
//		}
//		else{						// B Falling Edge
//			printf("BEMF B was Falling Edge...\r\n");
//			//commutation(5);
//		}
//
//	}
//	else if(TIM3->SR & TIM3_SR_CC3IF)
//	{
//		TIM3->SR &=~ TIM3_SR_CC3IF; // Phase C Interrupt Flag - Edge Observed at Phase C
//
//
//		printf("Received BEMF at TIM3 Channel 3...\r\n");
//
//		if(GPIOC->IDR&(1U<<8)){	// A Rising Edge
//			printf("BEMF A was Rising Edge...\r\n");
//			//commutation(6);
//		}
//		else{						// A Falling Edge
//			printf("BEMF A was Falling Edge...\r\n");
//			//commutation(3);
//		}
//
//	}
//
//}
