#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include "raylib.h"

namespace ITEM { enum { NOTHING, COINS, ARROW, ARROW_QUIVER, 
						HP_POTION, MANA_POTION, PISS_POTION, POISON_POTION, EXPLOSION_POTION, EXPERIENCE_POTION,
						SHORT_SWORD, LONG_SWORD, BASTARD_SWORD, GREAT_SWORD, 
						SHORT_BOW, LONG_BOW, 
						BOOK_FIREBOLT, BOOK_FIREBALL, BOOK_FROSTBOLT, BOOK_FREEZING_CLOUD, BOOK_LIGHTNING, BOOK_CHAIN_LIGHTNING,
						LEATHER_ARMOR, CHAIN_MAIL, DRAGON_LEATHER_ARMOR, PLATE_MAIL, GOLDEN_PLATE,
						COUNT }; }
						
Texture2D * get_item_sprite(int id);
bool item_is_armor(int id);
bool item_is_a_melee_weapon(int id);
bool item_is_a_ranged_weapon(int id);
bool item_is_a_spellbook(int id);
bool item_is_a_potion(int id);
bool item_is_stackable(int id);

#endif
