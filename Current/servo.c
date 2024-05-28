#include <LPC21xx.H>
#include "servo.h"
#include "led.h"
#include "timer_interrupts.h"

#define DETECTOR_bm (1 << 10)

enum DetectorState {ACTIVE, INACTIVE};

enum ServoState {CALLIB, IDLE, IN_PROGRESS};
struct Servo {
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
};

struct Servo sServo;

void DetectorInit(void) {
	IO0DIR = IO0DIR & (~DETECTOR_bm);
}

enum DetectorState eReadDetector(void) {
	if((IO0PIN & DETECTOR_bm) == 0) {
		return ACTIVE;
	}
	else {
		return INACTIVE;
	}
}

void Automat(void) {
	switch(sServo.eState){
		case CALLIB:
			if(eReadDetector() == ACTIVE) {
				sServo.uiDesiredPosition = 0;
				sServo.uiCurrentPosition = 0;
				sServo.eState = IDLE;
			}
			else {
				LedStepRight();
				sServo.eState = CALLIB;
			}
			break;
		case IDLE:
			if(sServo.uiCurrentPosition == sServo.uiDesiredPosition) {
				sServo.eState = IDLE;
			}
			else {
				sServo.eState = IN_PROGRESS;
			}
			break;
		case IN_PROGRESS:
			if(sServo.uiCurrentPosition == sServo.uiDesiredPosition) {
				sServo.eState = IDLE;
			}
			else {
				if((int)(sServo.uiCurrentPosition - sServo.uiDesiredPosition) > 0) {
					LedStepRight();
					sServo.uiCurrentPosition--;
				}
				else {
					LedStepLeft();
					sServo.uiCurrentPosition++;
				}
				sServo.eState = IN_PROGRESS;
			}
			break;
	}
}

void ServoInit(unsigned int uiServoFrequency) {
	DetectorInit();
	LedInit();
	sServo.eState = CALLIB;
	Timer0Interrupts_Init((1000000 / uiServoFrequency), Automat);
}

void ServoCallib(void) {
	sServo.eState = CALLIB;
}

void ServoGoTo(unsigned int uiPosition) {
	sServo.uiDesiredPosition = uiPosition;
}
