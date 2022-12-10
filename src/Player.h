#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Item.h"
#include "lists.h"
#include <stdint.h>

class Player: public Character
{
	
	public:
		
		class inventoryInfo
		{
			public:
				int id;
				int amount;
				inventoryInfo(): id(0), amount(0) { }
				inventoryInfo(int id, int amount): id(id), amount(amount) { }
				void set(int id, int amount) { this->id = id; this->amount = amount; }
		};
		
		int maxHP;
		int mana, maxMana;
		int experience, maxExperience;
		int level;
		int pointsToDistribute;
		int manaRegenerationTimer;
		int strength, dexterity, vitality, magic;
		inventoryInfo inventory[MAX_INVENTORY_SLOTS];
		uint8_t selectedInventorySlot;
		int coins;
		int arrows;
		int armorId;
		//int explosionBuff;
		bool swordAnimation;
		int kills;
		
		Player();
		
		void init_base_values();
		void init_for_new_game();
		
		void update();
		void die();
		bool increase_attribute(int attribute);
		
		void earn_experience(int value);
		int get_free_inventory_slot_index();
		
		bool cannot_autopick_item(Item & item);
		bool pickup_item_if_possible(Item & item);
		void swap_items(Item & item);
		
		void consume_selected_item();
		
		void do_potion_effect(int item_id);
		
		void recover_mana(int amount);
		
		void draw();
		
		//
		
		int get_animation_delay();
		bool currently_walking();
		void do_primary_action();
		int get_max_HP();
		int get_AC();
		float get_movement_speed();
		int get_attack_strength();
		int get_projectile_damage();
		float get_magic_damage_multiplier();
		int get_mana_cost(int item_id);
		bool attack_is_ranged();
		int get_projectile_type();
		
		int get_selected_item_id();
		bool select_inventory_item(int slot);
		
		float get_selected_item_base_damage();
		
		int get_base_invulnerability_timer();
		int get_attack_delay();
		
		void play_death_sound();
		
		//void update();
	
};
#endif
