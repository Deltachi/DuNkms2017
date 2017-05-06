#include <stdio.h>
#include <stdlib.h>
#include "support.h"

#define KEY_UP 119		//W
#define KEY_LEFT 97		//A
#define KEY_DOWN 115	//S
#define KEY_RIGHT 100	//D

#define BLANK 0			//empty field
#define HEAD 1			//snake head symbol
#define TAIL 2			//snake tail symbol
#define FOOD 9			//food item symbol

#define WIDTH 60		//plane size X
#define HEIGHT 20		//plane size Y
#define DEBUG 0			//for debugging if-conditions

int field[WIDTH][HEIGHT];

struct position {
	int x;
	int y;
} pos;

void printScreen(){
	int x,y = 0;
	for (y = 0; y<HEIGHT; y++){
		for (x = 0; x<WIDTH; x++){
			
		}
	}
}


int main(int argc, char const *argv[]){
        printf("Hello World!");
        return 0;
}
