#include <stdio.h>
#include "../PIDlib/PIDlib.h"
#include <libplayerc/playerc.h>

int main(int argc, const char **argv)
{
  int i;
  double actual_result;
  playerc_client_t *client;
  playerc_position2d_t *position2d;
  playerc_bumper_t *bumper;

  // Create a client object and connect to the server; the server must
  // be running on "localhost" at port 6665
  client = playerc_client_create(NULL, "localhost", 6665);
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

   //create a bumper proxy (device id "bumper:0") and subscribe in read mode
   bumper = playerc_bumper_create (bumper,0);
   if(playerc_bumper_subscribe(bumper, PLAYERC_OPEN_MODE) != 0)
   {
   	 fprint(stderr, "error: %s\n", playerc_error_str());
   	 return -1
   }

  // Enable the robots motors
  playerc_position2d_enable(position2d, 1);

  //calls our move function
	actual_result = Move(client,position2d,bumper,5.0,0.0);
	printf("Results Returned from Move: %f",actual_result);


  // Shutdown and tidy up
  playerc_position2d_unsubscribe(position2d);
  playerc_position2d_destroy(position2d);
  playerc_client_disconnect(client);
  playerc_client_destroy(client);

  return 0;
}

