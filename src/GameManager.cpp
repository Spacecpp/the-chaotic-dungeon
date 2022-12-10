#include "GameManager.h"
#include "globals.h"
#include "item_data.h"
#include "raylib.h"
#include "lists.h"
#include "utilities.h"
#include "FileHandler.h"

#include <math.h>
#include <string>


GameManager::GameManager(): levelUpMenuPosition(0), levelUpMenuCursorPosition(0), escapeTimer(0)
{
	
	camera = { 0 };
	
	load_progress();
	
	gameState = GAMESTATE::MENU;

	
	//noclip = true;
	
}


void GameManager::UpdateDrawFrame()
{
	
	// Screen scale logic (x2)
    if (IsKeyPressed(KEY_F1) ) screenScale = 1;
    else if (IsKeyPressed(KEY_F2) ) screenScale = 2;
    else if (IsKeyPressed(KEY_F3) ) screenScale = 3;
    
    if (screenScale != prevScreenScale)
    {
        // Scale window to fit the scaled render texture
        SetWindowSize(screenWidth * screenScale, screenHeight * screenScale);
        
        // Scale mouse proportionally to keep input logic inside the 256x256 screen limits
        SetMouseScale(1.0f / (float)screenScale, 1.0f / (float)screenScale);
        
        prevScreenScale = screenScale;
		
    }
	
	update();
    draw();
	
}


void GameManager::shutdown()
{
	
	if (player.active)
		save_progress();
}



void GameManager::start_game()
{
	
	//gameMap.load("map.txt");
	//gameMap.generate_random(20, 20, 6);
	
	gameState = GAMESTATE::PLAYING;
	
	dungeon.reset();
	dungeon.generate_next_floor();
	
	
	player.init_for_new_game();
	
	int x = 0, y = 0;
	if (dungeon.gameMap[0].find_upstairs(x, y) )
		player.set_position_at_tile(x, y);
	
	
	levelUpMenuPosition = 256;
	levelUpMenuCursorPosition = 0;
	
	escapeTimer = 0;
	redTint = 0.5f;
	redTintDirection = true;

}




void GameManager::update()
{
	
	read_inputs();
	
	if (gameState == GAMESTATE::MENU)
	{
		
	}
	
	if (gameState == GAMESTATE::PLAYING)
	{
		
		// level up
		if (player.pointsToDistribute > 0)
		{
			
			// animacao
			if (levelUpMenuPosition > 0)
				levelUpMenuPosition -= 8.0f;
			
			
		}
		
		else update_gameplay();
	
	}
	
}



void GameManager::update_gameplay()
{
	
	if (player.active)
		player.update();
	
	dungeon.update();
	
	for (Projectile & current_projectile : projectile)
	{
		if (current_projectile.active)
			current_projectile.update();
	}
	
	for (DamageMarker & current_damageMarker : damageMarker)
	{
		if (current_damageMarker.active)
			current_damageMarker.update();
	}
	
	for (Particle & current_particle : particle)
	{
		if (current_particle.active)
			current_particle.update();
	}
	
	// ----- collision
	
	if (player.active && !noclip)
		resolve_player_collision(player);
	
	dungeon.update_collision();
	
	for (Projectile & current_projectile : projectile)
	{
		if (current_projectile.active)
			resolve_projectile_collision(current_projectile);
	}
	
	
	if (escapeTimer > 0)
	{
		
		escapeTimer--;
		if (escapeTimer == ESCAPE_TIME)
			PlaySound(s_EscapeNow);
		
		
		const float speed = 0.01f;
		if (redTintDirection)
		{
			redTint += speed;
			if (redTint >= 0.8f)
				redTintDirection = !redTintDirection;
		}
		else
		{
			redTint -= speed;
			if (redTint <= 0.4f)
				redTintDirection = !redTintDirection;
		}
		
		
		// perdeu
		if (escapeTimer == 0)
		{
			
			player.die();
			PlaySound(s_Explode);
			
			do_particle_spray(player.position, 120, 128, 0.05f, 16, RED);
			
		}
		
	}
	
	
	update_camera();
	
}



void GameManager::update_camera()
{
	
	camera.target = (Vector2){ player.position.x, player.position.y };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.zoom = 1.0f;
	
	if (escape_sequence_activated() )
	{
		
		const float delta = 1.0f;
		camera.rotation += GetRandomValue(-delta, delta);
		
		const float limit = 30.0f;
		// limites
		if (camera.rotation > limit)
			camera.rotation = limit;
		
		if (camera.rotation < -limit)
			camera.rotation = -limit;
		
	}
	else camera.rotation = 0.0f;
	
}


void GameManager::read_inputs()
{
	
	if (gameState == GAMESTATE::MENU)
	{
		
		if (IsKeyPressed(KEY_ENTER) )
		{
			start_game();
		}
		
	}
	
	if (gameState == GAMESTATE::ENDING)
	{
		
		if (IsKeyPressed(KEY_ENTER) )
			gameState = GAMESTATE::MENU;
		
	}
	
	if (gameState == GAMESTATE::PLAYING)
	{
		
		if (player.active)
		{
		
			// menu de level up
			if (player.pointsToDistribute > 0)
			{
				
				// trava inputs antes de finalizar a animacao
				if (levelUpMenuPosition <= 0)
				{
					
					if (IsKeyPressed(KEY_UP) )
					{
						
						if (levelUpMenuCursorPosition > 0)
						{
							levelUpMenuCursorPosition--;
							PlaySound(s_Blip);
						}
						
					}
					
					if (IsKeyPressed(KEY_DOWN) )
					{
						
						if (levelUpMenuCursorPosition < 3)
						{
							levelUpMenuCursorPosition++;
							PlaySound(s_Blip);
						}
						
					}
					
					if (IsKeyPressed(KEY_Z) )
					{
						
						if (player.increase_attribute(levelUpMenuCursorPosition) )
						{
							levelUpMenuPosition = 256;
						}
						
					}
					
				}
				
			}
			
			else
			{
				
				if (IsKeyDown(KEY_UP) ) player.inputUp = true;
				if (IsKeyDown(KEY_DOWN) ) player.inputDown = true;
				if (IsKeyDown(KEY_LEFT) ) player.inputLeft = true;
				if (IsKeyDown(KEY_RIGHT) ) player.inputRight = true;
				
				if (IsKeyPressed(KEY_Z) ) player.inputAttack = true;
				if (IsKeyPressed(KEY_X) ) player.inputSecondary = true;
				if (IsKeyPressed(KEY_SPACE) ) player.inputClimb = true;
				
				// selecao de item
				
				if (IsKeyPressed(KEY_ONE) ) player.select_inventory_item(0);
				if (IsKeyPressed(KEY_TWO) ) player.select_inventory_item(1);
				if (IsKeyPressed(KEY_THREE) ) player.select_inventory_item(2);
				if (IsKeyPressed(KEY_FOUR) ) player.select_inventory_item(3);
				if (IsKeyPressed(KEY_FIVE) ) player.select_inventory_item(4);
				if (IsKeyPressed(KEY_SIX) ) player.select_inventory_item(5);
				
				// debug
				
				/*
				#ifdef ALLOW_DEBUG
				if (IsKeyPressed(KEY_O) )
					noclip = !noclip;
				
				if (IsKeyPressed(KEY_P) )
				{
					
					GameMap & current_map = dungeon.get_current_floor();
					
					
					current_map.create_item( Item(10, 10, ITEM::SHORT_BOW, 1, true) );
					
				}
				
				if (IsKeyPressed(KEY_L) )
					escapeTimer = ESCAPE_TIME + 1;
				
				if (IsKeyPressed(KEY_M) )
					win_the_game();
				
				if (IsKeyPressed(KEY_N) )
					dungeon.go_to_floor(25);
				
				#endif
				*/
				
			}
		
		}
		
		else
		{
			if (IsKeyPressed(KEY_ENTER) )
				start_game();
		}
	
	}
	
}


void GameManager::climb_down_stairs()
{
	
	//currentFloorIndex++;
	dungeon.advance_floor();
	
	int x, y;
	if (dungeon.find_current_upstairs(x, y) )
		player.set_position_at_tile(x, y);
	
	clear_ephemeral_objects();
	
}



void GameManager::climb_up_stairs()
{
	
	//currentFloorIndex--;
	if (dungeon.currentFloorIndex == 0 && escape_sequence_activated() )
		win_the_game();
	
	else if (dungeon.go_back_floor() )
	{
	
		int x, y;
		if (dungeon.find_current_downstairs(x, y) )
			player.set_position_at_tile(x, y);
	
	}
	
	clear_ephemeral_objects();
	
}



void GameManager::clear_ephemeral_objects()
{
	
	projectile.clear();
	damageMarker.clear();
	particle.clear();
	
}



void GameManager::do_smoke_spray(const Vector2 position, int lifeTime, float size)
{
	
	Color particleColor { 192, 192, 192, 255 };
	const float speed = 2.0f;
	
	// 4 fumacas ao redor
	create_particle( Particle(position.x, position.y, 45.0f, speed, lifeTime, size, particleColor) );
	create_particle( Particle(position.x, position.y, 135.0f, speed, lifeTime, size, particleColor) );
	create_particle( Particle(position.x, position.y, 225.0f, speed, lifeTime, size, particleColor) );
	create_particle( Particle(position.x, position.y, 315.0f, speed, lifeTime, size, particleColor) );
	
	
}

void GameManager::do_particle_spray(const Vector2 position, int lifeTime, float size, float speed, int amount, const Color & color)
{
	
	if (amount <= 0) return;
	
	//const float speed = 2.0f;
	float angle = 0.0f;
	float angle_add = 360.0f / (float)amount;
	
	for (int i = 0; i < amount; i++)
	{
		create_particle( Particle(position.x, position.y, angle, speed, lifeTime, size, color) );
		angle += angle_add;
	}
	
}


int GameManager::get_tile_world_position(float x, float y)
{
	
	GameMap & current_map = dungeon.get_current_floor();
	return current_map.get_tile_world_position(x, y);
	
}



int GameManager::get_tile(const GameObject & object)
{
	
	GameMap & current_map = dungeon.get_current_floor();
		return current_map.get_tile_world_position(object.position.x, object.position.y);
	
}



void GameManager::start_escape_sequence()
{
	
	escapeTimer = ESCAPE_TIME + (60 * 5);
	
}


bool GameManager::escape_sequence_activated()
{
	return (escapeTimer > 0 && escapeTimer <= ESCAPE_TIME);
}


void GameManager::win_the_game()
{
	//printf("gamewon!\n");
	save_progress();
	gameState = GAMESTATE::ENDING;
	PlaySound(s_Explode);
	
}



void GameManager::compute_attack(Character & attacker)
{
	
	if (attacker.attack_is_ranged() )
		compute_projectile_attack(attacker);
	else
		compute_melee_attack(attacker);
	
}



void GameManager::compute_melee_attack(Character & attacker)
{
	
	GameMap & current_map = dungeon.get_current_floor();
	
	PlaySound(s_Swing); 
	
	attacker.attackDelay = attacker.get_attack_delay(); //30 * (attacker.slowedTimer > 0 ? 2 : 1);
	
	Vector2 hurtPoint = attacker.position;
	
	const float rad_angle = degrees_to_radians(attacker.angle);
	const float step_distance = 4.0f;
	Vector2 step_vector = { cos(rad_angle) * step_distance, sin(rad_angle) * step_distance };
	
	const float max_attack_range = 50.0f;
	const int max_iterations = (int)(max_attack_range / step_distance);
	
	for (int i = 0; i < max_iterations; i++)
	{
		
		if (attacker.is_enemy() )
		{
			
			if (player.point_collision(hurtPoint.x, hurtPoint.y) )
			{
				attacker.attack(player);
				return;
			}
			
		}
		
		else
		{
			
			for (Enemy & current_enemy : current_map.enemy)
			{
				if (current_enemy.active)
				{
					if (current_enemy.point_collision(hurtPoint.x, hurtPoint.y) )
					{
						attacker.attack(current_enemy);
						return;
					}
				}
			}
			
		}
		
		hurtPoint.x += step_vector.x;
		hurtPoint.y += step_vector.y;
		
	}
	
	
}



void GameManager::compute_projectile_attack(Character & attacker)
{
	
	PlaySound(s_Throw); 
	
	attacker.attackDelay = attacker.get_attack_delay(); //attacker.get_base_attack_delay() * (attacker.slowedTimer > 0 ? 2 : 1);
	
	create_projectile( Projectile( attacker.position.x, attacker.position.y, attacker.angle, attacker.get_projectile_type(), attacker.is_enemy(), attacker.get_projectile_damage() ) );
	
}


void GameManager::compute_area_damage(const Vector2 & position, float radius, int damage, int secondary_effect, bool enemy)
{
	
	if (player.active && player.hurtTimer <= 0)
	{
		
		const float distance = player.distance_to(position);
		if (distance <= (radius + player.get_radius() ) )
		{
			
			int final_damage = enemy ? damage : damage / 4;
			player.take_damage(final_damage);
			
			if (player.active)
			{
				player.accelerate_angular( player.angle_to(position), KNOCKBACK_STRENGHT * 0.5f);
				
				if (secondary_effect == SECONDARY_EFFECT::FREEZING)
					player.slowedTimer += 60;
			}
			
		}
		
	}
	
	GameMap & current_map = dungeon.get_current_floor();
	for (Enemy & current_enemy : current_map.enemy)
	{
		if (current_enemy.active && current_enemy.hurtTimer <= 0)
		{
			
			const float distance = current_enemy.distance_to(position);
			if (distance <= (radius + current_enemy.get_radius() ) )
			{
				
				int final_damage = enemy ? damage / 4: damage;
				current_enemy.take_damage(final_damage);
				
				if (current_enemy.active)
				{
					current_enemy.accelerate_angular( current_enemy.angle_to(position), KNOCKBACK_STRENGHT * 0.5f);
					
					if (secondary_effect == SECONDARY_EFFECT::FREEZING)
						current_enemy.slowedTimer += 60;
					
				}
				
			}
			
		}
	}
	
	
}



void GameManager::compute_stair_climb(Character & character)
{
	
	int tile_id = get_tile(character);
		
	if (tile_id == TILE::STAIRS_DOWN)
		climb_down_stairs();
	
	else if (tile_id == TILE::STAIRS_UP)
		climb_up_stairs();
	
}



Projectile * GameManager::create_projectile(const Projectile & O)
{
	
	for (Projectile & object : projectile)
	{
		if (!object.active) { object = O; return &object; }
	}
	
	projectile.push_back(O);
	return &projectile.back();
	
}


DamageMarker * GameManager::create_damage_marker(const DamageMarker & O)
{
	
	for (DamageMarker & object : damageMarker)
	{
		if (!object.active) { object = O; return &object; }
	}
	
	damageMarker.push_back(O);
	return &damageMarker.back();
	
}


Particle * GameManager::create_particle(const Particle & O)
{
	
	for (Particle & object : particle)
	{
		if (!object.active) { object = O; return &object; }
	}
	
	particle.push_back(O);
	return &particle.back();
	
}



// save load
bool GameManager::save_progress()
{
	
	FileHandler file;
	if (file.create("character.sav", false) )
	{
		
		file.write_variable(player.strength);
		file.write_variable(player.dexterity);
		file.write_variable(player.vitality);
		file.write_variable(player.magic);
		
		file.close();
		
		printf("Character saved\n");
		return true;
		
	}
	
	return false;
	
}



bool GameManager::load_progress()
{
	
	FileHandler file;
	if (file.open("character.sav", false) )
	{
		
		file.read_variable(player.strength);
		file.read_variable(player.dexterity);
		file.read_variable(player.vitality);
		file.read_variable(player.magic);
		
		file.close();
		
		printf("Game loaded\n");
		return true;
		
	}
	
	return false;
	
}
