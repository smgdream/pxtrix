/* No buffer get char functions */

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include "ctl.h"


int kbhit(void)
{ 
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF) {
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}

int getch(void)
{
    struct termios tm, tm_old;
    int ch;
 
    if (tcgetattr(0, &tm) < 0) {
          return EOF;
    }
 
    tm_old = tm;
    cfmakeraw(&tm);
    if (tcsetattr(0, TCSANOW, &tm) < 0) {
        return EOF;
    }
 
    ch = getchar();
    if (tcsetattr(0, TCSANOW, &tm_old) < 0) {
        return EOF;
    }
    
    return ch;
}