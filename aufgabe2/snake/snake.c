#include <stdio.h>
#include <stdlib.h>
#include <support.h>
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
