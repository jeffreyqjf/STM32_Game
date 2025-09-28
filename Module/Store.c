#include "stm32f10x.h"                  // Device header
#include "MyFlash.h"
#include "main.h"
#include "OLED.h"
#include <string.h>


#define STORE_START_ADDRESS      0x0800FC00
#define STORE_COUNT              512


uint16_t Store_Data[STORE_COUNT];

extern Rank_Struct rank_struct[16];
extern uint16_t rank_array_len;


void Store_init(void){
	
	if(MyFLASH_ReadHalfWord(STORE_START_ADDRESS) != 0xA5A5)
	{
		MyFLASH_ErasePage(STORE_START_ADDRESS);
		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS, 0xA5A5);
		for(int i = 1; i<STORE_COUNT; i++)
		{
			MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i * 2, 0x0000);
		}
	}
	
	for(int i = 0; i < STORE_COUNT; i++)
	{
		Store_Data[i] = MyFLASH_ReadHalfWord(STORE_START_ADDRESS + 2 * i);
	}
	
}


void Store_Save(void){
	MyFLASH_ErasePage(STORE_START_ADDRESS);
	for(int i = 0; i < STORE_COUNT;i ++)
	{
		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i * 2, Store_Data[i]);
	}
}


void Store_Clear(void){
	for(int i = 1; i < STORE_COUNT;i ++)
	{
		Store_Data[i] = 0x0000;
	}
	Store_Save();
}


void Store_Save_rank_struct(void){
	/*
	typedef struct {
		char name[16];
		double score;
	}Rank_Struct;
	0-19 A5A50007
						LEN
	*/ 
	Store_Clear();
	size_t bytes = sizeof(Rank_Struct) * rank_array_len ;
  memcpy((uint8_t*)(Store_Data + 10), (uint8_t*)rank_struct, bytes); // null space is 20 bytes
	Store_Data[1] = (uint16_t)rank_array_len;
	Store_Save();
}


void Store_read_rank_struct(void){
	rank_array_len = Store_Data[1];
	size_t bytes = sizeof(Rank_Struct) * rank_array_len;
	memcpy((uint8_t*)rank_struct, (uint8_t*)(Store_Data + 10), bytes);
	
}


void Store_show_array(void){
	OLED_Clear();
	for(int i = 0;i < 6 ;i ++){
		OLED_ShowHexNum(20*i, 0, Store_Data[i], 2, OLED_6X8);
		OLED_ShowHexNum(20*i, 10, Store_Data[i+6], 2, OLED_6X8);
		OLED_ShowHexNum(20*i, 20, Store_Data[i+12], 2, OLED_6X8);
		OLED_ShowHexNum(20*i, 30, Store_Data[i+18], 2, OLED_6X8);
		OLED_ShowHexNum(20*i, 40, Store_Data[i+24], 2, OLED_6X8);
		OLED_ShowHexNum(20*i, 50, Store_Data[i+30], 2, OLED_6X8);
	}
	OLED_Update();
}
