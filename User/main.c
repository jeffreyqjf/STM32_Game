#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "GPIO_init.h"
#include "OLED.h"
#include "Key.h"
#include "Timer.h"

#include <stdlib.h>
#include <string.h>

#define HOME_PAGE  ((uint16_t)1)
#define MENU_PAGE  ((uint16_t)2)
#define GAME_PAGE  ((uint16_t)3)
#define COMBAT_STRUCT_LEN ((int)11)
	
#define GAME_LASTING_TIME ((uint16_t)15)

typedef struct {
	char name[32];
	char arrow[16];
}Combat_Readiness_Struct;
Combat_Readiness_Struct combat_readiness[20];

uint16_t status = HOME_PAGE;

void Data_init(void){
	// arrow use w, s, a, d 
	strcpy(combat_readiness[0].name, "Eagle 500kg Bomb");strcpy(combat_readiness[0].arrow, "WDSSS\0");
	strcpy(combat_readiness[1].name, "Orbital Gatling Barrage");strcpy(combat_readiness[1].arrow, "DSAWWW\0");
	strcpy(combat_readiness[2].name, "Orbital Airburst Strike");strcpy(combat_readiness[2].arrow, "DDD\0");
	strcpy(combat_readiness[3].name, "Orbital 120mm HE Barrage");strcpy(combat_readiness[3].arrow, "DDSADS\0");
	strcpy(combat_readiness[4].name, "Orbital 380mm HE Barrage");strcpy(combat_readiness[4].arrow, "DSWWASS\0");
	strcpy(combat_readiness[5].name, "Orbital Walking Barrage");strcpy(combat_readiness[5].arrow, "DSDSDS\0");
	strcpy(combat_readiness[6].name, "Orbital Laser");strcpy(combat_readiness[6].arrow, "DSWDS\0");
	strcpy(combat_readiness[7].name, "Orbital Napalm Barrage");strcpy(combat_readiness[7].arrow, "DDSADW\0");
	strcpy(combat_readiness[8].name, "Orbital Railcannon Strike");strcpy(combat_readiness[8].arrow, "DWSSD\0");
	strcpy(combat_readiness[9].name, "Eagle Strafing Run");strcpy(combat_readiness[9].arrow, "WDD\0");
	strcpy(combat_readiness[10].name, "Eagle Airstrike");strcpy(combat_readiness[10].arrow, "WDSD\0");
	strcpy(combat_readiness[11].name, "Eagle Cluster Bomb");strcpy(combat_readiness[11].arrow, "WDSSD\0");
}


void Home_Page(void){
	OLED_Printf(0, 0, OLED_6X8, "Look familiar?");
	OLED_Printf(0, 10, OLED_6X8, "You!could!be!next!");
	
	if(Button_any_close()){
		status = GAME_PAGE;
		OLED_Clear();
	}
}


void Menu_Page(void){

}


uint8_t Game_Ready = 0; // sure to play game?
int Score = 0;  // first in and become 0
int Game_Time = GAME_LASTING_TIME; // leaving time
int Rand_Num = 0; // give a new num; give the 500kg first
	
uint8_t Finished_Pointer = 0; // use to know the input pointer
uint8_t Combat_Length = 5;  // record the combat_readiness length and not equal to Finished_Pointer
uint8_t Right_Tag = 1; // confirm right or not


void Show_Time_Bar(void){
	/*
	128 / GAME_LASTING_TIME * Game_Time
	*/
	OLED_DrawRectangle(0, 60, 128 / GAME_LASTING_TIME * Game_Time, 4, OLED_FILLED);
}


void Score_Page(int Score){
	OLED_Printf(30, 0, OLED_8X16, "GAME OVER!", Score);
	OLED_Printf(35, 20, OLED_8X16, "score: %d", Score);
	
	OLED_ShowString(0, 45, "back", OLED_8X16);
	OLED_ReverseArea(0, 45, 32, 61);
	OLED_ShowString(85, 45, "again", OLED_8X16);
	OLED_ReverseArea(85, 45, 127, 61);
	OLED_Update();
	
	while(1){
		if(Key_check(GPIOA, GPIO_Pin_2) == 1){
			status = HOME_PAGE;
			OLED_Clear();
			break;
		}
		//confirm, back to delete_page and delete the data
		if(Key_check(GPIOA, GPIO_Pin_3) == 1){
			status = GAME_PAGE;
			OLED_Clear();
			break;
		}
	}
}


void Game_Page_Ready(void){
	// The game function
	
	TIM_Cmd(TIM2,ENABLE); // begin to record time
	
	
	/*
	if(Right_Tag == 1){ // finish and give a new combat_readiness
		// add score
		
	}
	*/
	
	if(Key_check(GPIOA, GPIO_Pin_2)){
	// press A
		if(combat_readiness[Rand_Num].arrow[Finished_Pointer] == 'A'){ // press right
			Finished_Pointer += 1; // next arrow
		}else{ // not right and back to first arrow
			Finished_Pointer = 0;
		}
	}
	if(Key_check(GPIOA, GPIO_Pin_3)){
	//press D
		if(combat_readiness[Rand_Num].arrow[Finished_Pointer] == 'D'){ // press right
			Finished_Pointer += 1; // next arrow
		}else{
			Finished_Pointer = 0;
		}
	}
	if(Key_check(GPIOA, GPIO_Pin_4)){
	//press W
		if(combat_readiness[Rand_Num].arrow[Finished_Pointer] == 'W'){ // press right
			Finished_Pointer += 1; // next arrow
		}else{
			Finished_Pointer = 0;
		}
	}
	if(Key_check(GPIOA, GPIO_Pin_5)){
	//press S
		if(combat_readiness[Rand_Num].arrow[Finished_Pointer] == 'S'){ // press right
			Finished_Pointer += 1; // next arrow
		}else{
			Finished_Pointer = 0;
		}
	}
	
	if(Finished_Pointer == Combat_Length){ // all is right
		Score += 1; // simple, should improve!!
		Rand_Num = rand() % COMBAT_STRUCT_LEN; // give a new num
		Combat_Length = strlen(combat_readiness[Rand_Num].arrow);
		Finished_Pointer = 0;
		Game_Time += 1; // give more time
		// Right_Tag = 0; // begin
	}
	
	
	// show on OLED
	OLED_Clear(); // in order to show clean screen
	OLED_Printf(0, 0, OLED_6X8, combat_readiness[Rand_Num].name); // show the combat_readiness's name
	
	for(int i = 0; i < Finished_Pointer; i++){ //show the right arrow
		switch(combat_readiness[Rand_Num].arrow[i]){
			case 'W':
				OLED_ShowImage(i * 16, 30, 16, 16, W_filled);
				break;
			case 'S':
				OLED_ShowImage(i * 16, 30, 16, 16, S_filled);
				break;
			case 'A':
				OLED_ShowImage(i * 16, 30, 16, 16, A_filled);
				break;
			case 'D':
				OLED_ShowImage(i * 16, 30, 16, 16, D_filled);
				break;
			default:
				break;
		}
	}
	
	for(int i = Finished_Pointer; i < Combat_Length; i++){
		switch(combat_readiness[Rand_Num].arrow[i]){
			case 'W':
				OLED_ShowImage(i * 16, 30, 16, 16, W_unfilled);
				break;
			case 'S':
				OLED_ShowImage(i * 16, 30, 16, 16, S_unfilled);
				break;
			case 'A':
				OLED_ShowImage(i * 16, 30, 16, 16, A_unfilled);
				break;
			case 'D':
				OLED_ShowImage(i * 16, 30, 16, 16, D_unfilled);
				break;
			default:
				break;
		}
	}
	OLED_Printf(0, 50, OLED_6X8, "%d", Rand_Num);
	OLED_Printf(20, 50, OLED_6X8, "%d", Score);
	// OLED_Printf(40, 50, OLED_6X8, "%d", Combat_Length);
	// OLED_Printf(60, 50, OLED_6X8, "%d", Finished_Pointer);
	OLED_Printf(40, 50, OLED_6X8, "%d", Game_Time);
	Show_Time_Bar();
	
	if(Game_Time < 0){ // have time to play
		Game_Ready = 0;
		
		OLED_Clear();
		TIM_Cmd(TIM2,DISABLE);
		Game_Time = GAME_LASTING_TIME;
		
		Score_Page(Score);
		Score = 0;
	}
}


void Game_Page_nReady(void){
	// The game confirm function 
	OLED_Printf(0, 0, OLED_8X16, "Game Ready?");
	OLED_Printf(0, 20, OLED_8X16, "Join the");
	OLED_Printf(0, 40, OLED_8X16, "Helldivers!");
	
	if(Key_check(GPIOA, GPIO_Pin_3)){
		status = GAME_PAGE;
		Game_Ready = 1;
		Score = 0;
		OLED_Clear();
	}
	
	if(Key_check(GPIOA, GPIO_Pin_2)){
		status = HOME_PAGE;
		Game_Ready = 0;
		Score = 0;
		OLED_Clear();
	}
}


void Game_Page(void){
	if(Game_Ready){
		Game_Page_Ready();
	}else{
		Game_Page_nReady();
	}
}


int main(void)
{
	OLED_Init();
	Key_init();
	Data_init();
	Timer_init();
	
	while(1){
		switch(status){
			case HOME_PAGE:
				Home_Page();
				break;
			case MENU_PAGE:
				Menu_Page();
				break;
			case GAME_PAGE:
				Game_Page();
				break;
			default:
				break;
		}
		
		OLED_Update();
		Indicator_light_init();
	}
}




void TIM2_IRQHandler(void){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
		Game_Time -= 1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
