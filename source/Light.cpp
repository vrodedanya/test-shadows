#include "Light.h"

void Light::change_position(const int& new_x, const int& new_y)
{
	x = new_x;
	y = new_y;
}

void Light::change_radius(const int& new_radius)
{
	radius = new_radius;
}
