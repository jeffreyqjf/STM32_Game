#include "stm32f10x.h"                  // Device header


void Timer_init(void){
	/* This function is used for initing the TIM2, the code should be changed if you use other TIMx; */
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// switch Clock
	TIM_InternalClockConfig(TIM2);
	
	// TIM(Timer) init
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1; // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00; // not use 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // TIM_IT_Update is about what?
	
	// NVIC init
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	// Turn up TIM2
	// TIM_Cmd(TIM2,ENABLE);
}


/*
void TIM2_IRQHandler(void){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/
