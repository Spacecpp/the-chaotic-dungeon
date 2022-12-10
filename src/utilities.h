#ifndef UTILITIES_H
#define UTILITIES_H

#include <math.h>

//int iRand(int min, int max);

bool rectangle_to_rectangle_collision_topleft(double x1, double y1, double w1, double h1, double x2, double y2, double w2, double h2);
bool rectangle_to_rectangle_collision(double x1, double y1, double w1, double h1, double x2, double y2, double w2, double h2);

bool point_to_rectangle_collision_topleft(double p_x, double p_y, double x, double y, double width, double height);
bool point_to_rectangle_collision(double p_x, double p_y, double x, double y, double width, double height);

float angle_between_points(float x1, float y1, float x2, float y2);
float angle_between_points_rad(float x1, float y1, float x2, float y2);

float distance_between_points(float x1, float y1, float x2, float y2);
float distance_between_points_3D(float x1, float y1, float z1, float x2, float y2, float z2);



// inline utilities
#ifndef PI
#define PI 3.14159265359
#endif

inline double degrees_to_radians(double degrees)
{
	return (degrees * (PI / 180.0) );
}

inline float degrees_to_radians(float degrees)
{
	return (degrees * (PI / 180.0f) );
}

inline double radians_to_degrees(double radians)
{
	return (radians * (180.0 / PI) );
}

inline float radians_to_degrees(float radians)
{
	return (radians * (180.0f / PI) );
}


inline double round_down_(double n)
{
	return floor(n);
}

inline double round_up_(double n)
{
	return ceil(n);
}

inline double round_to_nearest_(double n)
{	
	return floor(n + 0.5);
}

#endif
