#ifndef __KEY_H
#define __KEY_H

uint8_t Key_check(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Key_init(void);

#endif 
