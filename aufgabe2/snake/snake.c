#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "support.h"

#define KEY_UP 119		//W
#define KEY_LEFT 97		//A
#define KEY_DOWN 115	//S
#define KEY_RIGHT 100	//D

//field filler
#define BLANK 0			//empty field
#define HEAD 1			//snake head symbol
#define TAIL 2			//snake tail symbol
#define FOOD 9			//food item symbol
//field parameter
#define WIDTH 60		//field size X
#define HEIGHT 20		//field size Y
//etc
#define DEBUG 0			//for debugging if-conditions
//direction values
#define N 0				//north or up
#define E 1				//east  or right
#define S 2				//south or down
#define W 3				//west  or left

int field[WIDTH][HEIGHT];

struct position {
	int x;
	int y;
} pos;
struct tail_position {
	int x;
	int y;
	int direction;
} tail_pos;

void movePlayer();
void createItem();
void calculateField();
void printScreen();
void wait(unsigned int seconds);

int main(int argc, char const *argv[]){
        printf("Hello World!");
        while(1){
			movePlayer();
			//wait(1);
		}
        return 0;
}

void movePlayer(){
	
	system("/bin/stty raw");
	int input = getchar();  //support_readkey(1000);
	fprintf(stdout, "\033[2J");
	fprintf(stdout, "\033[1;1H");
	printf("%d",input);
	system("/bin/stty cooked");
}

void printScreen(){
	int x,y = 0;
	for (y = 0; y<HEIGHT; y++){
		for (x = 0; x<WIDTH; x++){
			
		}
	}
}
void wait(unsigned int seconds){
	unsigned int finished = time(0) + seconds;
	while (time(0) < finished); //wait
}
