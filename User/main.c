#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "GPIO_init.h"
#include "OLED.h"
#include "Key.h"
#include "Timer.h"
#include "main.h"
#include "Store.h"

#include <stdlib.h>
#include <string.h>

#define HOME_PAGE  ((uint16_t)1)
#define MENU_PAGE  ((uint16_t)2)
#define GAME_PAGE  ((uint16_t)3)
#define RANK_PAGE  ((uint16_t)4)

#define COMBAT_STRUCT_LEN ((int)11)
#define GAME_LASTING_TIME ((double)15.0)
#define LEVEL_PAGE_REMAIN_TIME ((int)5)
#define NAME_LEN ((uint8_t)32)

typedef struct {
	char name[32];
	char arrow[16];
}Combat_Readiness_Struct;
Combat_Readiness_Struct combat_readiness[20];

// next time refactor can change 16 to define
Rank_Struct rank_struct[16];
uint16_t rank_array_len = 0;

uint16_t status = HOME_PAGE;

void Data_init(void){
	// read flash data
	Store_read_rank_struct();
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
	OLED_Clear();
	OLED_Printf(0, 0, OLED_6X8, "Look familiar?");
	OLED_Printf(0, 10, OLED_6X8, "You!could!be!next!");
	

	OLED_ShowString(0, 45, "Rank", OLED_8X16);
	OLED_ReverseArea(0, 45, 32 + 1, 61);
	OLED_ShowString(95, 45, "Game", OLED_8X16);
	OLED_ReverseArea(95 - 1, 45, 127, 61);
	
	if(Key_check(GPIOB, GPIO_Pin_7)){
		status = GAME_PAGE;
		OLED_Clear();
	}
	
	if(Key_check(GPIOB, GPIO_Pin_6)){
		status = RANK_PAGE;
		OLED_Clear();
	}
}


void Menu_Page(void){

}


uint8_t Game_Ready = 0; // sure to play game?
double Score = 0.0;  // first in and become 0
double Game_Time = GAME_LASTING_TIME; // leaving time
int Rand_Num = 0; // give a new num; give the 500kg first
uint8_t Finished_Pointer = 0; // use to know the input pointer

uint8_t Combat_Length = 5;  // record the combat_readiness length and not equal to Finished_Pointer

uint8_t Count_Fininshed_Combat = 0;
uint8_t Count_Next_Level_Combat = 4;
uint8_t All_Right = 1;
uint8_t level = 1;


void Game_Para_Init(void){
	// when first play game, the parameter that should init
	Score = 0.0;
	Game_Time = GAME_LASTING_TIME;
	Rand_Num = rand() % COMBAT_STRUCT_LEN; // give a new num
	Combat_Length = strlen(combat_readiness[Rand_Num].arrow);
	Finished_Pointer = 0; 
	Count_Fininshed_Combat = 0;
	All_Right = 1;
	level = 1;
}


void Show_Time_Bar(void){
	/*
	128 / GAME_LASTING_TIME * Game_Timeq
	*/
	OLED_DrawRectangle(0, 60, (uint8_t)(128.0 / GAME_LASTING_TIME * Game_Time), 4, OLED_FILLED);
}


int8_t input_cursor = 0;

char input_char = 'a';
char input_name_array[NAME_LEN];

void Input_Username(uint8_t Rank, Rank_Struct *rank_struct){
	// have bugs
	
	OLED_Clear();
	uint8_t exit_flag = 0;
	
	OLED_Printf(0, 0, OLED_6X8, "YOU RANK:%d", Rank);
	OLED_Printf(0, 10, OLED_6X8, "input your name:");
	
	while(!exit_flag){
		if((Key_check(GPIOB, GPIO_Pin_6) == 1)){
					status = HOME_PAGE;
					OLED_Clear();
					exit_flag = 1;
					break;
		}
				
		// change the char, only support a-z,input 'a' - 1 to stop
		if((Key_check(GPIOA, GPIO_Pin_5) == 1)){
			input_char += 1;
			OLED_ClearArea(10 * input_cursor, 30, 8, 16);
			}
		if(Key_check(GPIOA, GPIO_Pin_4) == 1){
			input_char -= 1;
			OLED_ClearArea(10 * input_cursor, 30, 8, 16);
			}
		if(Key_check(GPIOB, GPIO_Pin_7) == 1){
			input_name_array[input_cursor] = input_char;
			input_cursor += 1;
			if(input_cursor > 0){OLED_ReverseArea(10 * (input_cursor - 1), 30, 8, 16);}				// reverse the front char
			
			// finish
			if(input_char == 'a' - 1){
				
				input_name_array[input_cursor - 1] = '\0';
				// init again
				input_cursor = 0;
				input_char = 'a';
				
				strcpy(rank_struct[Rank].name, input_name_array); 
				strcpy(input_name_array, "\0");
				status = RANK_PAGE;
				exit_flag = 1;
				OLED_Clear();
				break; 
			}	
			input_char = 'a';
		}
		OLED_ShowChar(10 * input_cursor, 30, input_char, OLED_8X16);
		OLED_ReverseArea(10 * input_cursor, 30, 8, 16);
		OLED_Update();
	}
}

void Rank_Score(double Score, Rank_Struct *rank_struct, uint16_t *rank_array_len){
	// if rank_array_len < 16, append a user and rank, else, rank and delete the final one
	// rank 0 - rank 15
	// if you get a rank ,then you can input your name to record, now it's ST
	uint8_t MyRank = *rank_array_len; // the last one
	for(int i = *rank_array_len - 1; i >= 0; i--)
	{
		if(Score > rank_struct[i].score)
		{
			MyRank = i; // find new score's position
		}
	}
	
	/*
	if(*rank_array_len == 0){
		MyRank = 0;
	}
	*/
	// OLED_Printf(40, 20, OLED_8X16, "%d", MyRank);
	
	if(MyRank <= 15){ 
		
		// can have less loop use name_array_len
		for(int i = 15; i > MyRank; i--)// cpoy and move old user 
		{
			rank_struct[i] = rank_struct[i - 1]; 
			// strcpy(rank_struct[i].name, rank_struct[i - 1].name);
			// rank_struct[i].score = rank_struct[i - 1].score;
		}
		//insert the new user
		Input_Username(MyRank, rank_struct);
		// strcpy(rank_struct[MyRank].name, user_name);
		rank_struct[MyRank].score = Score;
	}
	
	if(*rank_array_len < 15)
	{
		*rank_array_len += 1;
	}
}


void Score_Page(double Score){
	OLED_Clear();
	
	// here should rank the score
	Rank_Score(Score, rank_struct, &rank_array_len);
	
	OLED_Clear();
	OLED_Printf(30, 0, OLED_8X16, "GAME OVER!");
	OLED_Printf(25, 20, OLED_8X16, "score: %0.1lf", Score);
	
	OLED_ShowString(0, 45, "back", OLED_8X16);
	OLED_ReverseArea(0, 45, 32 + 1, 61);
	OLED_ShowString(85, 45, "again", OLED_8X16);
	OLED_ReverseArea(85 - 1, 45, 127, 61);
	
	
	
	// memcpy(&rank_struct[rank_array_len].score, &Score, 8); 
	// OLED_Printf(100, 20, OLED_8X16, "%0.1lf", rank_struct[rank_array_len].score);
	// strcpy(rank_struct[rank_array_len].name, "ST");
	
	Store_Save_rank_struct();
	OLED_Update();
	
	while(1){
		if(Key_check(GPIOB, GPIO_Pin_6) == 1){
			status = HOME_PAGE;
			OLED_Clear();
			break;
		}
		
		if(Key_check(GPIOB, GPIO_Pin_7) == 1){
			status = GAME_PAGE;
			OLED_Clear();
			break;
		}
	}
}


int next_level_page_remain_time = LEVEL_PAGE_REMAIN_TIME; // remain 5s

void next_level_page(uint8_t All_Right, double Game_Time, uint8_t * level, double * Score){
	// the page use to remind user to play next level
	
	TIM_Cmd(TIM3, ENABLE);
	
	double Addition_Score = 0.0;
	
	while(next_level_page_remain_time > 0){
		
		Addition_Score = 0.0;
		
		if(next_level_page_remain_time <= 4){
			OLED_Printf(0, 0, OLED_6X8, "level %d finished!", *level);
		}
		
		if(next_level_page_remain_time <= 3){
			double Add_Score = 0.0;
			if(All_Right == 1){
				Add_Score = 3.0 * (double)(*level);
				Addition_Score += Add_Score; 
			}
			OLED_Printf(0, 15, OLED_6X8, "All Right: %0.1lf", Add_Score);
		}
		
		if(next_level_page_remain_time <= 2){
				double Add_Score = (double)Game_Time * 1.0; 
				Addition_Score += Add_Score;
				OLED_Printf(0, 30, OLED_6X8, "Game Time: %0.1lf", Add_Score);
		}
		OLED_Printf(45, 45, OLED_8X16, "%d", next_level_page_remain_time);
		OLED_Update();
	}
	
	TIM_Cmd(TIM3, DISABLE);
	
	// finish and to next level
	*Score += Addition_Score;
	*level += 1;
	
	// in next level should init:next_level_page_remain_time, All_Right, other had init when go into this function
	next_level_page_remain_time = LEVEL_PAGE_REMAIN_TIME;
	All_Right = 1;
}


void one_level(uint8_t * level){
	// main game
	
	TIM_Cmd(TIM2, ENABLE); // begin to record time
	
	// next combat_readiness
	if(Finished_Pointer == Combat_Length){ // all is right
		Score += 1.0; // simple, should improve!!
		Rand_Num = rand() % COMBAT_STRUCT_LEN; // give a new num
		Combat_Length = strlen(combat_readiness[Rand_Num].arrow);
		Finished_Pointer = 0;
		Count_Fininshed_Combat += 1; 
		Game_Time += 0.2; // give more time
		Delay_us(500); // show last arrow is filled
	}
	
	if(Key_check(GPIOA, GPIO_Pin_2)){
	// press A
		if(combat_readiness[Rand_Num].arrow[Finished_Pointer] == 'A'){ // press right
			Finished_Pointer += 1; // next arrow
		}else{ // not right and back to first arrow
			Finished_Pointer = 0;
			All_Right = 0;
		}
	}
	if(Key_check(GPIOA, GPIO_Pin_3)){
	//press D
		if(combat_readiness[Rand_Num].arrow[Finished_Pointer] == 'D'){ // press right
			Finished_Pointer += 1; // next arrow
		}else{
			Finished_Pointer = 0;
			All_Right = 0;
		}
	}
	if(Key_check(GPIOA, GPIO_Pin_4)){
	//press W
		if(combat_readiness[Rand_Num].arrow[Finished_Pointer] == 'W'){ // press right
			Finished_Pointer += 1; // next arrow
		}else{
			Finished_Pointer = 0;
			All_Right = 0;
		}
	}
	if(Key_check(GPIOA, GPIO_Pin_5)){
	//press S
		if(combat_readiness[Rand_Num].arrow[Finished_Pointer] == 'S'){ // press right
			Finished_Pointer += 1; // next arrow
		}else{
			Finished_Pointer = 0;
			All_Right = 0;
		}
	}
	
	// show on OLED
	OLED_Clear(); // in order to show clean screen
	OLED_Printf(0, 0, OLED_6X8, combat_readiness[Rand_Num].name); // show the combat_readiness's name
	
	for(int i = 0; i < Finished_Pointer - 1; i++){ //show the right arrow
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
	
	// the for is not usable, should change 
	for(int i = 0; i <= 4; i++){
		switch(combat_readiness[Rand_Num].arrow[Finished_Pointer - 1]){
				case 'W':
					OLED_ShowImage((Finished_Pointer - 1) * 16, 30 - i, 16, 16, W_filled);
					break;
				case 'S':
					OLED_ShowImage((Finished_Pointer - 1) * 16, 30 - i, 16, 16, S_filled);
					break;
				case 'A':
					OLED_ShowImage((Finished_Pointer - 1) * 16, 30 - i, 16, 16, A_filled);
					break;
				case 'D':
					OLED_ShowImage((Finished_Pointer - 1)* 16, 30 - i, 16, 16, D_filled);
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
	// OLED_Printf(0, 50, OLED_6X8, "%d", Rand_Num);
	OLED_Printf(0, 50, OLED_6X8, "score: %0.1lf", Score);
	// OLED_Printf(40, 50, OLED_6X8, "%d", Combat_Length);
	// OLED_Printf(60, 50, OLED_6X8, "%d", Finished_Pointer);
	// OLED_Printf(40, 50, OLED_6X8, "%d", next_level_page_remain_time);
	Show_Time_Bar();
	
	// game over
	if(Game_Time < 0.0){ // have time to play
		Game_Ready = 0;
		
		OLED_Clear();
		TIM_Cmd(TIM2,DISABLE);
		
		Score_Page(Score);
		Game_Para_Init();

	}
	
	// next level
	if(Count_Fininshed_Combat >= Count_Next_Level_Combat + *level){
		
		OLED_Clear();
		
		TIM_Cmd(TIM2,DISABLE);
		
		double Game_Time_tmp = Game_Time;
		Game_Time = GAME_LASTING_TIME;
		Rand_Num = rand() % COMBAT_STRUCT_LEN;  // to init the next game
		Combat_Length = strlen(combat_readiness[Rand_Num].arrow);
		Finished_Pointer = 0;
		Count_Fininshed_Combat = 0;
		next_level_page(All_Right, Game_Time_tmp, level, &Score);
		// Delay_s(3);
	}
}


void Game_Page_Ready(void){
	// The game function
	one_level(&level);
	
}


void Game_Page_nReady(void){
	// The game confirm function 
	OLED_Clear();
	
	OLED_Printf(0, 0, OLED_8X16, "Game Ready?");
	OLED_Printf(0, 20, OLED_6X8, "Join the");
	OLED_Printf(0, 30, OLED_6X8, "Helldivers!");
	
	OLED_ShowString(0, 45, "Back", OLED_8X16);
	OLED_ReverseArea(0, 45, 32 + 1, 61);
	OLED_ShowString(85, 45, "Ready", OLED_8X16);
	OLED_ReverseArea(85 - 1, 45, 127, 61);
	
	if(Key_check(GPIOB, GPIO_Pin_7)){
		status = GAME_PAGE;
		Game_Ready = 1;
		Game_Para_Init();
		OLED_Clear();
	}
	
	if(Key_check(GPIOB, GPIO_Pin_6)){
		status = HOME_PAGE;
		Game_Ready = 0;
		Game_Para_Init();
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


uint8_t rank_cursor = 0; // use to select the choice, point at reverse(choose) data
uint8_t rolling_cursor = 0; // use to show the data, point at first show data

void Rank_Page(void){
	
	if((Key_check(GPIOA, GPIO_Pin_5) == 1)){
		
		if(rank_cursor < rank_array_len - 1){
			rank_cursor += 1;
			if(rank_cursor > rolling_cursor + 3)
			{
				rolling_cursor += 1;
			}
		}
		
		OLED_Clear();
	}
	
	if(Key_check(GPIOA, GPIO_Pin_4) == 1){

		if(rank_cursor > 0){
			rank_cursor -= 1;
			if(rank_cursor < rolling_cursor)
			{
				rolling_cursor -= 1;
			}
		}
		
		OLED_Clear();
	}
	OLED_Clear();
	
	OLED_ShowString(0, 0, "name", OLED_8X16);
	OLED_ShowString(60, 0, "Score", OLED_8X16);
	
	OLED_ShowString(0, 20, rank_struct[0 + rolling_cursor].name, OLED_6X8);
	OLED_Printf(60, 20, OLED_6X8, "%0.1lf", rank_struct[0 + rolling_cursor].score);
	OLED_ShowString(0, 30, rank_struct[1 + rolling_cursor].name, OLED_6X8);
	OLED_Printf(60, 30, OLED_6X8, "%0.1lf", rank_struct[1 + rolling_cursor].score);
	OLED_ShowString(0, 40, rank_struct[2 + rolling_cursor].name, OLED_6X8);
	OLED_Printf(60, 40, OLED_6X8, "%0.1lf", rank_struct[2 + rolling_cursor].score);
	OLED_ShowString(0, 50, rank_struct[3 + rolling_cursor].name, OLED_6X8);
	OLED_Printf(60, 50, OLED_6X8, "%0.1lf", rank_struct[3 + rolling_cursor].score);
	
	OLED_ReverseArea(0, 20 - 1 + (rank_cursor - rolling_cursor) * 10, 127, 10);
	
	if(Key_check(GPIOB, GPIO_Pin_6)){
		status = HOME_PAGE;
		rank_cursor = 0;
		rolling_cursor = 0;
		OLED_Clear();
	}
	
}


int main(void)
{
	Store_init();
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
			case RANK_PAGE:
				Rank_Page();
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
		Game_Time -= 1.0;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}


void TIM3_IRQHandler(void){
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET){
		next_level_page_remain_time -= 1;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
