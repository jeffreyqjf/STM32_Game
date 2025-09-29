#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "GPIO_init.h"

uint8_t Key_check(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	// all button is activate with 0
	uint8_t tag = 0;
	if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 0){
		tag = 1;
		Delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 0);
		Delay_ms(15);
	}
	return tag;
}


void Key_init(void){
	GPIOA_init(GPIO_Mode_IPU, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
	GPIOB_init(GPIO_Mode_IPU, GPIO_Pin_6 | GPIO_Pin_7);
}


uint8_t Key_any_close(void){
	// if tag is 1, the button close one or more
	uint8_t tag = 0;
	if(Key_check(GPIOA, GPIO_Pin_2) == 1){tag = 1;}
	if(Key_check(GPIOA, GPIO_Pin_3) == 1){tag = 1;}
	if(Key_check(GPIOA, GPIO_Pin_4) == 1){tag = 1;}
	if(Key_check(GPIOA, GPIO_Pin_5) == 1){tag = 1;}
	if(Key_check(GPIOB, GPIO_Pin_6) == 1){tag = 1;}
	if(Key_check(GPIOB, GPIO_Pin_7) == 1){tag = 1;}
	
	return tag;

}
