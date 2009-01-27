//Justin Murray
//PID control functions

#include <stdio.h>
#include <libplayerc/playerc.h>
#include "PIDlib.h"

#define Kp 3.5
#define Kd 1
#define Ki 1
#define I_MAX 50
#define TOL 0.001

#define NUMERR 10
double * errors = NULL;
int Eindex = 0;

int bumped(playerc_bumper_t * b) {
	return b->bumpers[0] || b->bumpers[1];
}

double Move(playerc_posiiton2d_t *device, double meters) {
	double startpos, pos, error;
	
	startpos = device->px;
	
	do {
		pos = device->px - startpos;
		error = meters - pos;
		playerc_position2d_set_cmd_vel(position2d, PID(error), 0, 0, 1);
	} while(error > TOL);
	
	playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
	return 
}

double Turn(playerc_posiiton2d_t *device, double radians) {
	
}

double PID(double error) {
	double pTerm, dTerm, iTerm;
	
	if(errors == NULL) {
		errors = malloc(NUMERR * sizeof(double));
		memset(errors, 0, 10 * sizeof(double));
		Eindex = 0;
	}
	
	errors[Eindex] = error;
	
	pTerm = Kp * error;
	
	dTerm = Kd * (error - prevError());
	
	iTerm = Ki * errorSum();
	
	Eindex++;
	if(Eindex >= NUMERR) {
		Eindex = 0;
	}
	
	return (pTerm + dTerm + iTerm);
}

double prevError() {
	if(Eindex == 0) {
		return errors[NUMERR - 1];
	} else {
		return errors[Eindex - 1];
	}
}

double errorSum() {
	double sum = 0;
	int i;
	for(i=0; i<NUMERR; i++) {
		sum += errors[i];
	}
	
	if(sum > I_MAX) {
		return I_MAX;
	} else {
		return sum;
	}
}

