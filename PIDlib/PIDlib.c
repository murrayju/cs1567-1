//Justin Murray
//Dean Pantages
//PID control functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	double Xi, Yi, Ai;
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
	
	if(sum > data->maxI) {
		return data->maxI;
	} else {
		return sum;
	}
}

double PID(pid_data * data) {
	double pTerm, dTerm, iTerm;
	
	double error = data->errorHist[data->iErr];
	
	pTerm = data->Kp * error;
	
	dTerm = data->Kd * (error - prevError(data));
	
	iTerm = data->Ki * errorSum(data);
	
	printf("PID P=%f D=%f I=%f total=%f\n",pTerm, dTerm, iTerm, (pTerm+dTerm+iTerm));
	
	return (pTerm + dTerm + iTerm);
}

double tranError(playerc_position2d_t * pos2D, pid_data * data, double tX, double tY) {
	double relX = pos2D->px - data->Xi;
	double relY = pos2D->py - data->Yi;
	data->iErr = (data->iErr + 1) % NUMERR;
	
	return data->errorHist[data->iErr] = sqrt( (tX - relX)*(tX - relX) + (tY - relY)*(tY - relY) );
}

double targetRotError(playerc_position2d_t * pos2D, pid_data * data, double tX, double tY) {
	double relX = pos2D->px - data->Xi;
	double relY = pos2D->py - data->Yi;
	double relA = pos2D->pa - data->Ai;
	double theta;
	data->iErr = (data->iErr + 1) % NUMERR;
	
	theta = atan2(tY - relY, tX - relX);
	
	printf("TRE: relX=%f relY=%f relA=%f theta=%f err=%f\n",relX,relY,relA,theta,theta-relA);
	
	return data->errorHist[data->iErr] = theta - relA;
}

double rotError(playerc_position2d_t * pos2D, pid_data * data, double tA) {
	double relA = pos2D->pa - data->Ai;
	
	data->iErr = (data->iErr + 1) % NUMERR;
	
	return data->errorHist[data->iErr] = tA - relA;
}

int bumped(playerc_bumper_t * b) {
	return b->bumpers[0] || b->bumpers[1];
}

double Move(playerc_client_t * client, playerc_position2d_t * pos2D, playerc_bumper_t * bumper, double X, double Y) {
	double tError, rError, vX, vA;
	pid_data tranData, rotData;
	
	//clear structs
	memset(&tranData, 0, sizeof(pid_data));
	memset(&rotData, 0, sizeof(pid_data));
	
	//Set parameters
	tranData.Kp = 2;
	tranData.Kd = 0;
	tranData.Ki = 0;
	tranData.tol = 0.01;
	tranData.maxI = 10;
	
	rotData.Kp = 4;
	rotData.Kd = 0;
	rotData.Ki = 0;
	rotData.tol = 0.01;
	rotData.maxI = 10;
	
	playerc_client_read(client);
	
	//Store initial position info
	tranData.Xi = pos2D->px;
	tranData.Yi = pos2D->py;
	tranData.Ai = pos2D->pa;
	
	rotData.Xi = pos2D->px;
	rotData.Yi = pos2D->py;
	rotData.Ai = pos2D->pa;
	
	while(!bumped(bumper) && (tError = tranError(pos2D, &tranData, X, Y)) > tranData.tol) {
		rError = targetRotError(pos2D,&rotData, X, Y);
		if(rError > PI/2.0 || rError < -PI/2.0) {
			//We passed it up, error should be negative
			tError = -tError;
			tranData.errorHist[tranData.iErr] = tError;
			if(rError < 0) {
				rError += PI;
			} else {
				rError -= PI;
			}
			rotData.errorHist[rotData.iErr] = rError;
			printf("Backwards! T: %f  R: %f\n",tError,rError);
		}
		vX = PID(&tranData);
		vA = PID(&rotData);
		playerc_position2d_set_cmd_vel(pos2D, vX, 0, vA, 1); //set new speeds
		playerc_client_read(client); //update position from sensors
		
		printf("VX: %f  VA: %f  Terror: %f  Rerror: %f  position : %f %f %f  bumpers: %d %d\n", vX, vA, tError, rError, pos2D->px, pos2D->py, pos2D->pa, bumper->bumpers[0], bumper->bumpers[1]);
	} 
	
	playerc_position2d_set_cmd_vel(pos2D, 0, 0, 0, 1); //STOP!
	playerc_client_read(client); //update position from sensors
	return tranError(pos2D, &tranData, X, Y);
}

double Turn(playerc_position2d_t *device, double A) {
	double rError, vA;
	pid_data rotData;
	
	//clear struct
	memset(&rotData, 0, sizeof(pid_data));
	
	//Set parameters
	rotData.Kp = 4;
	rotData.Kd = 0;
	rotData.Ki = 0;
	rotData.tol = 0.01;
	rotData.maxI = 10;
	
	playerc_client_read(client);
	
	//Store initial position info
	rotData.Xi = pos2D->px;
	rotData.Yi = pos2D->py;
	rotData.Ai = pos2D->pa;
	
	while(!bumped(bumper) && (rError = targetRotError(pos2D,&rotData, X, Y)) > rotData.tol) {
		vA = PID(&rotData);
		playerc_position2d_set_cmd_vel(pos2D, 0, 0, vA, 1); //set new speeds
		playerc_client_read(client); //update position from sensors
		
		printf("VA: %f  Rerror: %f  position : %f %f %f  bumpers: %d %d\n", vA, rError, pos2D->px, pos2D->py, pos2D->pa, bumper->bumpers[0], bumper->bumpers[1]);
	} 
	
	playerc_position2d_set_cmd_vel(pos2D, 0, 0, 0, 1); //STOP!
	playerc_client_read(client); //update position from sensors
	return tranError(pos2D, &tranData, X, Y);
}

