#include <stdio.h>

#include <libplayerc/playerc.h>
#include "../PIDlib/PIDlib.h"

#define SERVER "gort" //"localhost"
#define PORT	9876 //12121

void printPos(playerc_client_t * client, playerc_position2d_t * pos2D, playerc_bumper_t * bumper) {
	playerc_client_read(client);
	printf("\n\nStopped in position : %f %f %f  bumpers: %d %d\n\n", pos2D->px, pos2D->py, pos2D->pa, bumper->bumpers[0], bumper->bumpers[1]);
}

int main(int argc, const char **argv)
{
	double  actual_result_trans;
	double  actual_result_angle;
	int finished;
	playerc_client_t *client;
	playerc_position2d_t *position2d;
	playerc_bumper_t * bumper;
	
	// Create a client object and connect to the server; the server must
	// be running on "localhost" at port 6665
	client = playerc_client_create(NULL, SERVER, PORT);
	if (playerc_client_connect(client) != 0)
	{
		fprintf(stderr, "error: %s\n", playerc_error_str());
		return -1;
	}
	printf("Connected...");
	// Create a position2d proxy (device id "position2d:0") and susbscribe
	// in read/write mode
	position2d = playerc_position2d_create(client, 0);
	if (playerc_position2d_subscribe(position2d, PLAYERC_OPEN_MODE))
	{
		fprintf(stderr, "error: %s\n", playerc_error_str());
		return -1;
	}
	
	printf("Position2D Subscribed...");
	
	bumper = playerc_bumper_create(client, 0);
	if(playerc_bumper_subscribe(bumper, PLAYERC_OPEN_MODE)) {
		fprintf(stderr, "error: %s\n", playerc_error_str());
		return -1;
	}
	
	printf("Bumper Subscribed...");
	
	// Enable the robots motors
	playerc_position2d_enable(position2d, 1);
	
	printf("Motor Enabled\n");
	
	/*
	Turn(client,position2d,bumper,PI/2.0);
	printPos(client,position2d,bumper);
	Turn(client,position2d,bumper,3.0*PI/2.0);
	printPos(client,position2d,bumper);
	Turn(client,position2d,bumper,PI);
	printPos(client,position2d,bumper);
	Turn(client,position2d,bumper,-3.0*PI/2.0);
	printPos(client,position2d,bumper);
	Turn(client,position2d,bumper,-PI/2.0);
	printPos(client,position2d,bumper);
	Turn(client,position2d,bumper,-PI/4.0);
	printPos(client,position2d,bumper);
	Turn(client,position2d,bumper,0);
	printPos(client,position2d,bumper);
	Turn(client,position2d,bumper,-0.9*PI);
	printPos(client,position2d,bumper);
	Turn(client,position2d,bumper,0.9*PI);
	printPos(client,position2d,bumper);
	Turn(client,position2d,bumper,0.0);
	printPos(client,position2d,bumper);
	
	*/
	
#ifdef ABSOLUTE_COORD
	//calls our move function to move to second point
	actual_result_trans = Move(client,position2d,bumper,3.2,0.0);
	printf("Results Returned from Move: %f\n",actual_result_trans);
	printPos(client,position2d,bumper);
	
	//rotates robot into position for third point
	actual_result_angle = Turn(client,position2d,bumper,(PI/2.0));
	printf("Results Returned from TurnL %f\n",actual_result_angle);
	printPos(client,position2d,bumper);
	
	//moves to third point from second point
	actual_result_trans = Move(client,position2d,bumper,3.2,3.04);
	printf("Results Returned from Move: %f\n",actual_result_trans);
	printPos(client,position2d,bumper);
	
	//rotates robot into position for the fourth point
	actual_result_angle = Turn(client,position2d,bumper,2.75741633);
	printf("Results Returned from TurnL %f\n",actual_result_angle);
	printPos(client,position2d,bumper);
	
	//moves to fouth point from third point
	actual_result_trans = Move(client,position2d,bumper,-0.5,4.7);
	printf("Results Returned from Move: %f\n",actual_result_trans);
	printPos(client,position2d,bumper);
	
	//rotates robot into position for point five
	actual_result_angle = Turn(client,position2d,bumper,(PI/2.0));
	printf("Results Returned from TurnL %f\n",actual_result_angle);
	printPos(client,position2d,bumper);
	
	//moves robot from position four to position five
	actual_result_trans = Move(client,position2d,bumper,-0.55,11.6);
	printf("Results Returned from Move: %f\n",actual_result_trans);
	printPos(client,position2d,bumper);
#else
	//calls our move function to move to second point
	actual_result_trans = Move(client,position2d,bumper,3.2,0.0);
	printf("Results Returned from Move: %f\n",actual_result_trans);
	printPos(client,position2d,bumper);
	
	//rotates robot into position for third point
	actual_result_angle = Turn(client,position2d,bumper,(PI/2.0));
	printf("Results Returned from TurnL %f\n",actual_result_angle);
	printPos(client,position2d,bumper);
	
	//moves to third point from second point
	actual_result_trans = Move(client,position2d,bumper,3.04,0.0);
	printf("Results Returned from Move: %f\n",actual_result_trans);
	printPos(client,position2d,bumper);
	
	//rotates robot into position for the fourth point
	actual_result_angle = Turn(client,position2d,bumper,1.18662);
	printf("Results Returned from TurnL %f\n",actual_result_angle);
	printPos(client,position2d,bumper);
	
	//moves to fouth point from third point
	actual_result_trans = Move(client,position2d,bumper,4.02,0.0);
	printf("Results Returned from Move: %f\n",actual_result_trans);
	printPos(client,position2d,bumper);
	
	//rotates robot into position for point five
	actual_result_angle = Turn(client,position2d,bumper,-1.18662);
	printf("Results Returned from TurnL %f\n",actual_result_angle);
	printPos(client,position2d,bumper);
	
	//moves robot from position four to position five
	actual_result_trans = Move(client,position2d,bumper,6.83,0.0);
	printf("Results Returned from Move: %f\n",actual_result_trans);
	printPos(client,position2d,bumper);
#endif
	
	// Shutdown and tidy up
	playerc_position2d_unsubscribe(position2d);
	playerc_position2d_destroy(position2d);
	playerc_bumper_unsubscribe(bumper);
	playerc_bumper_destroy(bumper);
	playerc_client_disconnect(client);
	playerc_client_destroy(client);
	
	return 0;
}

