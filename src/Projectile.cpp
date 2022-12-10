#include "Projectile.h"
#include "globals.h"
#include "utilities.h"
#include "stdio.h"

Projectile::Projectile(float x, float y, float angle, int id, bool enemy, int damage): GameObject(x, y, angle), id(id), enemy(enemy), damage(damage)
{
	
	accelerate_angular(angle, get_speed() );
	
	if (id == PROJECTILE::LIGHTNING)
		piercesRemain = 20;
	
	else if (id == PROJECTILE::CHAIN_LIGHTNING)
		piercesRemain = 10;
	
	else
		piercesRemain = 0;
	
}

void Projectile::update()
{
	
	// trail
	if (id == PROJECTILE::LIGHTNING || id == PROJECTILE::CHAIN_LIGHTNING)
	{
		
		const float spread = 8.0f;
		gameManager.create_particle( Particle(position.x + GetRandomValue(-spread, spread), position.y + GetRandomValue(-spread, spread), 0.0f, 0.0f, 60 * 3, 10.0f, RAYWHITE) );
	}
	
	GameObject::update();
	
}



void Projectile::draw()
{
	
	if (Texture2D * sprite = get_sprite() )
	{
	
		const float src_width = sprite->width;
		const float src_height = sprite->height;
		const float half_width = src_width * 0.5f;
		const float half_height = src_height * 0.5f;
		
		DrawTexturePro(*sprite, Rectangle { 0.0f, 0.0f, src_width, src_height }, Rectangle { position.x, position.y, src_width, src_height }, Vector2 { half_width, half_height }, angle, RAYWHITE);
	
	}
	
}


float Projectile::get_speed()
{
	
	switch (id)
	{
		
		case PROJECTILE::ARROW: return 8.0f;
		case PROJECTILE::KNIFE: return 4.0f;
		case PROJECTILE::LIGHTNING: return 6.0f;
		case PROJECTILE::EXPLOSION_POTION: return 3.0f;
		
	}
	return 4.0f;
	
}


Texture2D * Projectile::get_sprite()
{
	
	switch (id)
	{
		
		case PROJECTILE::ARROW: return &arrowTexture;
		case PROJECTILE::KNIFE: return &throwingKnifeTexture;
		case PROJECTILE::FIREBOLT: return &fireboltTexture;
		case PROJECTILE::FIREBALL: return &fireballTexture;
		case PROJECTILE::FROSTBOLT: return &frostboltTexture;
		case PROJECTILE::FREEZING_CLOUD: return &frostboltTexture;
		case PROJECTILE::LIGHTNING: return &lightningTexture;
		case PROJECTILE::CHAIN_LIGHTNING: return &lightningTexture;
		case PROJECTILE::EXPLOSION_POTION: return &explosionPotionTexture;
		
	}
	return NULL;
	
}



void Projectile::do_damage(Character & target)
{
	
	target.take_damage(damage);
	
	if (id == PROJECTILE::FROSTBOLT || id == PROJECTILE::FREEZING_CLOUD)
		target.slowedTimer += (60 * 3);
	
	if (is_piercing() )
	{
		
		piercesRemain--;
		if (piercesRemain <= 0)
			die();
		
		else if (id == PROJECTILE::CHAIN_LIGHTNING)
		{
			
			// busca alvo
			Character * best_target = NULL;
			float best_distance = 9999.0f;
			const float max_distance = TW * 10.0f;
			
			
			if (enemy)
			{
				if (gameManager.player.active && gameManager.player.hurtTimer <= 0)
				{
					
					const float distance = gameManager.player.distance_to(this->position);
					if (distance <= max_distance && distance < best_distance)
					{
						best_target = &gameManager.player;
						best_distance = distance;
					}
					
				}
			}
			
			GameMap & current_map = gameManager.dungeon.get_current_floor();
			for (Enemy & current_enemy : current_map.enemy)
			{
				if (current_enemy.active && current_enemy.hurtTimer <= 0)
				{
					
					const float distance = current_enemy.distance_to(this->position);
					if (distance <= max_distance && distance < best_distance)
					{
						best_target = &current_enemy;
						best_distance = distance;
					}
					
				}
			}
			
			// mira no alvo
			if (best_target)
			{
				
				this->reset_velocity();
				this->angle = this->angle_to(*best_target);
				accelerate_angular(this->angle, get_speed() );
				
				//printf("chain lightning angle: %.2f, distance: %.2f\n", this->angle, best_distance);
				
			}
			
			
		}
		
	}
		
	else
		die();
	
}



void Projectile::die()
{
	
	bool explosive = false;
	if (id == PROJECTILE::FIREBALL)
	{
		gameManager.compute_area_damage(this->position, 50.0f, damage, 0, enemy);
		gameManager.do_particle_spray(this->position, 40, 32.0f, 2.0f, 8, RED);
		explosive = true;
	}
	
	else if (id == PROJECTILE::FREEZING_CLOUD)
	{
		gameManager.compute_area_damage(this->position, 60.0f, damage / 2, 1, enemy);
		gameManager.do_particle_spray(this->position, 40, 32.0f, 2.0f, 8, BLUE);
		explosive = true;
	}
	
	else if (id == PROJECTILE::EXPLOSION_POTION)
	{
		gameManager.compute_area_damage(this->position, 40.0f, 20, 0, enemy);
		gameManager.do_particle_spray(this->position, 40, 32.0f, 2.0f, 8, RED);
		explosive = true;
	}
	
	if (explosive)
		PlaySound(s_Explode);
	
	destroy();
	
}


float Projectile::get_radius()
{
	
	switch (id)
	{
		
		case PROJECTILE::ARROW: return 2.0f;
		case PROJECTILE::KNIFE: return 2.0f;
		case PROJECTILE::FIREBOLT: return 3.0f;
		case PROJECTILE::FIREBALL: return 5.0f;
		case PROJECTILE::FROSTBOLT: return 3.0f;
		case PROJECTILE::FREEZING_CLOUD: return 5.0f;
		case PROJECTILE::LIGHTNING: return 5.0f;
		case PROJECTILE::CHAIN_LIGHTNING: return 5.0f;
		
	}
	return 2.0f;
}


bool Projectile::is_piercing()
{
	return (id == PROJECTILE::LIGHTNING || id == PROJECTILE::CHAIN_LIGHTNING);
}
