#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>
#include <wiringPi.h>

#define SERVO_1   1
#define SERVO_2   24
#define ENCODER   2
#define MIN_PULSE 100
#define MAX_PULSE 200

#define ZERO_PULSE 150

#define TICKS_PER_REV (24 * 3)


typedef struct ServoCR {
	uint8_t pin;
	uint8_t neutral_pulse;
	uint8_t offset;
	uint8_t pulse;
	int16_t enc_ticks;

} ServoCR;

typedef struct PID {
	double K_p,
	       K_i,
	       K_d,
	       prev = 0.0,
	       sum = 0.0;
} PID;


// PID feedback controller
void computePID(double error, double* output, double dt, PID* pid, double thres);

// encoder callback function
void enc1_callback(void);

// percent output, from -1.0 to 1.0
void setPower(ServoCR* servo, double pOut);


void setPWMPulse(ServoCR* servo, uint8_t pulse_);

// servo1 is for base, servo2 is shaft
void servoInitialize(ServoCR* servo1, ServoCR* servo2, uint8_t nPulse1, uint8_t nPulse2);

#endif
