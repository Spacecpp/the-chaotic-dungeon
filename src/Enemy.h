#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include <stdint.h>

class Enemy: public Character
{
	
	public:
		//bool awake;
		uint8_t state;
		int id;
		int AIDelay;
		//int8_t animationFrame;
		//int8_t animationTime;
		int screamTimer;
		
		//Enemy();
		Enemy(float x, float y, int id);
		
		void draw();
		void update();
		void AI_logic_update();
		
		void take_damage(int damage, bool ignore_armor = false);
		void die();
		
		bool is_enemy();
		
		void wake_up();
		int get_max_HP();
		float get_movement_speed();
		int get_attack_strength();
		int get_projectile_damage();
		bool attack_is_ranged();
		int get_ranged_attack_probability();
		int get_projectile_type();
		int get_base_attack_delay();
		bool can_walk_through_walls();
		void play_hurt_sound();
		void play_death_sound();
		
		int get_experience();
		bool spawns_awake();
		float get_detection_range();
		int get_loot();
		int get_loot_probability();
		
		bool has_ranged_attack();
		Texture2D * get_sprite();
		
		int get_attack_delay();
		float get_radius();
		
	
};
#endif
