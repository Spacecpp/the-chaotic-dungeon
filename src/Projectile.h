#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObject.h"
#include "Character.h"

class Projectile: public GameObject
{
	
	private:
	
	public:
		bool enemy;
		int id;
		int damage;
		int piercesRemain;
		
		
		Projectile(float x, float y, float angle, int id, bool enemy, int damage);
		
		void update();
		void draw();
		
		float get_speed();
		Texture2D * get_sprite();
		
		void do_damage(Character & target);
		void die();
		
		float get_radius();
		bool is_piercing();

};
#endif
