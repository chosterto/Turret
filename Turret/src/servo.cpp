#include <iostream>

#include "servo.h"


uint8_t current_pulse;
ServoCR* servo_p;

void enc1_callback(void)
{
	if ( (servo_p->pulse) > (servo_p->neutral_pulse) )
	{
		(servo_p->enc_ticks)++;
	}
	else if ( (servo_p->pulse) < (servo_p->neutral_pulse) )
	{
		(servo_p->enc_ticks)--;
	}
}


void computePID(double error, double* output, double dt, PID* pid) {
	double s1, s2, s3;

	s1 = error;

	pid->sum += error * dt;
	s2 = (pid->sum);

	s3 = (error - pid->prev) / dt;

	*output = (pid->K_p)*s1 + (pid->K_i)*s2 + (pid->K_d)*s3;
	pid->prev = error;
}


void servoInitialize(ServoCR* servo1, ServoCR* servo2, uint8_t nPulse1, uint8_t nPulse2)
{
	servo1->pin = SERVO_1;
	servo2->pin = SERVO_2;
	servo1->neutral_pulse = nPulse1;
	servo2->neutral_pulse = nPulse2;
	pinMode(SERVO_1, PWM_OUTPUT);
	pinMode(SERVO_2, PWM_OUTPUT);
	pinMode(ENCODER, INPUT);

	servo1->offset = (nPulse1 - ZERO_PULSE);
	servo2->offset = (nPulse2 - ZERO_PULSE);
	servo1->pulse = 0;
	servo2->pulse = 0;

	servo1->enc_ticks = 0;
	servo2->enc_ticks = 0;

	servo_p = servo1;

	if (wiringPiISR(ENCODER, INT_EDGE_BOTH, &enc1_callback) == -1)
	{
		std::cout << "ISR failed\n";
	}
	// pwmFrequency in Hz = 19.2e6 Hz / pwmClock / pwmRange
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(192);
	pwmSetRange(2000); // 20 ms / 2000 units = 0.01 ms/unit
}


void setPower(ServoCR* servo, double pOut)
{
	// clamp percent power
	if (pOut > 1.0)  pOut = 1.0;
	if (pOut < -1.0) pOut = -1.0;

	// convert from power to pulse
	current_pulse = (uint8_t)( (MAX_PULSE*(1 + pOut) + MIN_PULSE*(1 - pOut)) / 2.0);
	// correct pulse by using an offset
	current_pulse += (servo->offset);

	pwmWrite(servo->pin, current_pulse);
	servo->pulse = current_pulse;
}


void setPWMPulse(ServoCR* servo, uint8_t pulse_)
{
	// set raw pwm to servo
	servo->pulse = pulse_;
	pwmWrite(servo->pin, pulse_);
}

