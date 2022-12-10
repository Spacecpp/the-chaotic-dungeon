#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameObject.h"
#include <stdint.h>

class Character: public GameObject
{
	
	public:
		int8_t animationFrame;
		int8_t animationTime;
		int HP;
		int attackDelay;
		int hurtTimer;
		int invulnerabilityTimer;
		int regenerationTimer;
		int slowedTimer;
		bool inputLeft, inputRight, inputUp, inputDown, inputAttack, inputSecondary, inputClimb;
		
		Character();
		Character(float x, float y);
		
		virtual void update();
		void clear_inputs();
		virtual void take_damage(int damage, bool ignore_armor = false);
		virtual void die();
		void heal(int amount);
		
		void attack(Character & other);
		
		virtual void wake_up();
		virtual int get_animation_delay();
		virtual bool currently_walking();
		virtual void do_primary_action();
		virtual bool is_enemy();
		virtual int get_max_HP();
		virtual int get_AC(); // armor class
		virtual float get_movement_speed();
		virtual int get_attack_strength();
		virtual int get_projectile_damage();
		virtual bool attack_is_ranged();
		virtual int get_projectile_type();
		virtual int get_base_attack_delay();
		bool can_be_hurt();
		virtual bool can_walk_through_walls();
		virtual void play_hurt_sound();
		virtual void play_death_sound();
		virtual int get_base_invulnerability_timer();
		virtual int get_attack_delay();
	
};
#endif
