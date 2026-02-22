#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "timer.h"

#define Phase_A_High	(TIM_CCER_CC1E)
#define Phase_A_Low		(TIM_CCER_CC1NE)
#define Phase_B_High	(TIM_CCER_CC2E)
#define Phase_B_Low		(TIM_CCER_CC2NE)
#define Phase_C_High	(TIM_CCER_CC3E)
#define Phase_C_Low		(TIM_CCER_CC3NE)

#define PWM_MODE		(0b110)
#define FORCE_INACTIVE	(0b100)
#define	FORCE_ACTIVE	(0b101)

#define CCMR1_OC1M		(0b111<<4)
#define CCMR1_OC2M		(0b111<<12)
#define CCMR2_OC3M		(0b111<<4)

#define CCER_ENABLE_MASK 		(Phase_A_High | Phase_A_Low | Phase_B_High | Phase_B_Low | Phase_C_High | Phase_C_Low)

#define CCMR1_ENABLE_MASK	(CCMR1_OC1M | CCMR1_OC2M)
#define CCMR2_ENABLE_MASK	(CCMR2_OC3M)

static uint8_t step = 0;

static Step_Configuration config_table[6] = {

		// 0. A+, B- // A8, B0
		{
				.ccer	= (Phase_A_High | Phase_B_Low),
				.ccmr1 	= ((PWM_MODE<<TIM_CCMR1_OC1M_Pos) |(FORCE_ACTIVE<<TIM_CCMR1_OC2M_Pos)),
				.ccmr2	= (FORCE_INACTIVE<<TIM_CCMR2_OC3M_Pos),
				// Phase A 100 -> 110, Phase B 100 -> 101, Phase C 100->100
				//.ccr 	= {200,200,0},
		},

		// 1. A+, C- //  A8, B1
		{
				.ccer	= Phase_A_High | Phase_C_Low,
				.ccmr1 	= ((PWM_MODE<<TIM_CCMR1_OC1M_Pos) |(FORCE_INACTIVE<<TIM_CCMR1_OC2M_Pos) ),
				.ccmr2	= (FORCE_ACTIVE<<TIM_CCMR2_OC3M_Pos),
				// Phase A 110 -> 110, Phase B 101 -> 100, Phase C 100->101
				//.ccr 	= {200,0,200},

		},
		// 2. B+, C- // A9, B1
		{
				.ccer	= Phase_B_High | Phase_C_Low,
				.ccmr1 	= ((FORCE_INACTIVE<<TIM_CCMR1_OC1M_Pos) |(PWM_MODE<<TIM_CCMR1_OC2M_Pos )),
				.ccmr2	= (FORCE_ACTIVE<<TIM_CCMR2_OC3M_Pos),
				// Phase A 110 -> 100, Phase B 100 -> 110, Phase C 101->101
				//.ccr 	= {0,200,200},

		},
		// 3. B+, A- // A9, A7
		{
				.ccer	= Phase_B_High | Phase_A_Low,
				.ccmr1 	= ((FORCE_ACTIVE<<TIM_CCMR1_OC1M_Pos) |(PWM_MODE<<TIM_CCMR1_OC2M_Pos )),
				.ccmr2	= (FORCE_INACTIVE<<TIM_CCMR2_OC3M_Pos),
				// Phase A 100 -> 101, Phase B 110 -> 110, Phase C 101->100
				//.ccr 	= {200,200,0},

		},
		// 4. C+, A- // A10, A7
		{
				.ccer	= Phase_C_High | Phase_A_Low,
				.ccmr1 	= ((FORCE_ACTIVE<<TIM_CCMR1_OC1M_Pos)|(FORCE_INACTIVE<<TIM_CCMR1_OC2M_Pos) ),
				.ccmr2	= (PWM_MODE<<TIM_CCMR2_OC3M_Pos),
				// Phase A 101 -> 101, Phase B 110 -> 100, Phase C 100->110
				//.ccr 	= {200,0,200},

		},
		// 5. C+, B- // A10, B0
		{
				.ccer	= Phase_C_High | Phase_B_Low,
				.ccmr1 	= ((FORCE_INACTIVE<<TIM_CCMR1_OC1M_Pos )|(FORCE_ACTIVE<<TIM_CCMR1_OC2M_Pos )),
				.ccmr2	= (PWM_MODE<<TIM_CCMR2_OC3M_Pos),
				// Phase A 101 -> 100, Phase B 100 -> 101, Phase C 110->110
				//.ccr 	= {0,200,200},

		},

};



void commutation(void){

	//Step_Configuration* config = &config_table[step];
	TIM1->CCMR1 &=~ CCMR1_ENABLE_MASK;
	TIM1->CCMR1 |= config_table[step].ccmr1;

	TIM1->CCMR2 &=~ CCMR2_ENABLE_MASK;
	TIM1->CCMR2 |= config_table[step].ccmr2;

	TIM1->CCER &=~ CCER_ENABLE_MASK;
	TIM1->CCER |= config_table[step].ccer;


//	TIM1->CCR1	= config->ccr[0];
//	TIM1->CCR2	= config->ccr[1];
//	TIM1->CCR3	= config->ccr[2];

	TIM1->EGR = TIM_EGR_COMG;

	delay_us(10);

	step++;

    if(step >= 6){ // this is for testing only

    	step = 4;

    }


}

void set_duty_cycle(uint16_t duty){

	TIM1->CCR1 = duty;
	TIM1->CCR2 = duty;
	TIM1->CCR3 = duty;

}

void delay_us(uint32_t us){

    TIM2->CNT = 0;           // reset the counter

   // led_toggle();

    while (TIM2->CNT < us);  // wait until desired count

}

void ramp(void){


	TIM1->CCR1 = 100;
	TIM1->CCR2 = 100;
	TIM1->CCR3 = 100;

	uint16_t delay = 3000;

	uint8_t lap = 0;

	//TIM4-> CNT = 0;

	while(delay >100 && TIM1->CCR1 < 600) {

		if(lap<=5){

			for(uint8_t step = 1; step <= 6; step++){
						printf("Step %d\r\n", step);
						//commutation(step);
						delay_us(delay);
				}

			if(delay!=100){
				delay = delay - 100;
			}

			lap ++;
		}

		if(TIM1->CCR1!=600){
			TIM1->CCR1 = TIM1->CCR1 + 100;
			TIM1->CCR2 = TIM1->CCR2 + 100;
			TIM1->CCR3 = TIM1->CCR3 + 100;
		}

	}

}

void alignment(void){

	printf("Starting Alignment\r\n");

	//commutation(10);

//	for(uint8_t step = 1; step<=1; step++){
//
//		printf("Aligning for Step: %d\r\n", step);
//
//		commutation(step);
//
//		delay_us(1000000); // 1 Second Delay
//	}

	printf("Alignment Over\r\n");

}

void commutation_old(uint8_t step){

    TIM1->CCER &= ~(
        Phase_A_High | Phase_A_Low |
        Phase_B_High | Phase_B_Low |
        Phase_C_High | Phase_C_Low
    );

	switch(step){
	case 1:
		// 60 Degree - Phase C floats - Falling Edge BEMF Zero Crossing
		printf("60 Degree...\r\n");
		TIM1->CCER = Phase_A_High | Phase_B_Low;
		//TIM1->CCER = Phase_B_Low;

		break;

	case 2:
		// 120 Degree - Phase B floats - Rising Edge BEMF Zero Crossing
		printf("120 Degree...\r\n");
		TIM1->CCER = Phase_A_High | Phase_C_Low;
		//TIM1->CCER = Phase_C_Low;

		break;
	case 3:
		// 180 Degree - Phase A floats - Falling Edge BEMF Zero Crossing
		printf("180 Degree...\r\n");
		TIM1->CCER = Phase_B_High | Phase_C_Low;
		//TIM1->CCER = Phase_C_Low;
		break;

	case 4:
		// 240 Degree - Phase C floats - Rising Edge BEMF Zero Crossing
		printf("240 Degree...\r\n");
		TIM1->CCER = Phase_B_High | Phase_A_Low;
		//TIM1->CCER = Phase_A_Low;

		break;
	case 5:
		// 300 Degree - Phase B floats - Falling Edge BEMF Zero Crossing
		printf("300 Degree...\r\n");
		TIM1->CCER = Phase_C_High | Phase_A_Low;
		//TIM1->CCER = Phase_A_Low;

		break;
	case 6:
		// 360 Degree - Phase A floats - Rising Edge BEMF Zero Crossing
		printf("360 Degree...\r\n");
		TIM1->CCER = Phase_C_High | Phase_B_Low;
		//TIM1->CCER = Phase_B_Low;
		break;
	case 7:
		//TIM1->CCER = Phase_A_Low | Phase_B_Low | Phase_C_Low ;
		printf("Step 7... Phase_A_High | Phase_B_Low\r\n");
		TIM1->CCER = Phase_A_High | Phase_B_Low;
		break;
	case 8:
		//TIM1->CCER = Phase_A_Low | Phase_B_Low | Phase_C_Low ;
		printf("Step 8... Phase_B_High | Phase_C_Low \r\n");
		TIM1->CCER = Phase_B_High | Phase_C_Low;
		break;
	case 9:
		//TIM1->CCER = Phase_A_Low | Phase_B_Low | Phase_C_Low ;
		printf("Step 9... Phase_C_High | Phase_B_Low \r\n");
		TIM1->CCER = Phase_C_High | Phase_B_Low;
		break;
	case 10:
		printf("Alignment Phase A \r\n");
		TIM1->CCR1 = 200;
		TIM1->CCER |= Phase_A_High | Phase_A_Low;

		//delay(1000000);

		TIM1->CCER &=~ Phase_A_High | Phase_A_Low;
		TIM1->CCR1=400;
	}

}

void open_loop(void){
//
//	uint32_t delay = 5000;
//
//	uint8_t step = 1;
//
//	uint8_t false_trigger_count = 0;
//
//	printf("Initiating Open Loop...\r\n");
//
//	TIM3->SR &= ~(TIM3_SR_CC1IF | TIM3_SR_CC2IF | TIM3_SR_CC3IF);
//	NVIC_DisableIRQ(TIM3_IRQn);
//	TIM3->SR = 0;  // Clear any pending flags
//
//	while( !(TIM3->SR & (TIM3_SR_CC1IF | TIM3_SR_CC2IF | TIM3_SR_CC3IF))){
//
//	// Debug: Show which step we're in
//			printf("Step %d:\r\n ", step);
//
//			printf("No BEMF Detected... \r\n");
//			printf("Aligning Motor... \r\n");
//
//			commutation(step);
//
//			delay_us(100);
//
//	        // Check if flag was set DURING or IMMEDIATELY AFTER commutation (likely noise)
//	        if(TIM3->SR & (TIM3_SR_CC1IF | TIM3_SR_CC2IF | TIM3_SR_CC3IF)) {
//	            false_trigger_count++;
//	            printf("False trigger %d detected and ignored\r\n", false_trigger_count);
//	            TIM3->SR &= ~(TIM3_SR_CC1IF | TIM3_SR_CC2IF | TIM3_SR_CC3IF);
//	        }
//
//
//			delay_us(delay);
//
//
//			printf("Waiting... Delay=%lu\r\n", delay);
//	        // Check for REAL BEMF (occurred during the main delay, not immediately after commutation)
//	        if(TIM3->SR & (TIM3_SR_CC1IF | TIM3_SR_CC2IF | TIM3_SR_CC3IF)) {
//	            printf("Real BEMF Detected! Flag=0x%08lX\r\n", TIM3->SR);
//	            break;
//	        }
//
//
//			// Increment step and handle wrap-around
//			step++;
//			if(step > 6){
//				printf("Open Loop Alignment Complete...\r\n");
//				step = 1;
//				printf("Open Loop Alignment Reset...\r\n");
//			}
//
//
//		if(delay == 0){
//
//			delay = 5000;
//		}
//		else{
//			delay = delay - 50;
//
//		}
//
//		printf("Checking for BEMF again...\r\n");
//
//	}
//
//	delay_us(100);
//	printf("BEMF Detected, Firing Timer 3 Interrupt Routine... \r\n");
//	NVIC_EnableIRQ(TIM3_IRQn);
}

