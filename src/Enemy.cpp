#include "Enemy.h"
#include "globals.h"
#include "lists.h"
#include "item_data.h"
#include "utilities_raylib.h"

#include <string>
#include <math.h>

/*
Enemy::Enemy(): Character(), id(0), AIDelay(INITIAL_AI_DELAY), screamTimer(0)
{
	
	HP = get_max_HP();
	
	if (spawns_awake() )
		state = ENEMY_STATE::WANDERING;
	else
		state = ENEMY_STATE::SLEEPING;
	
	if (id == ENEMY::BOSS)
		state = ENEMY_STATE::ALERT;
	
}*/


Enemy::Enemy(float x, float y, int id): Character(x, y), id(id), AIDelay(INITIAL_AI_DELAY), /*animationFrame(0), animationTime(0), */ screamTimer(0)
{
	
	HP = get_max_HP();
	
	if (spawns_awake() )
		state = ENEMY_STATE::WANDERING;
	else
		state = ENEMY_STATE::SLEEPING;
	
	if (id == ENEMY::BOSS)
		state = ENEMY_STATE::ALERT;
	
}



void Enemy::draw()
{
	
	if (id == ENEMY::MIMIC && state == ENEMY_STATE::SLEEPING)
	{
		
		const float draw_x = position.x - TW2;
		const float draw_y = position.y - TW2;
		
		DrawTextureRec(chestTexture, (Rectangle){ 0.0f, 0.0f, TW, TW }, (Vector2) { draw_x, draw_y }, RAYWHITE); 
		
	}
	
	else if (Texture2D * sprite = get_sprite() )
	{
		
		
		if (id == ENEMY::BOSS)
		{
			
			const float src_width = sprite->width;
			const float src_height = sprite->height;
			const float half_width = src_width * 0.5f;
			const float half_height = src_height * 0.5f;
			
			DrawTexturePro(*sprite, Rectangle { 0.0f, 0.0f, src_width, src_height }, Rectangle { position.x, position.y, src_width, src_height }, Vector2 { half_width, half_height }, angle, RAYWHITE);
		
			
		}
		
		else
		{
			
			const float draw_x = position.x - TW2;
			const float draw_y = position.y - TW2;
			
			const float blit_y = (float)(animationFrame * TW);
			
			DrawTextureRec(*sprite, (Rectangle){ 0.0f, blit_y, TW, TW }, (Vector2) { draw_x, draw_y }, RAYWHITE); 
			
			if (invulnerabilityTimer > 0)
				DrawTextureRec(*sprite, (Rectangle){ 0.0f, blit_y, TW, TW }, (Vector2) { draw_x, draw_y }, Color { 255, 255, 255, 64 } ); 
			
			if (slowedTimer > 0)
				DrawTextureRec(*sprite, (Rectangle){ 0.0f, blit_y, TW, TW }, (Vector2) { draw_x, draw_y }, Color { 64, 64, 255, 64 } ); 
			
			// debug
			//std::string str = std::to_string(position.x) + ", " + std::to_string(position.y);
			/*
			std::string str = std::to_string(HP) ;
			DrawText(str.c_str(), position.x, position.y, 16, RAYWHITE );*/
			
		}
		
	}
	
}



void Enemy::update()
{
	
	AI_logic_update();
	Character::update();
	
	/*
	animationTime--;
	if (animationTime <= 0)
	{
		animationTime = ANIMATION_TIME;
		
		animationFrame++;
		if (animationFrame > 1)
			animationFrame = 0;
		
	}*/
	
	// gritos
	if (id == ENEMY::GHOST && state == ENEMY_STATE::ALERT)
	{
		
		screamTimer--;
		if (screamTimer <= 0)
		{
			screamTimer = 25;
			PlaySound(s_Scream);
		}
		
	}
	
}


void Enemy::AI_logic_update()
{
	
	Character & target = gameManager.player;
	if (target.active)
	{
		
		//if (awake)
		if (state == ENEMY_STATE::ALERT)
		{
			
			// melee range
			const float distance_to_player = distance_to(target);
			
			/*
			if (distance_to_player < 30.0f)
				inputAttack = true;
			*/
			
			if (distance_to_player <= TW * 8.0f)
			{
				
				clear_inputs();
				
				// ataque a distancia
				if (has_ranged_attack() && distance_to_player <= TW * 5)
				{
					
					//if (GetRandomValue(0, 100 - get_ranged_attack_probability() ) <= 0)
					if ( random_chance( get_ranged_attack_probability() ) )
						inputAttack = true;
					
					// corrige angulo pra mirar no jogador
					const float delta_x = fabs(this->position.x - target.position.x);
					const float delta_y = fabs(this->position.y - target.position.y);
					const float target_radius = target.get_radius();
					
					if (delta_x <= target_radius)
					{
						
						if (this->position.y < target.position.y)
							inputDown = true;
						
						else if (this->position.y > target.position.y)
							inputUp = true;
							
					}
					
					if (delta_y <= target_radius)
					{
						
						if (this->position.x < target.position.x)
							inputRight = true;
						
						else if (this->position.x > target.position.x)
							inputLeft = true;
							
					}
					
				}
				
				if (!inputAttack)
				{
					
					// segue o jogador
					if (this->position.x < target.position.x)
						inputRight = true;
					
					else if (this->position.x > target.position.x)
						inputLeft = true;
					
					if (this->position.y < target.position.y)
						inputDown = true;
					
					else if (this->position.y > target.position.y)
						inputUp = true;
					
				}
				
			}
			else state = ENEMY_STATE::WANDERING;
			
		}
		
		bool search_for_player = (state == ENEMY_STATE::SLEEPING && AIDelay <= 0);
		
		if (state == ENEMY_STATE::WANDERING)
		{
			
			inputAttack = false;
			
			if (AIDelay <= 0)
			{
				
				inputRight = GetRandomValue(0, 1);
				inputLeft = GetRandomValue(0, 1);
				inputDown = GetRandomValue(0, 1);
				inputUp = GetRandomValue(0, 1);
				
				AIDelay = GetRandomValue(30, 120);
				
				search_for_player = true;
				
			}
			
			
		}
	
		if (search_for_player)
		{
			
			/*
			if (state == ENEMY_STATE::WANDERING)
				printf("searching\n");
			*/
			
			const float distance_to_player = distance_to(target);
			if (distance_to_player <= get_detection_range() )
				state = ENEMY_STATE::ALERT;
			
			else if (state == ENEMY_STATE::SLEEPING)
				AIDelay = 60;
			
		}
		
		if (AIDelay > 0)
			AIDelay--;
		
	}
	else
	{
		if (state != ENEMY_STATE::SLEEPING)
		{
			state = ENEMY_STATE::SLEEPING;
			clear_inputs();
		}
	}
	
}



void Enemy::take_damage(int damage, bool ignore_armor)
{
	
	Character::take_damage(damage, ignore_armor);
	
	//printf ("take_damage\n");
	
	wake_up();
	
	// leve stun
	if (attackDelay < 10)
		attackDelay = 10;
	
}


void Enemy::die()
{
	
	gameManager.player.earn_experience( get_experience() );
	gameManager.player.kills++;
	
	if ( random_chance( get_loot_probability() ) )
	{
		const int item_id = get_loot();
		if (item_id > 0)
		{
			
			int amount = 1;
			if (item_id == ITEM::COINS)
				amount = (gameManager.dungeon.currentFloorIndex + 1) * 3;
			
			gameManager.dungeon.create_item( Item(position.x, position.y, item_id, amount, false) );
			
		}
	}
	
	if (id == ENEMY::BOSS)
	{
		
		gameManager.start_escape_sequence();
		gameManager.do_particle_spray(this->position, 120, 128.0f, 2.0f, 16, RED);
		gameManager.do_particle_spray(this->position, 120, 128.0f, 1.0f, 16, RED);
		PlaySound(s_Explode);
		
	}
	
	Character::die();
	
}



bool Enemy::is_enemy()
{
	return true;
}



void Enemy::wake_up()
{
	
	if (state != ENEMY_STATE::ALERT)
	{
		state = ENEMY_STATE::ALERT;
		AIDelay = 0;
		//printf("woke up\n");
	}
	
}


int Enemy::get_max_HP()
{
	
	switch (id)
	{
		case ENEMY::RAT: return 10;
		case ENEMY::SLIME: return 30;
		case ENEMY::GOBLIN: return 30;
		case ENEMY::IMP: return 25;
		case ENEMY::SKELETON: return 40;
		case ENEMY::SKELETON_ARCHER: return 30;
		case ENEMY::SKELETON_WARRIOR: return 60;
		case ENEMY::GHOST: return 50;
		case ENEMY::DEATH_BLADE: return 80;
		case ENEMY::MIMIC: return 40;
		case ENEMY::DEATH_WIZARD: return 60;
		case ENEMY::DRAGON: return 150;
		case ENEMY::REAPER: return 100;
		case ENEMY::BOSS: return 1000;
		
	} 
	return 1;
	
}


float Enemy::get_movement_speed()
{
	
	switch (id)
	{
		case ENEMY::RAT: return 0.8f;
		case ENEMY::SLIME: return 0.5f;
		case ENEMY::GOBLIN: return 1.0f;
		case ENEMY::IMP: return 1.5f;
		case ENEMY::SKELETON: return 1.0f;
		case ENEMY::SKELETON_ARCHER: return 1.0f;
		case ENEMY::SKELETON_WARRIOR: return 1.2f;
		case ENEMY::GHOST: return 2.2f;
		case ENEMY::DEATH_BLADE: return 1.8f;
		case ENEMY::MIMIC: return 1.0f;
		case ENEMY::DEATH_WIZARD: return 1.0f;
		case ENEMY::DRAGON: return 1.5f;
		case ENEMY::REAPER: return 1.5f;
		case ENEMY::BOSS: return 3.0f;
	} 
	return 1.0f;
	
}


int Enemy::get_attack_strength()
{
	
	switch (id)
	{
		case ENEMY::RAT: return 10;
		case ENEMY::SLIME: return 12;
		case ENEMY::GOBLIN: return 8;
		case ENEMY::IMP: return 10;
		case ENEMY::SKELETON: return 20;
		case ENEMY::SKELETON_ARCHER: return 10;
		case ENEMY::SKELETON_WARRIOR: return 35;
		case ENEMY::GHOST: return 15;
		case ENEMY::DEATH_BLADE: return 40;
		case ENEMY::MIMIC: return 22;
		case ENEMY::DEATH_WIZARD: return 20;
		case ENEMY::DRAGON: return 40;
		case ENEMY::REAPER: return 50;
		case ENEMY::BOSS: return 50;
	} 
	return 10;
	
}


int Enemy::get_projectile_damage()
{
	
	switch (id)
	{
		case ENEMY::GOBLIN: return 8;
		case ENEMY::IMP: return 6;
		case ENEMY::SKELETON_ARCHER: return 10;
		case ENEMY::DEATH_WIZARD: return 50;
		case ENEMY::DRAGON: return 40;
		case ENEMY::BOSS: return 50;
		
	} 
	return 10;
	
}



bool Enemy::attack_is_ranged()
{
	return has_ranged_attack();
}


int Enemy::get_ranged_attack_probability()
{
	
	switch (id)
	{
		case ENEMY::GOBLIN: return 5;
		case ENEMY::IMP: return 3;
		case ENEMY::SKELETON_ARCHER: return 10;
		case ENEMY::DEATH_WIZARD: return 20;
		case ENEMY::DRAGON: return 3;
		case ENEMY::BOSS: return 10;
		
	} 
	return 0;
	
}


int Enemy::get_projectile_type()
{
	switch (id)
	{
		case ENEMY::GOBLIN: return PROJECTILE::KNIFE;
		case ENEMY::IMP: return PROJECTILE::FIREBOLT;
		case ENEMY::SKELETON_ARCHER: return PROJECTILE::ARROW;
		case ENEMY::DEATH_WIZARD: 
		{
			int value = GetRandomValue(0, 2);
			if (value == 0) return PROJECTILE::FIREBOLT;
			if (value == 1) return PROJECTILE::FROSTBOLT;
							return PROJECTILE::LIGHTNING;
		}
		case ENEMY::DRAGON: return PROJECTILE::FIREBALL;
		case ENEMY::BOSS: return GetRandomValue( PROJECTILE::FIREBOLT, PROJECTILE::EXPLOSION_POTION );
		
	} 
	return 0;
}



int Enemy::get_base_attack_delay()
{
	return (has_ranged_attack() ? 50 : 30);
}


bool Enemy::can_walk_through_walls()
{
	return id == ENEMY::GHOST;
}


void Enemy::play_hurt_sound()
{
	PlaySound(s_EnemyHurt);
}


void Enemy::play_death_sound()
{
	PlaySound(s_EnemyDeath);
}


int Enemy::get_experience()
{
	switch (id)
	{
		case ENEMY::RAT: return 1;
		case ENEMY::SLIME: return 3;
		case ENEMY::GOBLIN: return 5;
		case ENEMY::IMP: return 5;
		case ENEMY::SKELETON: return 8;
		case ENEMY::SKELETON_ARCHER: return 14;
		case ENEMY::SKELETON_WARRIOR: return 30;
		case ENEMY::GHOST: return 25;
		case ENEMY::DEATH_BLADE: return 50;
		case ENEMY::MIMIC: return 20;
		case ENEMY::DEATH_WIZARD: return 60;
		case ENEMY::DRAGON: return 100;
		case ENEMY::REAPER: return 50;
		case ENEMY::BOSS: return 400;
	} 
	return 1;
}


bool Enemy::spawns_awake()
{
	return (id == ENEMY::GHOST);
}


bool Enemy::has_ranged_attack()
{
	return get_ranged_attack_probability() > 0;
}


float Enemy::get_detection_range()
{
	
	switch (id)
	{
		case ENEMY::IMP: return TW * 5.0f;
		case ENEMY::SKELETON: return TW * 4.0f;
		case ENEMY::SKELETON_ARCHER: return TW * 4.0f;
		case ENEMY::SKELETON_WARRIOR: return TW * 5.0f;
		case ENEMY::GHOST: return TW * 2.0f;
		case ENEMY::DEATH_BLADE: return TW * 4.0f;
		case ENEMY::MIMIC: return TW * 1.0f;
		case ENEMY::DRAGON: return TW * 5.0f;
		case ENEMY::REAPER: return TW * 10.0f;
	} 
	return TW * 3.0f;
	
}


int Enemy::get_loot()
{
	
	switch (id)
	{
		case ENEMY::SLIME: return ITEM::POISON_POTION;
		case ENEMY::GOBLIN: return ITEM::ARROW;
		case ENEMY::IMP: return ITEM::BOOK_FIREBOLT;
		case ENEMY::SKELETON: return ITEM::LONG_SWORD;
		case ENEMY::SKELETON_ARCHER: return ITEM::ARROW_QUIVER;
		case ENEMY::SKELETON_WARRIOR: return ITEM::BASTARD_SWORD;
		case ENEMY::GHOST: return ITEM::MANA_POTION;
		case ENEMY::DEATH_BLADE: return ITEM::COINS;
		case ENEMY::MIMIC: return ITEM::COINS;
		case ENEMY::DEATH_WIZARD: return ITEM::MANA_POTION;
		case ENEMY::DRAGON: return ITEM::GREAT_SWORD;
		case ENEMY::REAPER: return ITEM::HP_POTION;
	} 
	
	return 0;
	
}



int Enemy::get_loot_probability()
{
	
	switch (id)
	{
		case ENEMY::SLIME: return 10;
		case ENEMY::GOBLIN: return 20;
		case ENEMY::IMP: return 5;
		case ENEMY::SKELETON: return 5;
		case ENEMY::SKELETON_ARCHER: return 40;
		case ENEMY::SKELETON_WARRIOR: return 10;
		case ENEMY::GHOST: return 10;
		case ENEMY::DEATH_BLADE: return 10;
		case ENEMY::MIMIC: return 50;
		case ENEMY::DEATH_WIZARD: return 30;
		case ENEMY::DRAGON: return 5;
		case ENEMY::REAPER: return 5;
	} 
	
	return 0;
	
}


Texture2D * Enemy::get_sprite()
{
	
	if (id == ENEMY::SLIME)
		return &SlimeMonsterTexture;
	
	else if (id == ENEMY::RAT)
		return &ratTexture;
	
	else if (id == ENEMY::GOBLIN)
		return &goblinTexture;
	
	else if (id == ENEMY::IMP)
		return &impTexture;
	
	else if (id == ENEMY::SKELETON)
		return &skeletonTexture;
	
	else if (id == ENEMY::SKELETON_ARCHER)
		return &skeletonArcherTexture;
	
	else if (id == ENEMY::GHOST)
		return &ghostTexture;
	
	else if (id == ENEMY::DEATH_BLADE)
		return &deathBladeTexture;
	
	else if (id == ENEMY::MIMIC)
		return &mimicTexture;
	
	else if (id == ENEMY::SKELETON_WARRIOR)
		return &skeletonWarriorTexture;
	
	else if (id == ENEMY::REAPER)
		return &reaperTexture;
	
	else if (id == ENEMY::DEATH_WIZARD)
		return &deathWizardTexture;
	
	else if (id == ENEMY::DRAGON)
		return &dragonTexture;
	
	else if (id == ENEMY::BOSS)
		return &bossTexture;
	
	// fallback
	return &unknownTexture;
	
}



int Enemy::get_attack_delay()
{
	
	int delay = Character::get_attack_delay();
	
	if (has_ranged_attack() )
		delay *= 2;
	
	return delay;
	
}


float Enemy::get_radius()
{
	if (id == ENEMY::BOSS)
		return 64.0f;
	return 8.0f;
	
}
