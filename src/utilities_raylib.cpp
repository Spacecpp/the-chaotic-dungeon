#include "utilities_raylib.h"
#include <stdio.h>

bool load_texture(Texture2D & texture, const char * path)
{
	
	texture = LoadTexture(path);
	if (texture.id == 0)
	{
		printf("Failed to load texture %s\n", path);
		return false;
	}
	return true;
	
}


bool load_sound(Sound & sound, const char * path)
{
	
	sound = LoadSound(path); 
	if (sound.stream.buffer == 0)
	{
		printf("Failed to load sound %s\n", path);
		return false;
	}
	return true;
	
}


void DrawTextShadow(const char *text, int posX, int posY, int fontSize, Color color, Color shadow_color, int shadow_distance)
{
	DrawText(text, posX + shadow_distance, posY + shadow_distance, fontSize, shadow_color);
	DrawText(text, posX, posY, fontSize, color);
}



bool random_chance(int chance) // 0 - 100
{
	if (chance <= 0) return false;
	return ( GetRandomValue(1, 100) <= chance);
}
