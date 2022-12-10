#include "Player.h"
#include "globals.h"
#include "lists.h"
#include "item_data.h"
#include "utilities.h"

#include <string>

Player::Player(): Character(), mana(0), experience(0), level(0), pointsToDistribute(0), manaRegenerationTimer(0), 
	selectedInventorySlot(0), coins(0), arrows(0), armorId(0), kills(0)
{
	init_base_values();
}



void Player::init_base_values()
{
	
	maxHP = 50;
	maxMana = 0;
	maxExperience = 10;
	
	strength = 0;
	dexterity = 0;
	vitality = 0;
	magic = 0;
	
	for (size_t i = 0; i < MAX_INVENTORY_SLOTS; i++)
		inventory[i].set(0, 0);
	
	
}



void Player::init_for_new_game()
{
	
	active = true;
	
	maxHP = 50 + vitality * 5;
	HP = maxHP;
	
	maxMana = magic * 5;
	mana = maxMana;
	
	experience = 0;
	maxExperience = 10;
	level = 1;
	pointsToDistribute = 0;
	
	for (size_t i = 0; i < MAX_INVENTORY_SLOTS; i++)
		inventory[i].set(0, 0);
	
	inventory[0].set(ITEM::SHORT_SWORD, 1);
	
	selectedInventorySlot = 0;
	
	coins = 0;
	arrows = 0;
	armorId = 0;
	
	hurtTimer = 0;
	invulnerabilityTimer = 0;
	regenerationTimer = 0;
	attackDelay = 0;
	slowedTimer = 0;
	
	//explosionBuff = 0;
	swordAnimation = false;
	kills = 0;
	
	angle = 90;
	
	clear_inputs();
	
}


void Player::update()
{
	
	// troca de item
	if (inputSecondary)
	{
		
		GameMap & current_map = gameManager.dungeon.get_current_floor();
		for (Item & current_item : current_map.item)
		{
			
			if (current_item.active && this->collision(current_item) && this->cannot_autopick_item(current_item) )
			{
				swap_items(current_item);
				break;
			}
		}
		
	}
	
	Character::update();
	
	manaRegenerationTimer--;
	if (manaRegenerationTimer <= 0)
	{
		
		manaRegenerationTimer = MANA_REGEN_TIME;
		if (mana < maxMana )
			mana++;
		
	}
	
	if (attackDelay <= 0)
		swordAnimation = false;
	
}



void Player::die()
{
	
	Character::die();
	gameManager.save_progress();
	
}



bool Player::increase_attribute(int attribute)
{
	
	if (attribute == ATTRIBUTE::STRENGTH)
		strength++;
	
	else if (attribute == ATTRIBUTE::DEXTERITY)
		dexterity++;
	
	else if (attribute == ATTRIBUTE::VITALITY)
	{
		vitality++;
		maxHP += 5;
		HP += 5;
	}

	else if (attribute == ATTRIBUTE::MAGIC)
	{
		magic++;
		maxMana += 5;
		mana += 5;
	}
	
	pointsToDistribute--;
	return true;
	
}



void Player::earn_experience(int value)
{
	
	experience += value;
	if (experience >= maxExperience)
	{
		
		level++;
		pointsToDistribute++;
		experience = 0;
		maxExperience += (maxExperience / 2);
		PlaySound(s_LevelUp);
		
	}
	
}


int Player::get_free_inventory_slot_index()
{
	
	for (size_t i = 0; i < MAX_INVENTORY_SLOTS; i++)
	{
		if (inventory[i].id <= 0)
			return i;
	}
	return -1;
	
}


bool Player::cannot_autopick_item(Item & item)
{
	
	// sempre pega
	if (item.id == ITEM::COINS || item.id == ITEM::ARROW || item.id == ITEM::ARROW_QUIVER)
		return false;
	
	if (item.is_armor() )
		return armorId != 0;
	
	if (item.is_stackable() )
	{
		
		for (int i = 0; i < MAX_INVENTORY_SLOTS; i++)
		{
			if (inventory[i].id == item.id)
				return false;
		}
		
	}
	
	return (get_free_inventory_slot_index() == -1);
	
}



bool Player::pickup_item_if_possible(Item & item)
{
	
	bool picked = false;
	
	if (item.id == ITEM::COINS)
	{
		this->coins += item.amount;
		picked = true;
	}
	
	else if (item.id == ITEM::ARROW)
	{
		this->arrows += item.amount;
		picked = true;
	}
	
	else if (item.id == ITEM::ARROW_QUIVER)
	{
		this->arrows += item.amount * 10;
		picked = true;
	}
	
	else if (item.is_armor() )
	{
		if (armorId == 0)
		{
			armorId = item.id;
			picked = true;
		}
	}
	
	else if (item.is_stackable() )
	{
		
		for (int i = 0; i < MAX_INVENTORY_SLOTS; i++)
		{
			if (inventory[i].id == item.id)
			{
				inventory[i].amount += item.amount;
				picked = true;
				break;
			}
		}
		
		if (!picked)
		{
			
			int slot = get_free_inventory_slot_index();
			if (slot != -1)
			{
				
				inventory[slot].set(item.id, item.amount);
				picked = true;
				
			}
	
		}
		
	}
	
	else	
	{
		
		int slot = get_free_inventory_slot_index();
		if (slot != -1)
		{
			
			inventory[slot].set(item.id, item.amount);
			picked = true;
			
		}
	
	}
	
	if (picked)
	{
		PlaySound(s_Pickup);
		item.destroy();
	}
	
	return picked;
	
}



void Player::swap_items(Item & item)
{
	
	if (item.is_armor() )
	{
		std::swap( armorId, item.id);
	}
	
	else
	{
		std::swap( inventory[selectedInventorySlot].id, item.id);
		std::swap( inventory[selectedInventorySlot].amount, item.amount);
	}
	
	PlaySound(s_Blip);
	
}


void Player::consume_selected_item()
{
	
	inventory[selectedInventorySlot].amount--;
	if (inventory[selectedInventorySlot].amount <= 0)
		inventory[selectedInventorySlot].id = 0;
	
}



void Player::do_potion_effect(int item_id)
{
	
	switch (item_id)
	{
		
		case ITEM::HP_POTION: heal(50); break;
		case ITEM::MANA_POTION: recover_mana(50); break;
		case ITEM::PISS_POTION: take_damage(1, true); break;
		case ITEM::POISON_POTION: take_damage(20, true); break;
		case ITEM::EXPLOSION_POTION: 
		{
			//explosionBuff = 60 * 10; 
			gameManager.create_projectile( Projectile( this->position.x, this->position.y, this->angle, PROJECTILE::EXPLOSION_POTION, false, 25) );
			break;
		}
		case ITEM::EXPERIENCE_POTION: earn_experience(maxExperience); break;
		
	}
	
}


void Player::recover_mana(int amount)
{
	
	mana += amount;
	
	if (mana > maxMana)
		mana = maxMana;
	
}



void Player::draw()
{
	
	const float draw_x = position.x - TW2;
	const float draw_y = position.y - TW2 - 6.0f; // o -6 eh pra evitar os pes aparecerem entrando na parede
	
	// animacao da espada
	if (swordAnimation)
	{
		
		const float rad_angle = degrees_to_radians(angle);
		const float distance = TW2;
		
		const float sword_draw_x = position.x + cos(rad_angle) * distance;
		const float sword_draw_y = position.y + sin(rad_angle) * distance;
		
		DrawTexturePro(swordAnimationTexture, Rectangle { 0.0f, 0.0f, (float)swordAnimationTexture.width, (float)swordAnimationTexture.height }, Rectangle { sword_draw_x, sword_draw_y, (float)swordAnimationTexture.width, (float)swordAnimationTexture.height }, Vector2 { 2.0f, 8.0f }, angle, RAYWHITE);
		
	}
	
	
	const float blit_y = (float)(animationFrame * TW);
	float blit_x = 0.0f;
	
	if (angle > -45.0f && angle < 45.0f)
		blit_x = 0.0f;
	
	else if (angle >= 45.0f && angle <= 135.0f)
		blit_x = TW;
	
	else if (angle > 135.0f && angle < 225.0f)
		blit_x = TW * 2.0f;
	
	else //if (angle >= 225.0f && angle <= 315.0f)
		blit_x = TW * 3.0f;
	
	DrawTextureRec(playerTexture, (Rectangle){ blit_x, blit_y, TW, TW }, (Vector2) { draw_x, draw_y }, RAYWHITE); 
	
	if (invulnerabilityTimer > 0)
		DrawTextureRec(playerTexture, (Rectangle){ blit_x, blit_y, TW, TW }, (Vector2) { draw_x, draw_y }, Color { 255, 255, 255, 64 } ); 
	
	if (slowedTimer > 0)
		DrawTextureRec(playerTexture, (Rectangle){ blit_x, blit_y, TW, TW }, (Vector2) { draw_x, draw_y }, Color { 64, 64, 255, 64 } ); 
	
	/*
	DrawTextureV(playerTexture, Vector2 { draw_x, draw_y }, RAYWHITE);
	
	if (invulnerabilityTimer > 0)
		DrawTextureV(playerTexture, Vector2 { draw_x, draw_y }, Color { 255, 255, 255, 64 } );
	*/
	
	
	/*
	// debug
	std::string str = std::to_string( (int)position.x / TW) + ", " + std::to_string( (int)position.y / TW);
	DrawText(str.c_str(), position.x, position.y, 16, RAYWHITE );
	*/
	
	/*
	std::string str = std::to_string( (int)angle);
	DrawText(str.c_str(), position.x, position.y, 16, RAYWHITE );
	*/
	
	/*
	std::string str = std::to_string( (int)HP) + " / " + std::to_string( (int)max_hp);
	DrawText(str.c_str(), position.x, position.y, 8, RAYWHITE);
	*/
}

//----

int Player::get_animation_delay()
{
	return 10;
}


bool Player::currently_walking()
{
	return moved_this_frame();
}

void Player::do_primary_action()
{
	
	const int item_id = get_selected_item_id();
	if (item_id >= 0)
	{
		
		
		if (item_is_a_melee_weapon(item_id) )
		{
			gameManager.compute_melee_attack(*this);
			swordAnimation = true;
		}
		
		else if (item_is_a_ranged_weapon(item_id) )
		{
			if (arrows > 0)
			{
				gameManager.compute_projectile_attack(*this);
				arrows--;
			}
			else PlaySound(s_NoAmmo);
		}
		
		else if (item_is_a_spellbook(item_id) )
		{
			
			const int mana_cost = get_mana_cost(item_id);
			if (mana >= mana_cost)
			{
				
				mana -= mana_cost;
				
				int delayReducer = std::min( (int)( (float)magic * 0.5f), 10); // magic deixa o ataque mais rapido
				this->attackDelay = 40 - delayReducer;
				
				int projectile_type = 0;
				int damage = 0;
				
				switch (item_id)
				{
					case ITEM::BOOK_FIREBOLT: { projectile_type = PROJECTILE::FIREBOLT; damage = 20; break; }
					case ITEM::BOOK_FIREBALL: { projectile_type = PROJECTILE::FIREBALL; damage = 50; break; }
					case ITEM::BOOK_FROSTBOLT: { projectile_type = PROJECTILE::FROSTBOLT; damage = 15; break; }
					case ITEM::BOOK_FREEZING_CLOUD: { projectile_type = PROJECTILE::FREEZING_CLOUD; damage = 30; break; }
					case ITEM::BOOK_LIGHTNING: { projectile_type = PROJECTILE::LIGHTNING; damage = 15; break; }
					case ITEM::BOOK_CHAIN_LIGHTNING: { projectile_type = PROJECTILE::CHAIN_LIGHTNING; damage = 25; break; }
				}
				
				damage = (int)( (float)damage * get_magic_damage_multiplier() );
				
				gameManager.create_projectile( Projectile( this->position.x, this->position.y, this->angle, projectile_type, false, damage) );
				
				PlaySound(s_Spell);
				
			}
			else PlaySound(s_NotEnoughMana);
			
		}
		
		else if (item_is_a_potion(item_id) )
		{
			
			PlaySound(s_Drink);
			do_potion_effect(item_id);
			consume_selected_item();
			attackDelay = 30;
			
		}
		
		//else if (item_is_a_spell(item_id) )
		
	}
	else printf("No item to use\n");

}



int Player::get_max_HP()
{
	return maxHP;
}


int Player::get_AC()
{
	
	switch (armorId)
	{
		case ITEM::LEATHER_ARMOR: return 3;
		case ITEM::CHAIN_MAIL: return 6;
		case ITEM::DRAGON_LEATHER_ARMOR: return 9;
		case ITEM::PLATE_MAIL: return 14;
		case ITEM::GOLDEN_PLATE: return 20;
	}
	return 0;
	
}



float Player::get_movement_speed()
{
	return std::min(PLAYER_BASE_SPEED * ( 1.0f + (float)dexterity * 0.1f), PLAYER_MAX_SPEED);
}


int Player::get_attack_strength()
{
	
	return (int)(get_selected_item_base_damage() * ( 1.0f + (float)strength * 0.1f) );
	
}


int Player::get_projectile_damage()
{
	return (int)(get_selected_item_base_damage() * ( 1.0f + (float)dexterity * 0.1f) );
}



float Player::get_magic_damage_multiplier()
{
	return (1.0f + (float)magic * 0.1f);
}



int Player::get_mana_cost(int item_id)
{
	
	switch (item_id)
	{
		
		case ITEM::BOOK_FIREBOLT: return 2;
		case ITEM::BOOK_FIREBALL: return 8;
		case ITEM::BOOK_FROSTBOLT: return 2;
		case ITEM::BOOK_FREEZING_CLOUD: return 8;
		case ITEM::BOOK_LIGHTNING: return 3;
		case ITEM::BOOK_CHAIN_LIGHTNING: return 10;
		
	}
	return 1;
	
}


bool Player::attack_is_ranged()
{
	
	const int item_id = get_selected_item_id();
	return (item_id == ITEM::SHORT_BOW || item_id == ITEM::LONG_BOW);
	
}


int Player::get_projectile_type()
{
	
	return PROJECTILE::ARROW;
}


int Player::get_selected_item_id()
{
	// prevencao de pegar item inexistente
	return (inventory[selectedInventorySlot].amount > 0 ? inventory[selectedInventorySlot].id : -1);
}


bool Player::select_inventory_item(int slot)
{
	
	// bound check
	if (slot >= 0 && slot < MAX_INVENTORY_SLOTS)
	{
		
		if (inventory[slot].id != 0 && selectedInventorySlot != slot)
		{
			selectedInventorySlot = slot;
			PlaySound(s_Blip);
			return true;
		}
		
	}
	return false;
	
}


float Player::get_selected_item_base_damage()
{
	
	int id = get_selected_item_id();
	switch (id)
	{
		case ITEM::SHORT_SWORD: return 10;
		case ITEM::LONG_SWORD: return 15;
		case ITEM::BASTARD_SWORD: return 22;
		case ITEM::GREAT_SWORD: return 34;
		case ITEM::SHORT_BOW: return 10;
		case ITEM::LONG_BOW: return 20;
	}
	return 1;
	
}


int Player::get_base_invulnerability_timer()
{
	return KNOCKBACK_TIME * 5;
}


int Player::get_attack_delay()
{
	int delayReducer = std::min( (int)( (float)dexterity * 0.5f), 10); // dexterity deixa o ataque mais rapido
	return Character::get_attack_delay() - delayReducer;
}


void Player::play_death_sound()
{
	PlaySound(s_PlayerDeath);
}

