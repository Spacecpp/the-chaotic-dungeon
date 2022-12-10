#include "Item.h"
#include "globals.h"
#include "lists.h"
#include "Item_data.h"
#include "utilities_raylib.h"
#include <string>

Item::Item(): GameObject(), id(0), amount(0), is_chest(false)
{
	
}

Item::Item(float x, float y, int id, int amount, bool is_chest): GameObject(x, y), id(id), amount(amount), is_chest(is_chest)
{
	
}



void Item::draw()
{
	
	Texture2D * sprite = is_chest ? &chestTexture : get_item_sprite(id);
	if (sprite)
	{
		DrawTextureRec(*sprite, (Rectangle){ 0.0f, 0.0f, TW, TW }, (Vector2) { position.x - TW2, position.y - TW2 }, RAYWHITE); 
	}
	else DrawTextShadow( std::to_string(id).c_str(), position.x - TW2, position.y - TW2, 8, RAYWHITE, BLACK, 1); // debug

}


float Item::get_radius()
{
	return is_chest ? 20.0f : 8.0f;
}


bool Item::is_armor()
{
	return item_is_armor(id);
}

bool Item::is_stackable()
{
	return item_is_stackable(id);
}
