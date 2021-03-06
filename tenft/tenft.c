#include <stdio.h>

#include <libplayerc/playerc.h>

int main(int argc, const char **argv)
{
  double dist, speed;
  playerc_client_t *client;
  playerc_position2d_t *position2d;

  // Create a client object and connect to the server; the server must
  // be running on "localhost" at port 6665
  client = playerc_client_create(NULL, "gort", 9876);
  if (playerc_client_connect(client) != 0)
    {
      fprintf(stderr, "error: %s\n", playerc_error_str());
      return -1;
    }

  // Create a position2d proxy (device id "position2d:0") and susbscribe
  // in read/write mode
  position2d = playerc_position2d_create(client, 0);
  if (playerc_position2d_subscribe(position2d, PLAYERC_OPEN_MODE) != 0)
    {
      fprintf(stderr, "error: %s\n", playerc_error_str());
      return -1;
    }

  // Enable the robots motors
  playerc_position2d_enable(position2d, 1);

  // Start the robot moving
  dist = 0;
  speed = 1;
  playerc_position2d_set_cmd_vel(position2d, speed, 0, 0, 1);
  while( dist <= 3.048 ) {
      // Read data from the server and display current robot position
      playerc_client_read(client);
      printf("position : %f %f %f\n", position2d->px, position2d->py, position2d->pa);
      dist = position2d->px;
      if(dist > 2.6 && speed > .01) {
	      playerc_position2d_set_cmd_vel(position2d, (speed /= 2), 0, 0, 1);
      }
  }
  
  //stop the robot
  playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);

  // Shutdown and tidy up
  playerc_position2d_unsubscribe(position2d);
  playerc_position2d_destroy(position2d);
  playerc_client_disconnect(client);
  playerc_client_destroy(client);

  return 0;
}

