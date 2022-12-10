#include "item_data.h"
#include "globals.h"

Texture2D * get_item_sprite(int id)
{
	
	switch (id)
	{
		
		case ITEM::COINS: return &coinsTexture;
		
		case ITEM::ARROW: return &arrowItemTexture;
		case ITEM::ARROW_QUIVER: return &arrowQuiverTexture;
		
		case ITEM::HP_POTION: return &hpPotionTexture;
		case ITEM::MANA_POTION: return &manaPotionTexture;
		case ITEM::PISS_POTION: return &pissPotionTexture;
		case ITEM::POISON_POTION: return &poisonPotionTexture;
		case ITEM::EXPLOSION_POTION: return &explosionPotionTexture;
		case ITEM::EXPERIENCE_POTION: return &poisonPotionTexture; // intentional
		
		case ITEM::SHORT_SWORD: return &shortSwordTexture;
		case ITEM::LONG_SWORD: return &longSwordTexture;
		case ITEM::BASTARD_SWORD: return &bastardSwordTexture;
		case ITEM::GREAT_SWORD: return &greatSwordTexture;
		
		case ITEM::SHORT_BOW: return &shortBowTexture;
		case ITEM::LONG_BOW: return &longBowTexture;
		
		case ITEM::LEATHER_ARMOR: return &leatherArmorTexture;
		case ITEM::CHAIN_MAIL: return &chainMailTexture;
		case ITEM::DRAGON_LEATHER_ARMOR: return &dragonLeatherArmorTexture;
		case ITEM::PLATE_MAIL: return &plateMailTexture;
		case ITEM::GOLDEN_PLATE: return &goldenPlateTexture;
		
		case ITEM::BOOK_FIREBOLT: return &bookFireboltTexture;
		case ITEM::BOOK_FIREBALL: return &bookFireballTexture;
		case ITEM::BOOK_FROSTBOLT: return &bookFrostboltTexture;
		case ITEM::BOOK_FREEZING_CLOUD: return &bookFreezingCloudTexture;
		case ITEM::BOOK_LIGHTNING: return &bookLightningTexture;
		case ITEM::BOOK_CHAIN_LIGHTNING: return &bookChainLightningTexture;
		
	}
	
	//return &unknownTexture;
	return NULL;
	
}


bool item_is_armor(int id)
{
	return (id == ITEM::LEATHER_ARMOR || id == ITEM::CHAIN_MAIL || id == ITEM::DRAGON_LEATHER_ARMOR || 
			id == ITEM::PLATE_MAIL || id == ITEM::GOLDEN_PLATE);
}


bool item_is_a_melee_weapon(int id)
{
	return (id == ITEM::SHORT_SWORD || id == ITEM::LONG_SWORD || id == ITEM::BASTARD_SWORD || id == ITEM::GREAT_SWORD); 
}


bool item_is_a_ranged_weapon(int id)
{
	return (id == ITEM::SHORT_BOW || id == ITEM::LONG_BOW); 
}



bool item_is_a_spellbook(int id)
{
	return (id == ITEM::BOOK_FIREBOLT || id == ITEM::BOOK_FIREBALL || 
			id == ITEM::BOOK_FROSTBOLT || id == ITEM::BOOK_FREEZING_CLOUD ||
			id == ITEM::BOOK_LIGHTNING || id == ITEM::BOOK_CHAIN_LIGHTNING);
}


bool item_is_a_potion(int id)
{
	return (id == ITEM::HP_POTION || id == ITEM::MANA_POTION || id == ITEM::PISS_POTION || 
			id == ITEM::POISON_POTION || id == ITEM::EXPLOSION_POTION || id == ITEM::EXPERIENCE_POTION);
}


bool item_is_stackable(int id)
{
	return item_is_a_potion(id);
}
