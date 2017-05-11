#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "support.h"

#define KEY_UP 119			//W
#define KEY_LEFT 97			//A
#define KEY_DOWN 115		//S
#define KEY_RIGHT 100		//D
#define KEY_QUIT 113		//Q
#define KEY_SPACE 32		//SPACE KEY

//item list
#define BLANK 0				//empty field
#define EMPTY 0				//empty field
#define HEAD 1				//snake head symbol
#define TAIL 2				//snake tail symbol
#define FOOD 3				//food item symbol
//field parameter
#define WIDTH 60			//field size X
#define HEIGHT 20			//field size Y
//game parameter
#define MOMENTUM 1			//1 = keeps the player moving (0 = player stops when no key is pressed)
#define FOOD_GEN_RATE 5		//1-100 1(low rate) | 100 (high rate -> 100% per frame)
#define FPS 5				//frames or physics - steps per second
//etc
#define DEBUG 0				//for debugging if-conditions
#define ONE_SECOND 1000000
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

int score;
int collision_detected;

//first declaration of methods
void gameInit();									//libraries and player coordinates 
void setTile(int x, int y, int item);				//sets a tile of the field to a given value (HEAD,TAIL,FOOD,EMPTY)
int getTile(struct position* p_pos);				//get the active tile from a position struct with set x and y coordinates
int getPlayerDirection();							//converts player input to value (0-4)
void moveX(struct position* p_pos, int step);		//calculates new coordinates for given struct (boundary-safe)(horizontally)
void moveY(struct position* p_pos, int step);		//same but different but still same! (vertically)
void movePlayer(struct position* p_pos);			//parent function that coordinates getPlayerDirection, moveX, moveY
void nextTailpiece(struct position* t_pos);		//searches surrounding tiles for a tail-object
void moveTail(struct position* t_pos);			//handler to shorten the tail (normal movement) or keeping the position (food intake)
void generateFood();
void gamePhysics();									//handler for all methods mentioned above
void renderFrame();									//prints the game

int main(int argc, char const *argv[]){
	while(1){
		gameInit();
		//int timeNow = (int)time(0);					//initial timestamp
		//int timePre = timeNow - 1;						//go back 1 sec to avoid delay at start
		struct timeval start, end;
		gettimeofday(&start,NULL);
		gettimeofday(&end,NULL);
		while(collision_detected != TAIL){
			long unsigned int delta_us = (end.tv_sec - start.tv_sec)*1000000+(end.tv_usec - start.tv_usec);
			
			//if (timeNow > timePre + 1){				//if timeDelta is > 0 sec aka >= 1 sec bc integer value of time
			if (delta_us > ONE_SECOND/FPS){				//if timeDelta is > milliseconds
				gamePhysics();							//calculate player motion, etc.
				renderFrame();							//renders frame
				gettimeofday(&start,NULL);	
				//timePre = timeNow;						
			}
			gettimeofday(&end,NULL);
			//timeNow = (int)time(0);						//			
		}
		support_clear();
		printf("\n=====================\n|||   GAME OVER   |||\n=====================\n\nYOUR FINAL SCORE => ![%d]! \n",score);			
		printf("\n\n\n...press Q to quit, SPACEBAR to restart\n");
		int answer;
		do{
			system("/bin/stty raw");						//hacky way to avoid newline key to toggle getchar()
			answer = getchar();
			system("/bin/stty cooked");						//hacky way to avoid newline key to toggle getchar()	
		}while (answer != KEY_QUIT && answer != KEY_SPACE);
		if (answer == KEY_QUIT){
			break;
		}
	}
	return 0;
}
void gameInit(){
	support_clear();
	printf("Welcome to Snake Reloaded!\n\n\t\t[W]\nUse\t[A]\t[S]\t[D]\t to move.\n\n\nPress [return] to start!\n");
	getchar();
	//initialize important functions
	support_init();
	srand(time(NULL));
	//set everything to 0
	score = 0;
	collision_detected = BLANK;
	int i,j;
	for (i = 0; i < WIDTH; i++){
		for (j = 0; j < HEIGHT; j++){
			setTile(i,j,BLANK);
		}	
	}
	//player_init
	setTile(WIDTH/2,HEIGHT/2,HEAD);
	head_pos.x=WIDTH/2;
	head_pos.y=HEIGHT/2;
	head_pos.direction = NORTH;
	setTile(WIDTH/2,HEIGHT/2 + 1,TAIL);
	setTile(WIDTH/2,HEIGHT/2 + 2,TAIL);
	tail_pos.x = WIDTH/2;
	tail_pos.y = HEIGHT/2 + 2;
}
void setTile(int x, int y, int item){
	field[x][y] = item;
}
int getTile(struct position* p_pos){
	return field[p_pos->x][p_pos->y];
}
int getPlayerDirection(){
	int last_key;
	int current_key = 0;
	do{
		last_key = current_key;
		current_key = support_readkey(1);
	}while(current_key != 0); 					//empty the input buffer if key is helt down and save the last key that is not 0
	
	int input;
	//system("/bin/stty raw");						//hacky way to avoid newline key to toggle getchar()
	//input = support_readkey(250);				//Read char from input, with max 500 ms delay	
	//system("/bin/stty cooked");					//disable hackysack
	input = last_key;
	
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
		case KEY_QUIT:
			direction_str = "quit";
			direction = -1;
			break;
		default:
			direction_str = "wrong key";
			direction = IDLE;
			break;
	}
	if (DEBUG>=2){printf("KEY:[%d] %s (%d)\n",input,direction_str,direction);}	//DEBUGGING
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
	
	//if player hit 'q' to quit the game
	if (player_direction == -1){
		collision_detected = TAIL; //forces game over screen
	}else{
		if (!MOMENTUM || player_direction){						//if not idle (to maintain momentum)
			
			//allow eighter same direction as before or turn left/right, not 180deg turns -> NORTH + SOUTH and EAST + WEST are even numbers
			if (player_direction == p_pos->direction || (player_direction + p_pos->direction)%2 == 1){
				p_pos->direction = player_direction;
			}	
		}
		setTile(p_pos->x,p_pos->y,TAIL);						//set current HEAD position to TAIL rendering	
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

		collision_detected = field[p_pos->x][p_pos->y];			//safe tile that is about to be overwritten as collision object
		if (collision_detected == FOOD){score++;}				//score updater if HEAD hit FOOD
		
		setTile(p_pos->x,p_pos->y,HEAD);	
		if (DEBUG){
			printf("Player position [%d][%d]\n",p_pos->x,p_pos->y);
			printf("Collision detector: %d\n",collision_detected);
		}		//DEBUGGING
		
	}
}
void nextTailpiece(struct position* t_pos){
	struct position temp;
	temp.x = t_pos->x;
	temp.y = t_pos->y;
	
	moveX(&temp,1);
	if (getTile(&temp) == TAIL){t_pos->x = temp.x;}
	else{
		moveX(&temp,-1);
		moveX(&temp,-1);
		if (getTile(&temp) == TAIL){t_pos->x = temp.x;}
		else{
			moveX(&temp,1);
			moveY(&temp,1);
			if (getTile(&temp) == TAIL){t_pos->y = temp.y;}
			else{
				moveY(&temp,-1);
				moveY(&temp,-1);
				if (getTile(&temp) == TAIL){t_pos->y = temp.y;}
			}
		}
	}
}
void moveTail(struct position* t_pos){
	if (collision_detected == BLANK){
		setTile(t_pos->x,t_pos->y,BLANK);
		nextTailpiece(t_pos);
	}
}
void generateFood(){
	int t = rand() % ((100 / FOOD_GEN_RATE) + 1 - 1) + 1;

	if(t == 1){
		while(1){
			int x = rand() % (59 + 1 - 2) + 2; //random number for x in tile
			int y = rand() % (19 + 1 - 2) + 2; //random number for y in tile

			if (field[x][y] == BLANK) {
				setTile(x,y,FOOD);								//set FOOD item symbol if field is BLANK
				break;
			}
		}
	}
}
void gamePhysics(){
	struct position* h_pos = &head_pos;
	struct position* t_pos = &tail_pos;
	movePlayer(h_pos);
	moveTail(t_pos);
	generateFood();
}
void renderFrame(){
	support_clear();
	printf("Score: %d\n\n",score);
	int x,y = 0;
	for (y = 0; y<HEIGHT; y++){	
		for (x=0; x<WIDTH; x++) {
			if(x==0 || x==WIDTH-1){
				printf("H");
				if(x==WIDTH-1){printf("\n");}
			}else{
				if(y==0 || y==HEIGHT-1){
					printf("W");
				}else{
					switch(field[x][y]){
					case BLANK:
						printf(" ");
						break;
					case HEAD:
						printf("Ω");
						break;
					case TAIL:
						printf("¥");
						break;
					case FOOD:
						printf("€");
						break;
					}
				}

			}
		}
	}
}
