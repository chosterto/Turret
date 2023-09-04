#include <iostream>
#include <termios.h>
#include <wiringPi.h>
#include "servo.h"

#define KEY_Q 113
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100


static struct termios old, current;

/* Initialize new terminal i/o settings */
void initTermios(void)
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  current.c_lflag &= ~ECHO; /* set no echo mode */
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &old);
}


int main(void)
{
	if (wiringPiSetup() == -1)
		exit(1);

	int ch;
	initTermios();

	ServoCR servo_1;
	ServoCR servo_2;
	servoInitialize(&servo_1, &servo_2, 150, 153);

	std::cout << "Use WASD keys to control the turret. Type Q to quit.\n";
	std::cout << "W - rotate shaft upward\n";
	std::cout << "S - rotate shaft downward\n";
	std::cout << "A - rotate base left\n";
	std::cout << "D - rotate base right\n";
	while (1)
	{
		std::cout << (int)servo_1.enc_ticks << std::endl;
		ch = (int)getchar();
		if (ch == KEY_Q)
		{
			setPower(&servo_2, 0.0);
			setPower(&servo_1, 0.0);
			break;
		}

		switch(ch)
		{
			case KEY_W:
				setPower(&servo_2, -0.5);
				break;
			case KEY_S:
				setPower(&servo_2, 0.5);
				break;
			case KEY_A:
				setPower(&servo_1, 0.75);
				break;
			case KEY_D:
				setPower(&servo_1, -0.75);
				break;
		}

		delay(35);
		setPower(&servo_1, 0.0);
		setPower(&servo_2, 0.0);
	}

	std::cout << "Exited \n";
	resetTermios();
	return 0;
}

