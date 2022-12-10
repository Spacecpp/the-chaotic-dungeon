#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "raylib.h"

class GameObject
{
	
	private:
	
	public:
		bool active;
		Vector2 position;
		Vector2 oldPosition;
		Vector2 velocity;
		float angle;
		
		GameObject();
		GameObject(float x, float y);
		GameObject(float x, float y, float angle);
		virtual ~GameObject() = default;
		
		void destroy();
		virtual void update();
		void accelerate(float x, float y);
		void accelerate_angular(float angle, float distance);
		void move(float x, float y);
		void set_position(float x, float y);
		void set_position_at_tile(int x, int y);
		void reset_velocity();
		bool moved_this_frame();
		
		virtual float get_radius();
		
		bool point_collision(float x, float y);
		bool collision(GameObject & other);
		
		float distance_to(GameObject & other);
		float distance_to(const Vector2 & other);
		float angle_to(GameObject & other);
		float angle_to(const Vector2 & other);
		float angle_to_rad(GameObject & other);

};
#endif
