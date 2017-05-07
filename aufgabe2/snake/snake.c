#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "support.h"

#define KEY_UP 119			//W
#define KEY_LEFT 97			//A
#define KEY_DOWN 115		//S
#define KEY_RIGHT 100		//D

//field filler
#define BLANK 0				//empty field
#define HEAD 1				//snake head symbol
#define TAIL 2				//snake tail symbol
#define FOOD 9				//food item symbol
//field parameter
#define WIDTH 60			//field size X
#define HEIGHT 20			//field size Y
//game parameter
#define MOMENTUM 1			//1 = keeps the player moving (0 = player stops when no key is pressed)
//etc
#define DEBUG 1				//for debugging if-conditions
//direction values
#define IDLE 0
#define NORTH 1				//north or up
#define EAST 2				//east  or right
#define SOUTH 3				//south or down
#define WEST 4				//west  or left

//play field
int field[WIDTH][HEIGHT];

//player head position and direction
struct position {
	int x;
	int y;
	int direction;
} pos;

//location of the tail end 
struct position tail_pos;

//first declaration of methods
void gameInit();
int getPlayerDirection();
void moveX(int step);
void moveY(int step);
void movePlayer();
void nextTailpiece();
void moveTail();
void createItem();
void gamePhysics();
void renderFrame();

int main(int argc, char const *argv[]){
	gameInit();
	int timeNow = (int)time(0);					//initial timestamp
	int timePre = timeNow - 1;						//go back 1 sec to avoid delay at start
	while(1){
		while(support_readkey(1) != 0); 			//empty the input buffer if key is helt down
		if (timeNow > timePre){						//if timeDelta is > 0 sec aka >= 1 sec bc integer value of time
			gamePhysics();							//calculate player motion, etc.
			renderFrame();
		}
		timePre = timeNow;							//timestamp reset
		timeNow = (int)time(0);						//	
	}
	return 0;
}
void gameInit(){
	printf("C Snake!\n");
	support_init();
	//player_init
	pos.x=WIDTH/2;
	pos.y=HEIGHT/2;
	pos.direction = IDLE;
}
int getPlayerDirection(){
	//system("/bin/stty raw");						//hacky way to avoid newline key to toggle getchar()
	int input = support_readkey(500);				//Read char from input, with max 500 ms delay	
	//system("/bin/stty cooked");					//disable hackysack
	char* direction_str;							//for debugging purposes
	int direction;									//return variable
	switch(input){
		case 0:
			direction_str = "nothing";
			direction = IDLE;
			break;
		case KEY_UP:
			direction_str = "up";
			direction = NORTH;
			break;
		case KEY_LEFT:
			direction_str = "left";
			direction = WEST;
			break;
		case KEY_DOWN:
			direction_str = "down";
			direction = SOUTH;
			break;
		case KEY_RIGHT:
			direction_str = "right";
			direction = EAST;
			break;
		default:
			direction_str = "wrong key";
			direction = IDLE;
			break;
	}
	if (DEBUG){printf("KEY: %s (%d)\n",direction_str,direction);}	//DEBUGGING
	return direction;	
}
void moveX(int step){
	if (step > 0){
		pos.x = (pos.x + 1) % WIDTH;
	}else{
		if (pos.x - 1 < 0){
			pos.x = HEIGHT;
		}else{
			pos.x--;
		}
	}
}
void moveY( int step){
	if (step > 0){
		pos.y = (pos.y + 1) % HEIGHT;
	}else{
		if (pos.y - 1 < 0){
			pos.y = HEIGHT;
		}else{
			pos.y--;
		}
	}
}
void movePlayer(){
	int player_direction = getPlayerDirection();			//return 0-4 (idle, N,E,S,W)
	if (!MOMENTUM || player_direction){						//if not idle (to maintain momentum)
		pos.direction = player_direction;
	}
	field[pos.x][pos.y] = TAIL;								//set current HEAD position to TAIL rendering	
	switch(pos.direction){
		case NORTH:
			moveY(-1);
			break;
		case EAST:
			moveX(1);
			break;
		case SOUTH:
			moveY(1);
			break;
		case WEST:
			moveX(-1);
			break;
		default:
			break;
	}
	field[pos.x][pos.y] = HEAD;	
	if (DEBUG){printf("Player position [%d][%d]\n",pos.x,pos.y);}		//DEBUGGING
}
void nextTailpiece(){
	
}
void moveTail(){
	
}
void createItem(){
	
}
void gamePhysics(){
	movePlayer();
	moveTail();
	createItem();
}
void renderFrame(){
	int x,y = 0;
	for (y = 0; y<HEIGHT; y++){
		for (x = 0; x<WIDTH; x++){
			
		}
	}
}
