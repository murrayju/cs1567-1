//Justin Murray
//PIDlib header

#include <libplayerc/playerc.h>

#define PI 3.141592654


int bumped(playerc_bumper_t *);

double Move(playerc_client_t * client, playerc_position2d_t * pos2D, playerc_bumper_t * bumper, double X, double Y);
