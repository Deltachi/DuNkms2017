#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "support.h"

#define KEY_UP 119			//W
#define KEY_LEFT 97			//A
#define KEY_DOWN 115		//S
#define KEY_RIGHT 100		//D
#define KEY_QUIT 113		//Q
#define KEY_SPACE 32		//SPACE KEY

//item list
#define BLANK 0				//empty field
#define HEAD 1				//snake head symbol
#define TAIL 2				//snake tail symbol
#define FOOD 3				//food item symbol
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
} head_pos;

//location of the tail end 
struct position tail_pos;

//first declaration of methods
void gameInit();									//libraries and player coordinates 
int getPlayerDirection();							//converts player input to value (0-4)
void moveX(struct position* p_pos, int step);		//calculates new coordinates for given struct (boundary-safe)(horizontally)
void moveY(struct position* p_pos, int step);		//same but different but still same! (vertically)
void movePlayer(struct position* p_pos);			//parent function that coordinates getPlayerDirection, moveX, moveY
void nextTailpiece(struct position* t_pos);		//searches surrounding tiles for a tail-object
void moveTail(struct position* t_pos);			//handler to shorten the tail (normal movement) or keeping the position (food intake)
void setTile(int x, int y, int item);				//sets a tile of the field to a given value (HEAD,TAIL,FOOD,EMPTY)
void generateFood();
void gamePhysics();									//handler for all methods mentioned above
void renderFrame();									//prints the game

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
	head_pos.x=WIDTH/2;
	head_pos.y=HEIGHT/2;
	head_pos.direction = IDLE;
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
	if (DEBUG){printf("KEY:[%d] %s (%d)\n",input,direction_str,direction);}	//DEBUGGING
	return direction;	
}
void moveX(struct position* p_pos, int step){
	if (step > 0){
		if (p_pos->x + 1 > WIDTH-2){
			p_pos->x = 1;
		}else{
			p_pos->x++;
		}
	}else{
		if (p_pos->x - 1 < 1){
			p_pos->x = WIDTH-2;
		}else{
			p_pos->x--;
		}
	}
}
void moveY(struct position* p_pos, int step){
	if (step > 0){
		if (p_pos->y + 1 > HEIGHT-2){
			p_pos->y = 1;
		}else{
			p_pos->y++;
		}
	}else{
		if (p_pos->y - 1 < 1){
			p_pos->y = HEIGHT-2;
		}else{
			p_pos->y--;
		}
	}
}
void movePlayer(struct position* p_pos){					
	int player_direction = getPlayerDirection();			//return 0-4 (idle, N,E,S,W)
	if (!MOMENTUM || player_direction){						//if not idle (to maintain momentum)
		p_pos->direction = player_direction;
	}
	setTile(p_pos->x,p_pos->y,TAIL);							//set current HEAD position to TAIL rendering	
	switch(p_pos->direction){
		case NORTH:
			moveY(p_pos,-1);
			break;
		case EAST:
			moveX(p_pos,1);
			break;
		case SOUTH:
			moveY(p_pos,1);
			break;
		case WEST:
			moveX(p_pos,-1);
			break;
		default:
			break;
	}
	setTile(p_pos->x,p_pos->y,HEAD);	
	if (DEBUG){printf("Player position [%d][%d]\n",p_pos->x,p_pos->y);}		//DEBUGGING
}
void nextTailpiece(struct position* t_pos){
	
}
void moveTail(struct position* t_pos){
	
}
void setTile(int x, int y, int item){
	field[x][y] = item;
}
void generateFood(){
	
}
void gamePhysics(){
	struct position* h_pos = &head_pos;
	struct position* t_pos = &tail_pos;
	movePlayer(h_pos);
	moveTail(t_pos);
	generateFood();
}
void renderFrame(){
	int x,y = 0;
	for (y = 0; y<HEIGHT; y++){
		printf("H");
		for (x=0; x<WIDTH; x++) {
			if(y==0 || y==HEIGHT-1){
				printf("W");
			}else{
				if (field[x][y] == BLANK){
					printf(" ");
				}else{
					printf("%d",field[x][y]);
				}
			}
		}
		printf("H\n");
	}
}
