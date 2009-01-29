#include <stdio.h>

#include <libplayerc/playerc.h>
#include "../PIDlib/PIDlib.h"

#define SERVER "rosie" //"localhost"
#define PORT	9876 //12121


int main(int argc, const char **argv)
{
	double  actual_result;
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
	
	//calls our move function
	actual_result = Move(client,position2d,bumper,3.2,0.0);
	printf("Results Returned from Move: %f\n",actual_result);
	
	playerc_client_read(client);
	printf("Stopped in position : %f %f %f  bumpers: %d %d\n", position2d->px, position2d->py, position2d->pa, bumper->bumpers[0], bumper->bumpers[1]);
	
	
	// Shutdown and tidy up
	playerc_position2d_unsubscribe(position2d);
	playerc_position2d_destroy(position2d);
	playerc_bumper_unsubscribe(bumper);
	playerc_bumper_destroy(bumper);
	playerc_client_disconnect(client);
	playerc_client_destroy(client);
	
	return 0;
}

