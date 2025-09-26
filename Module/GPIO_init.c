#include "stm32f10x.h"                  // Device header
#include "Key.h"


void GPIOA_init(GPIOMode_TypeDef GPIO_Mode, uint16_t GPIO_Pin_x){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void GPIOB_init(GPIOMode_TypeDef GPIO_Mode, uint16_t GPIO_Pin_x){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void GPIOC_init(GPIOMode_TypeDef GPIO_Mode, uint16_t GPIO_Pin_x){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


void Indicator_light_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


uint8_t Button_any_close(void){
	// if tag is 1, the button close one or more
	uint8_t tag = 0;
	if(Key_check(GPIOA, GPIO_Pin_2) == 1){tag = 1;}
	if(Key_check(GPIOA, GPIO_Pin_3) == 1){tag = 1;}
	if(Key_check(GPIOA, GPIO_Pin_4) == 1){tag = 1;}
	if(Key_check(GPIOA, GPIO_Pin_5) == 1){tag = 1;}
	
	return tag;

}

