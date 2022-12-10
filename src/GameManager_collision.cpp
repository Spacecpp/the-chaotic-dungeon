#include "GameManager.h"
#include "globals.h"
#include "raylib.h"
#include "lists.h"
#include "item_data.h"
#include "utilities.h"
#include "utilities_raylib.h"

#include <math.h>
#include <string>


bool GameManager::map_collision(float x, float y, float radius, bool noclip)
{
	
	// certificar de arredondar pra baixo
	int left_tile = (int)floor( (x - radius) / TW );
	int right_tile = (int)( (x + radius) / TW );
	int top_tile = (int)floor( (y - radius) / TW );
	int bottom_tile = (int)( (y + radius) / TW );
	
	GameMap & current_map = dungeon.get_current_floor();
	
	if (left_tile < 0) return true;
	if (top_tile < 0) return true;
	if (right_tile >= current_map.width) return true;	
	if (bottom_tile >= current_map.height) return true;
	
	if (!noclip)
	{
		
		for (int y = top_tile; y <= bottom_tile; y++)
		{
			
			for (int x = left_tile; x <= right_tile; x++)
			{		
				
				if (current_map.tile_is_solid(x, y) )
					return true;
				
			}
			
		}
	
	}
	
	return false;
	
}

void GameManager::resolve_character_collision(Character & character)
{
	
	resolve_collision(character, character.can_walk_through_walls() );
	
	
	if (character.is_enemy() && character.can_be_hurt() )
	{
		
		if (player.active && player.can_be_hurt() )
		{
			
			// melee attack on contact
			if (character.collision(player) )
			{
				character.attack(player);
				character.wake_up();
			}
		
		}
		
		
		// inimigos se repelem no contato
		GameMap & current_map = dungeon.get_current_floor();
		for (Enemy & current_enemy : current_map.enemy)
		{
			
			if (current_enemy.active && &character != &current_enemy && character.collision(current_enemy) )
			{
				
				//printf("collision\n");
				
				const float angle = character.angle_to(current_enemy);
				const float distance = (character.get_radius() + current_enemy.get_radius() - character.distance_to(current_enemy) ) * 0.5f;
				
				character.accelerate_angular(angle, -distance);
				current_enemy.accelerate_angular(angle, distance);
				
			}
			
		}
		
	}
	
	
}



void GameManager::resolve_player_collision(Player & player)
{
	
	resolve_character_collision(player);
	
	// interacao com items
	if (player.moved_this_frame() )
	{
		
		GameMap & current_map = dungeon.get_current_floor();
		for (Item & current_item : current_map.item)
		{
			
			if (current_item.active && player.collision(current_item) )
			{
				
				if (current_item.is_chest)
				{
					current_item.is_chest = false;
					PlaySound(s_Open);
				}
				
				else
					player.pickup_item_if_possible(current_item);
				
			}
			
		}
		
	}
	
}



void GameManager::resolve_projectile_collision(Projectile & projectile)
{
	
	if (projectile.enemy)
	{
		if (player.can_be_hurt() && projectile.collision(player) )
			projectile.do_damage(player);
	}
	
	else
	{
		
		GameMap & current_map = dungeon.get_current_floor();
		for (Enemy & current_enemy : current_map.enemy)
		{
			if (current_enemy.active && current_enemy.can_be_hurt() && projectile.collision(current_enemy) )
			{
				projectile.do_damage(current_enemy);
				break;
			}
		}
		
	}
	
	// colisao com as paredes do mapa
	const float radius = projectile.get_radius();
	if (map_collision(projectile.position.x, projectile.position.y, radius) )
	{
		
		if (projectile.id == PROJECTILE::ARROW || projectile.id == PROJECTILE::KNIFE)
			do_smoke_spray(projectile.position, 30, 8.0f);
		
		// flecha pode cair no chao
		if (projectile.id == PROJECTILE::ARROW)
		{
			int chance = projectile.enemy ? 10 : 50;
			if (random_chance(chance) )
			{
				GameMap & current_map = dungeon.get_current_floor();
				current_map.create_item( Item(projectile.position.x, projectile.position.y, ITEM::ARROW, 1, false) );
			}
		}
		
		projectile.die();
		
	}
	
}


void GameManager::resolve_collision(GameObject & object, bool noclip)
{
	

	const float radius = object.get_radius();
	if (map_collision(object.position.x, object.position.y, radius, noclip) )
	{
		
		bool x_collsion, y_collsion;
		
		if (map_collision(object.position.x, object.oldPosition.y, radius, noclip) )
			x_collsion = true;
		else 
			x_collsion = false;
		
		
		if (map_collision(object.oldPosition.x, object.position.y, radius, noclip) )
			y_collsion = true;
		else 
			y_collsion = false;
			
		
		if (x_collsion)
		{
			object.position.x = object.oldPosition.x;
			//printf("x ");
		}
		
		if (y_collsion)
		{
			object.position.y = object.oldPosition.y;
			//printf("y ");
		}
		
	}		
	
}
