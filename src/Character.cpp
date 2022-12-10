#include "Character.h"
#include "globals.h"
#include "raylib.h"
#include "utilities.h"
#include "lists.h"

Character::Character(): GameObject(), animationFrame(0), animationTime(0), HP(1), attackDelay(0), hurtTimer(0), invulnerabilityTimer(0), regenerationTimer(0), slowedTimer(0),
	inputLeft(false), inputRight(false), inputUp(false), inputDown(false), inputAttack(false), inputSecondary(false), inputClimb(false)
{
}

Character::Character(float x, float y): GameObject(x, y), animationFrame(0), animationTime(0), HP(1), attackDelay(0), hurtTimer(0), invulnerabilityTimer(0), regenerationTimer(0), slowedTimer(0),
	inputLeft(false), inputRight(false), inputUp(false), inputDown(false), inputAttack(false), inputSecondary(false), inputClimb(false)
{
	
}


void Character::update()
{
	
	if (hurtTimer > 0)
		hurtTimer--;
	
	if (invulnerabilityTimer > 0)
		invulnerabilityTimer--;
	
	if (attackDelay > 0)
		attackDelay--;
	
	if (slowedTimer > 0)
		slowedTimer--;
	
	if (currently_walking() )
	{
		
		animationTime--;
		if (animationTime <= 0)
		{
			
			animationTime = get_animation_delay();//ANIMATION_TIME;
			
			animationFrame++;
			if (animationFrame > 1)
				animationFrame = 0;
			
			
		}
	
	}
	
	regenerationTimer--;
	if (regenerationTimer <= 0)
	{
		
		regenerationTimer = HP_REGEN_TIME;
		if (HP < get_max_HP() )
			HP++;
		
	}
	
	if (hurtTimer <= 0 && attackDelay <= 0)
	{
		
		float speed = get_movement_speed();
		if (slowedTimer > 0)
			speed *= 0.5f;
		
		if (inputUp) move(0.0f, -speed);
		if (inputDown) move(0.0f, speed);
		if (inputLeft) move(-speed, 0.0f);
		if (inputRight) move(speed, 0.0f);
		
		// atualiza o angulo
		if (inputUp || inputDown || inputLeft || inputRight)
			angle = radians_to_degrees( atan2(velocity.y, velocity.x) );
		
	}
	
	if (attackDelay <= 0)
	{
		
		if (inputAttack)
			do_primary_action();
			//gameManager.compute_attack(*this);
		
		if (inputClimb)
			gameManager.compute_stair_climb(*this);
		
	}
	
	
	GameObject::update();
	
	if (hurtTimer <= 0)
		reset_velocity();
	
	if (!is_enemy() )
		clear_inputs();
	
}



void Character::clear_inputs()
{
	
	inputLeft = false; 
	inputRight = false; 
	inputUp = false; 
	inputDown = false;
	inputAttack = false;
	inputSecondary = false;
	inputClimb = false;
	
}


void Character::take_damage(int damage, bool ignore_armor)
{
	
	if (damage <= 0) return;
	
	const int final_damage = ignore_armor ? damage : std::max(damage - get_AC(), 1);
	HP -= final_damage;
	
	hurtTimer = KNOCKBACK_TIME;
	invulnerabilityTimer = get_base_invulnerability_timer();
	
	regenerationTimer = HP_REGEN_TIME; // reseta ao tomar dano
	
	gameManager.create_damage_marker( DamageMarker(position.x, position.y, final_damage) );
	
	if (HP <= 0)
	{
		HP = 0;
		die();
	}
	else
		play_hurt_sound();
	
}


void Character::die()
{
	
	
	const float particle_size = 16.0f;
	gameManager.do_smoke_spray(position, 50, particle_size);
	
	play_death_sound();
	
	destroy();
	
}


void Character::heal(int amount)
{
	
	HP += amount;
	
	const int max_hp = get_max_HP();
	if (HP > max_hp)
		HP = max_hp;
	
}


void Character::attack(Character & other)
{
	
	other.take_damage( this->get_attack_strength() );
	other.accelerate_angular( angle_between_points(this->position.x, this->position.y, other.position.x, other.position.y), KNOCKBACK_STRENGHT);
	
}


void Character::wake_up()
{
	
}


int Character::get_animation_delay()
{
	return ANIMATION_TIME;
}

bool Character::currently_walking()
{
	return true;
}

void Character::do_primary_action()
{
	gameManager.compute_attack(*this);
}



bool Character::is_enemy()
{
	return false;
}



int Character::get_max_HP()
{
	return 100;
}


int Character::get_AC()
{
	return 0;
}


float Character::get_movement_speed()
{
	return 2.0f;
}



int Character::get_attack_strength()
{
	return 10;
}

int Character::get_projectile_damage()
{
	return 10;
}


bool Character::attack_is_ranged()
{
	return false;
}


int Character::get_projectile_type()
{
	return 0;
}


int Character::get_base_attack_delay()
{
	return 30;
}


bool Character::can_be_hurt()
{
	return /*hurtTimer <= 0 && */invulnerabilityTimer <= 0;
}


bool Character::can_walk_through_walls()
{
	return false;
}

void Character::play_hurt_sound()
{
	PlaySound(s_PlayerHurt);
}

void Character::play_death_sound()
{
	
}

int Character::get_base_invulnerability_timer()
{
	return KNOCKBACK_TIME;
}


int Character::get_attack_delay()
{
	return (30 * (slowedTimer > 0 ? 2 : 1) );
}


