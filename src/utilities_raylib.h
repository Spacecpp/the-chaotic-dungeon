#ifndef UTILITIES_RAYLIB_H
#define UTILITIES_RAYLIB_H

#include "raylib.h"

bool load_texture(Texture2D & texture, const char * path);
bool load_sound(Sound & sound, const char * path);

void DrawTextShadow(const char *text, int posX, int posY, int fontSize, Color color, Color shadow_color, int shadow_distance);

bool random_chance(int chance);

#endif
