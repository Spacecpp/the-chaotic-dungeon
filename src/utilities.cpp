#include "utilities.h"

#include <iostream>
#include "stdio.h"
#include <time.h>
#include <math.h>

/*
int iRand(int min, int max)
{
	if (min >= max) return min;
	int i = rand()%(max - min + 1);
	return min + i;
}
*/



bool rectangle_to_rectangle_collision_topleft(double x1, double y1, double w1, double h1, double x2, double y2, double w2, double h2)
{
	
    if (x1 + w1 < x2) return false;
    if (x1 > x2 + w2) return false;
	if (y1 + h1 < y2) return false;
    if (y1 > y2 + h2) return false;
    return true;

}


bool rectangle_to_rectangle_collision(double x1, double y1, double w1, double h1, double x2, double y2, double w2, double h2)
{
	return rectangle_to_rectangle_collision_topleft(x1 - w1 * 0.5, y1 - h1 * 0.5, w1, h1, x2 - w2 * 0.5, y2 - h2 * 0.5, w2, h2);
}


bool point_to_rectangle_collision_topleft(double p_x, double p_y, double x, double y, double width, double height)
{
	
	if (p_x < x) return false;
    if (p_x > x + width) return false;
	if (p_y < y) return false;
    if (p_y > y + height) return false;   
    return true;

}


bool point_to_rectangle_collision(double p_x, double p_y, double x, double y, double width, double height)
{
	return point_to_rectangle_collision_topleft(p_x, p_y, x - width * 0.5, y - height * 0.5, width, height);
}


float angle_between_points(float x1, float y1, float x2, float y2)
{
	return radians_to_degrees( atan2(y2 - y1, x2 - x1) );
}

float angle_between_points_rad(float x1, float y1, float x2, float y2)
{
	return atan2(y2 - y1, x2 - x1);
}


float distance_between_points(float x1, float y1, float x2, float y2)
{
	return (float)sqrt( pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0) );
}

float distance_between_points_3D(float x1, float y1, float z1, float x2, float y2, float z2)
{
	return (float)sqrt( pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0) + pow(z1 - z2, 2.0) );
}
