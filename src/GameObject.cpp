#include "GameObject.h"
#include "lists.h"
#include "utilities.h"
#include "math.h"

GameObject::GameObject(): active(true), angle(0.0f)
{
	
	position.x = 0;
	position.y = 0;
	oldPosition = position;
	velocity.x = 0; velocity.y = 0;
	
}

GameObject::GameObject(float x, float y): active(true), angle(0)
{
	
	position.x = x;
	position.y = y;
	oldPosition = position;
	velocity.x = 0; velocity.y = 0;
	
}


GameObject::GameObject(float x, float y, float angle): active(true), angle(angle)
{
	
	position.x = x;
	position.y = y;
	oldPosition = position;
	velocity.x = 0; velocity.y = 0;
	
}



void GameObject::destroy()
{
	active = false;
}


void GameObject::update()
{
	
	oldPosition = position;
	position.x += velocity.x; position.y += velocity.y;
	
	//reset_velocity();
	
}


void GameObject::accelerate(float x, float y)
{
	velocity.x += x; velocity.y += y;
}


void GameObject::accelerate_angular(float angle, float distance)
{
	
	const float rad_angle = degrees_to_radians(angle);
	velocity.x += cos(rad_angle) * distance; 
	velocity.y += sin(rad_angle) * distance;
	
}


void GameObject::move(float x, float y)
{
	
	accelerate(x, y);
	angle = radians_to_degrees( atan2(y, x) );
	
	/*
	oldPosition = position;
	
	position.x += x;
	position.y += y;
	*/
}


void GameObject::set_position(float x, float y)
{
	
	position.x = x;
	position.y = y;
	oldPosition = position;
	velocity.x = 0.0f; velocity.y = 0.0f;
	
}


void GameObject::set_position_at_tile(int x, int y)
{
	set_position(x * TW + TW * 0.5f, y * TW + TW * 0.5f);
}


void GameObject::reset_velocity()
{
	velocity.x = 0.0f; velocity.y = 0.0f;
}


bool GameObject::moved_this_frame()
{
	return (position.x != oldPosition.x || position.y != oldPosition.y);
}



float GameObject::get_radius()
{
	return 8.0f;
}



bool GameObject::point_collision(float x, float y)
{
	
	const float diameter = get_radius() * 2.0f;
	return point_to_rectangle_collision(x, y, position.x, position.y, diameter, diameter);
	
}



bool GameObject::collision(GameObject & other)
{
	
	const float this_diameter = this->get_radius() * 2.0f;
	const float other_diameter = other.get_radius() * 2.0f;
	
	return rectangle_to_rectangle_collision(this->position.x, this->position.y, this_diameter, this_diameter, 
											other.position.x, other.position.y, other_diameter, other_diameter);
	
}



float GameObject::distance_to(GameObject & other)
{
	return distance_between_points(this->position.x, this->position.y, other.position.x, other.position.y);
}

float GameObject::distance_to(const Vector2 & other)
{
	return distance_between_points(this->position.x, this->position.y, other.x, other.y);
}


float GameObject::angle_to(GameObject & other)
{
	return angle_between_points(this->position.x, this->position.y, other.position.x, other.position.y);
}


float GameObject::angle_to(const Vector2 & other)
{
	return angle_between_points(this->position.x, this->position.y, other.x, other.y);
}

float GameObject::angle_to_rad(GameObject & other)
{
	return angle_between_points_rad(this->position.x, this->position.y, other.position.x, other.position.y);
}
