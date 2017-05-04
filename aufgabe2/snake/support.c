#include "support.h"

#ifdef _WIN32

#include <conio.h>

void support_init() {
	// not needed
}

void support_clear() {
	system("CLS");
}

int support_readkey(int timeout_ms) {
	Sleep(timeout_ms);
	if (!kbhit()) return 0;
	return getch();
}

#else

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

void support_init() {
	struct termios tio;
	tcgetattr(STDIN_FILENO, &tio);
	tio.c_lflag &= (~ICANON & ~ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &tio);
}

void support_clear() {
	printf("\x1B[2J\x1B[0;0f");
}

int support_readkey(int timeout_ms) {
	struct timeval tv = { 0L, timeout_ms * 1000L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	int r = select(1, &fds, NULL, NULL, &tv);
	if (!r) return 0;

	return getchar();
}

#endif

