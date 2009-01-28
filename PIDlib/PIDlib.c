//Justin Murray
//Dean Pantages
//PID control functions

#include <stdio.h>
#include <math.h>
#include <libplayerc/playerc.h>
#include "PIDlib.h"

#define NUMERR 10

typedef struct pid_data_struct {
	double Kp, Kd, Ki;
	double errorHist[NUMERR];
	int iErr;
	double tol;
	double maxI;
} pid_data;

double prevError(pid_data * data) {
	if(data->iErr == 0) {
		return data->errorHist[NUMERR - 1];
	} else {
		return data->errorHist[data->iErr - 1];
	}
}

double errorSum(pid_data * data) {
	double sum = 0;
	int i;
	for(i=0; i<NUMERR; i++) {
		sum += data->errorHist[i];
	}
	
	if(sum > maxI) {
		return maxI;
	} else {
		return sum;
	}
}

double PID(pid_data * data) {
	double pTerm, dTerm, iTerm;
	
	double error = data->errorHist[data->iErr];
	
	pTerm = data->Kp * error;
	
	dTerm = Kd * (error - prevError(data));
	
	iTerm = Ki * errorSum(data);
	
	
	return (pTerm + dTerm + iTerm);
}

double tranError(playerc_posiiton2d_t * pos2D, pid_data * data, double tX, double tY) {
	data->iErr = (data->iErr + 1) % NUMERR;
	
	return data->errorHist[data->iErr] = sqrt( (tX - pos2D->px)*(tX - pos2D->px) + (tY - pos2D->py)*(tY - pos2D->py) );
}

double rotError(playerc_posiiton2d_t * pos2D, pid_data * data, double tX, double tY) {
	double theta;
	data->iErr = (data->iErr + 1) % NUMERR;
	
	theta = arctan2(tX - pos2D->px, tY - pos2D->py);
	
	return data->errorHist[data->iErr] = pos2D->pa - theta;
}

int bumped(playerc_bumper_t * b) {
	return b->bumpers[0] || b->bumpers[1];
}

double Move(playerc_posiiton2d_t * pos2D, double meters) {
	double startpos, pos, error;
	pid_data tranData, rotData;
	
	//clear structs
	memset(&tranData, 0, sizeof(pid_data));
	memset(&rotData, 0, sizeof(pid_data));
	
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

