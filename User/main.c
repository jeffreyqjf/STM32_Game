#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "GPIO_init.h"
#include "OLED.h"
#include "Key.h"
#include "Timer.h"
#include "main.h"
#include "Store.h"
#include "ADC_Random.h"

#include <stdlib.h>
#include <string.h>

#define HOME_PAGE  ((uint16_t)1)
#define MENU_PAGE  ((uint16_t)2)
#define GAME_PAGE  ((uint16_t)3)
#define RANK_PAGE  ((uint16_t)4)

#define COMBAT_STRUCT_LEN ((int)51)
#define GAME_LASTING_TIME ((double)15.0)
#define LEVEL_PAGE_REMAIN_TIME ((int)5)
#define NAME_LEN ((uint8_t)32)

typedef struct {
	char name[32];
	char arrow[16];
}Combat_Readiness_Struct;
Combat_Readiness_Struct combat_readiness[60];

// next time refactor can change 16 to define
Rank_Struct rank_struct[16];
uint16_t rank_array_len = 0;

uint16_t status = HOME_PAGE;


void Data_init(void){
	// read flash data
	Store_read_rank_struct();
	// arrow use w, s, a, d 
	strcpy(combat_readiness[0].name, "Eagle 500kg Bomb");strcpy(combat_readiness[0].arrow, "WDSSS\0");
	strcpy(combat_readiness[1].name, "MBR/GPT");strcpy(combat_readiness[1].arrow, "DSAWWW\0");
	strcpy(combat_readiness[2].name, "ATX");strcpy(combat_readiness[2].arrow, "DDD\0");
	strcpy(combat_readiness[3].name, "OLED");strcpy(combat_readiness[3].arrow, "DDSADS\0");
	strcpy(combat_readiness[4].name, "nvcc");strcpy(combat_readiness[4].arrow, "DSWWASS\0");
	strcpy(combat_readiness[5].name, "TPM2.0");strcpy(combat_readiness[5].arrow, "DSDSDS\0");
	strcpy(combat_readiness[6].name, "SAS");strcpy(combat_readiness[6].arrow, "DSWDS\0");
	strcpy(combat_readiness[7].name, "VT-x");strcpy(combat_readiness[7].arrow, "DDSADW\0");
	strcpy(combat_readiness[8].name, "BGA");strcpy(combat_readiness[8].arrow, "DWSSD\0");
	strcpy(combat_readiness[9].name, "Chisel");strcpy(combat_readiness[9].arrow, "WDD\0");
	strcpy(combat_readiness[10].name, "LPCAMM2");strcpy(combat_readiness[10].arrow, "WDSD\0");
	strcpy(combat_readiness[11].name, "x86");strcpy(combat_readiness[11].arrow, "WDSSD\0");
	strcpy(combat_readiness[12].name, "UEFI");strcpy(combat_readiness[12].arrow, "WDSA\0");
	
	strcpy(combat_readiness[13].name, "BIOS");strcpy(combat_readiness[13].arrow, "DDSW\0");
	strcpy(combat_readiness[14].name, "PCIe");strcpy(combat_readiness[14].arrow, "WDDSS\0");
	strcpy(combat_readiness[15].name, "SATA");strcpy(combat_readiness[15].arrow, "DSAWW\0");
	strcpy(combat_readiness[16].name, "NVMe");strcpy(combat_readiness[16].arrow, "WSDD\0");
	strcpy(combat_readiness[17].name, "DDR4");strcpy(combat_readiness[17].arrow, "SDDW\0");
	strcpy(combat_readiness[18].name, "CUDA");strcpy(combat_readiness[18].arrow, "WASD\0");
	strcpy(combat_readiness[19].name, "FPGA");strcpy(combat_readiness[19].arrow, "DDSA\0");
	strcpy(combat_readiness[20].name, "RISC-V");strcpy(combat_readiness[20].arrow, "DWSDD\0");
	strcpy(combat_readiness[21].name, "ARM");strcpy(combat_readiness[21].arrow, "WDD\0");
	strcpy(combat_readiness[22].name, "SPI");strcpy(combat_readiness[22].arrow, "SWS\0");
	strcpy(combat_readiness[23].name, "I2C");strcpy(combat_readiness[23].arrow, "DSD\0");
	strcpy(combat_readiness[24].name, "UART");strcpy(combat_readiness[24].arrow, "WSSD\0");
	strcpy(combat_readiness[25].name, "GPU");strcpy(combat_readiness[25].arrow, "WDA\0");
	strcpy(combat_readiness[26].name, "TPU");strcpy(combat_readiness[26].arrow, "ASDW\0");
	strcpy(combat_readiness[27].name, "RTOS");strcpy(combat_readiness[27].arrow, "DDSSS\0");
	strcpy(combat_readiness[28].name, "NVCC");strcpy(combat_readiness[28].arrow, "WSSDD\0");
	strcpy(combat_readiness[29].name, "RAID");strcpy(combat_readiness[29].arrow, "SDDA\0");
	strcpy(combat_readiness[30].name, "ECC");strcpy(combat_readiness[30].arrow, "DDS\0");
	strcpy(combat_readiness[31].name, "SIMD");strcpy(combat_readiness[31].arrow, "WDSDW\0");
	
	strcpy(combat_readiness[32].name, "Linux Kernel");strcpy(combat_readiness[32].arrow, "WDSAWW\0");
	strcpy(combat_readiness[33].name, "Docker");strcpy(combat_readiness[33].arrow, "DDSAWWS\0");
	strcpy(combat_readiness[34].name, "Kubernetes");strcpy(combat_readiness[34].arrow, "WASDD\0");
	strcpy(combat_readiness[35].name, "OpenGL");strcpy(combat_readiness[35].arrow, "DWASDWS\0");
	strcpy(combat_readiness[36].name, "Vulkan");strcpy(combat_readiness[36].arrow, "SDAWWD\0");
	strcpy(combat_readiness[37].name, "TensorFlow");strcpy(combat_readiness[37].arrow, "WDSAWWD\0");
	strcpy(combat_readiness[38].name, "PyTorch");strcpy(combat_readiness[38].arrow, "DDWDDS\0");
	strcpy(combat_readiness[39].name, "LLVM");strcpy(combat_readiness[39].arrow, "SDAWDSS\0");
	strcpy(combat_readiness[40].name, "GCC");strcpy(combat_readiness[40].arrow, "WDDWD\0");
	strcpy(combat_readiness[41].name, "Clang");strcpy(combat_readiness[41].arrow, "DDAWWDS\0");
	strcpy(combat_readiness[42].name, "Rust");strcpy(combat_readiness[42].arrow, "WDDSWW\0");
	strcpy(combat_readiness[43].name, "Python");strcpy(combat_readiness[43].arrow, "DASDD\0");
	strcpy(combat_readiness[44].name, "Node.js");strcpy(combat_readiness[44].arrow, "WDDS\0");
	strcpy(combat_readiness[45].name, "Electron");strcpy(combat_readiness[45].arrow, "WADSAW\0");
	strcpy(combat_readiness[46].name, "Android");strcpy(combat_readiness[46].arrow, "AWWDD\0");
	strcpy(combat_readiness[47].name, "iOS SDK");strcpy(combat_readiness[47].arrow, "WAWDDS\0");
	strcpy(combat_readiness[48].name, "Windows NT");strcpy(combat_readiness[48].arrow, "WDDSA\0");
	strcpy(combat_readiness[49].name, "macOS");strcpy(combat_readiness[49].arrow, "WASDAW\0");
	strcpy(combat_readiness[50].name, "Ubuntu");strcpy(combat_readiness[50].arrow, "DAWWDS\0");
	strcpy(combat_readiness[51].name, "Arch Linux");strcpy(combat_readiness[51].arrow, "WDSAW\0");
}


void Animation_Play(void){
	// play an animation
	/*
	OLED_Clear();
	uint16_t frame_len = 0;
	for(int i = 0; i <= frame_len; i++){
		
		
		Delay_ms(16); // 1000 / 60
	}
	*/
	OLED_Clear();
	OLED_Printf(13, 0, OLED_8X16, "    Look");
	OLED_Printf(30, 30, OLED_8X16, "familiar?");
	OLED_Update();
	Delay_s(2);
	OLED_Clear();
	OLED_Printf(10, 10, OLED_8X16, "You!");
	OLED_Update();
	Delay_ms(800);
	OLED_Printf(64, 10, OLED_8X16, "Could!");
	OLED_Update();
	Delay_ms(800);
	OLED_Printf(10, 40, OLED_8X16, "Be!");
	OLED_Update();
	Delay_ms(800);
	OLED_Printf(64, 40, OLED_8X16, "Next!");
	OLED_Update();
	Delay_ms(1200);

	OLED_Clear();
	
	
}


void Home_Page(void){
	
	Animation_Play();
	OLED_Clear();
	OLED_Printf(30, 0, OLED_8X16, "Join the ");
	OLED_Printf(48, 30, OLED_8X16, "EVA!");
	

	OLED_ShowString(0, 45, "Rank", OLED_8X16);
	OLED_ReverseArea(0, 45, 32 + 1, 61);
	OLED_ShowString(95, 45, "Game", OLED_8X16);
	OLED_ReverseArea(95 - 1, 45, 127, 61);
	OLED_Update();
	while(1){
		if(Key_check(GPIOB, GPIO_Pin_7)){
			status = GAME_PAGE;
			OLED_Clear();
			break;
		}
		
		if(Key_check(GPIOB, GPIO_Pin_6)){
			status = RANK_PAGE;
			OLED_Clear();
			break;
		}
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
	
	OLED_Printf(0, 0, OLED_6X8, "YOU RANK:%d", Rank + 1);
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
		Game_Time += 0.5; // give more time
		Delay_ms(200); // show last arrow is filled
	}
	
	if(Key_check(GPIOA, GPIO_Pin_2)){
	// press A
		if(combat_readiness[Rand_Num].arrow[Finished_Pointer] == 'A'){ // press right
			Finished_Pointer += 1; // next arrow
		}else{ // not right and back to first arrow
			OLED_ShowImage(Finished_Pointer * 16, 30, 16, 16, A_wrong);
			OLED_Update();
			Delay_ms(200);
			Finished_Pointer = 0;
			All_Right = 0;
			
		}
	}
	if(Key_check(GPIOA, GPIO_Pin_3)){
	//press D
		if(combat_readiness[Rand_Num].arrow[Finished_Pointer] == 'D'){ // press right
			Finished_Pointer += 1; // next arrow
		}else{
			OLED_ShowImage(Finished_Pointer * 16, 30, 16, 16, D_wrong);
			OLED_Update();
			Delay_ms(200);
			Finished_Pointer = 0;
			All_Right = 0;
		}
	}
	if(Key_check(GPIOA, GPIO_Pin_4)){
	//press W
		if(combat_readiness[Rand_Num].arrow[Finished_Pointer] == 'W'){ // press right
			Finished_Pointer += 1; // next arrow
		}else{
			OLED_ShowImage(Finished_Pointer * 16, 30, 16, 16, W_wrong);
			OLED_Update();
			Delay_ms(200);
			Finished_Pointer = 0;
			All_Right = 0;
		}
	}
	if(Key_check(GPIOA, GPIO_Pin_5)){
	//press S
		if(combat_readiness[Rand_Num].arrow[Finished_Pointer] == 'S'){ // press right
			Finished_Pointer += 1; // next arrow
		}else{
			OLED_ShowImage(Finished_Pointer * 16, 30, 16, 16, S_wrong);
			OLED_Update();
			Delay_ms(200);
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
	OLED_Printf(0, 30, OLED_6X8, "EVA!");
	
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
	ADC_Random_Init();
	
	
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
