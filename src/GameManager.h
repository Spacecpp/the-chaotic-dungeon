#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <vector>

#include "GameMap.h"
#include "GameObject.h"
#include "Character.h"
#include "Player.h"
#include "Projectile.h"
#include "DamageMarker.h"
#include "Item.h"
#include "Enemy.h"
#include "Dungeon.h"
#include "Particle.h"


class GameManager
{
	
	private:
	
	public:
		Camera2D camera;
		//GameMap gameMap;
		Player player;
		Item item;
		//Enemy enemy;
		std::vector<Projectile> projectile;
		std::vector<DamageMarker> damageMarker;
		std::vector<Particle> particle;
		
		//int currentFloorIndex;
		Dungeon dungeon;
		
		uint8_t gameState;
		
		float levelUpMenuPosition;
		int levelUpMenuCursorPosition;
		
		int escapeTimer;
		float redTint;
		bool redTintDirection;
		
		
		// debug
		bool noclip;
		
		
		GameManager();
		
		void UpdateDrawFrame();
		void shutdown();
		
		void start_game();
		
		void update();
		void update_gameplay();
		void update_camera();
		
		void read_inputs();
		
		
		void climb_down_stairs();
		void climb_up_stairs();
		
		void clear_ephemeral_objects();
		
		void do_smoke_spray(const Vector2 position, int lifeTime, float size);
		void do_particle_spray(const Vector2 position, int lifeTime, float size, float speed, int amount, const Color & color);
		
		int get_tile_world_position(float x, float y);
		int get_tile(const GameObject & object);
		
		void start_escape_sequence();
		bool escape_sequence_activated();
		
		void win_the_game();
		
		// collision
		bool map_collision(float x, float y, float radius, bool noclip = false);
		void resolve_character_collision(Character & character);
		void resolve_player_collision(Player & player);
		void resolve_projectile_collision(Projectile & projectile);
		void resolve_collision(GameObject & object, bool noclip = false);
		
		
		void compute_attack(Character & attacker);
		void compute_melee_attack(Character & attacker);
		void compute_projectile_attack(Character & attacker);
		void compute_area_damage(const Vector2 & position, float radius, int damage, int secondary_effect = 0, bool enemy = false);
		void compute_stair_climb(Character & character);
		
		Projectile * create_projectile(const Projectile & O);
		DamageMarker * create_damage_marker(const DamageMarker & O);
		Particle * create_particle(const Particle & O);
		
		// draw logic
		void draw();
		//void draw_map();
		void draw_game_world();
		void draw_bar(float x, float y, double hp, double max_hp, const Color & tint);
		void draw_hud();
		void draw_inventory();
		void draw_level_up_menu();
		void draw_main_menu();
		void draw_victory_screen();
		
		
		// save load
		bool save_progress();
		bool load_progress();

};
#endif
