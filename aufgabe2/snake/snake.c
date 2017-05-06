#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
#include <support.h>
=======
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

void printScreen(){
	int x,y = 0;
	for (y = 0; y<HEIGHT; y++){
		for (x = 0; x<WIDTH; x++){
			
		}
	}
}
>>>>>>> delta

#define BLANK 0
#define HEAD 1
#define TAIL 2
#define FOOD 9
#define WIDTH 60
#define HEIGHT 20

struct position {
  int x;
  int y;
} pos;

int field[WIDTH][HEIGHT];

void printField();

int main(int argc, char const *argv[]){
<<<<<<< HEAD
    pos.x=WIDTH/2;
    pos.y=HEIGHT/2;

    printField();
};


void printField() {
	//clear screen
	fprintf(stdout, "\033[2J");
	fprintf(stdout, "\033[1;1H");

	int i,j,x;
	for(i=0;i<HEIGHT+2;i++) {
		printf("\n");
		printf("|");
		for(j=0;j<WIDTH;j++) {
			if(i==0 || i==HEIGHT+1)  {
				printf("-");
			}
			else {
				x = field[j][i];
				switch(x) {
				case BLANK: printf(" "); break;
				case TAIL: printf("*"); break;
				case HEAD: printf("O"); break;
				}
			}
		}
		printf("|");
  }
=======
        printf("Hello World!");
        return 0;
}
>>>>>>> delta
