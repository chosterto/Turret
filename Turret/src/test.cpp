#include <iostream>
#include "servo.h"
#include <wiringPi.h>

int main(void)
{
	if (wiringPiSetup() == -1)
		exit(1);

	ServoCR test_servo;
	ServoCR test_servo2;
	servoInitialize(&test_servo, &test_servo2, 150, 150);

	// servo testing
	setEncPos(&test_servo, 48);
	//setPower(&test_servo, 0.25);
	//while (1)
	//{
	//	std::cout << (int)test_servo.enc_ticks << std::endl;
	//	delay(10);
	//}

	//setPower(&test_servo, 0.5);
	//delay(2000);

	//setPower(&test_servo, 0.2);

	//delay(2000);

	//setPower(&test_servo, 0.0);

	//delay(2000);

	//setPower(&test_servo, -1.0);

	//delay(2000);

	//setPower(&test_servo, 0.0);

	return 0;
}

