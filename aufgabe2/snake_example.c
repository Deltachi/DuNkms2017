/*
	Snake, Console Game

	move the snake with w, a, s, d
	terminate with x

	Linux-System required!

	@author: www.c-howto.de
*/

#include<stdio.h>
#include<termio.h>
#define BLANK 0
#define HEAD 1
#define TAIL 2
#define FOOD 9
#define KEY_MOVE_UP 119
#define KEY_MOVE_LEFT 97
#define KEY_MOVE_DOWN 115
#define KEY_MOVE_RIGHT 100
#define WIDTH 60
#define HEIGHT 20
#define DEBUG 0

struct position {
	int x;
	int y;
} pos;

int field[WIDTH][HEIGHT];

static struct termio savemodes;
static int havemodes = 0;
int tty_break();
int tty_getchar();
int tty_fix();

void resetField();
void printField();
void setItem(int, int, int);
void move(int);

int main () {
	int key;
	resetField();

	pos.x=WIDTH/2;
	pos.y=HEIGHT/2;
	setItem(pos.x, pos.y, HEAD);
	tty_break();

	while(key!='x') {
		printField();
		key=tty_getchar();
		move(key);
		if(DEBUG) {
			printf(" %d %c |", key);
		}
	}

	printf("\n\n");
	return 0;
}

void move(int key) {

	setItem(pos.x, pos.y, TAIL);
	switch(key) {
		case KEY_MOVE_UP: pos.y--; break;
		case KEY_MOVE_LEFT: pos.x--; break;
		case KEY_MOVE_DOWN: pos.y++; break;
		case KEY_MOVE_RIGHT: pos.x++; break;
	}

	if(pos.y >= HEIGHT) pos.y=0;
	if(pos.y < 0) pos.y=HEIGHT-1;
	if(pos.x >= WIDTH) pos.x=0;
	if(pos.x < 0) pos.x=WIDTH-1;

	setItem(pos.x, pos.y, HEAD);
}

void setItem(int x, int y, int item) {
	field[x][y] = item;
}

void resetField() {
	int i,j;
	for(i=0;i<HEIGHT;i++) {
		for(j=0;j<WIDTH;j++) {
			field[i][j] = BLANK;
		}
	}
}

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

	// print debug things
	if(DEBUG) {
		printf("\n\nX: %3d   Y: %3d", pos.x, pos.y);
		printf("\n\ni: %3d   j: %3d", i, j);
	}
}

int tty_break() {
	struct termio modmodes;
	if(ioctl(fileno(stdin), TCGETA, &savemodes) < 0)
		return -1;
	havemodes = 1;
	modmodes = savemodes;
	modmodes.c_lflag &= ~ICANON;
	modmodes.c_cc[VMIN] = 1;
	modmodes.c_cc[VTIME] = 0;
	return ioctl(fileno(stdin), TCSETAW, &modmodes);
}

int tty_getchar() {
	return getchar();
}

int tty_fix() {
	if(!havemodes)
		return 0;
	return ioctl(fileno(stdin), TCSETAW, &savemodes);
}
