#include "Ball.h"

vector<Ball *> Ball::balls;
Timer Ball::t;
float Ball::G = 98.1f;
float Ball::floorLevel = -33.f;
float Ball::bumpiness = 0.9f;