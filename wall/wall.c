#include <stdio.h>

#include <libplayerc/playerc.h>
#include "../PIDlib/PIDlib.h"

#define SERVER "hal" //"localhost"
#define PORT	9876 //12121

#define PI	3.14159265


int main(int argc, const char **argv)
{
	double x, speed;
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
	
	// Create a position2d proxy (device id "position2d:0") and susbscribe
	// in read/write mode
	position2d = playerc_position2d_create(client, 0);
	if (playerc_position2d_subscribe(position2d, PLAYERC_OPEN_MODE))
	{
		fprintf(stderr, "error: %s\n", playerc_error_str());
		return -1;
	}
	
	bumper = playerc_bumper_create(client, 0);
	if(playerc_bumper_subscribe(bumper, PLAYERC_OPEN_MODE)) {
		fprintf(stderr, "error: %s\n", playerc_error_str());
		return -1;
	}
	
	// Enable the robots motors
	playerc_position2d_enable(position2d, 1);
	
	// Start the robot moving
	x = 0;
	speed = 10;
	playerc_position2d_set_cmd_vel(position2d, speed, 0, 0, 1);
	finished = 0;
	while( !finished ) {
		// Read data from the server and display current robot position
		playerc_client_read(client);
		printf("position : %f %f %f  bumpers: %d %d\n", position2d->px, position2d->py, position2d->pa, bumper->bumpers[0], bumper->bumpers[1]);
		if(bumped(bumper)){
			finished = 1;
			//stop the robot
			playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
		} else {
			//keep going
		}
	}
	
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

