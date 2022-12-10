#include "GameManager.h"
#include "globals.h"
#include "raylib.h"
#include "lists.h"
#include "item_data.h"
#include "utilities.h"
#include "utilities_raylib.h"

#include <math.h>
#include <string>

//----------- DRAW
void GameManager::draw()
{
	
	// Render all screen to texture (for scaling)
    BeginTextureMode(target);
		
        ClearBackground(BLACK);
		
		if (gameState == GAMESTATE::MENU)
			draw_main_menu();
		
		if (gameState == GAMESTATE::ENDING)
			draw_victory_screen();
		
		if (gameState == GAMESTATE::PLAYING)
		{
			draw_game_world();
			draw_hud();
		}
        
    EndTextureMode();
    
	
    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw render texture to screen scaled as required
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)target.texture.width*screenScale, (float)target.texture.height*screenScale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
		
        // Draw equivalent mouse position on the target render-texture
        //DrawCircleLines(GetMouseX(), GetMouseY(), 10, MAROON);
		
        // TODO: Draw everything that requires to be drawn at this point:
		
    EndDrawing();
	
	
}

/*
void GameManager::draw_map()
{
	
	GameMap & current_map = dungeon.get_current_floor();
	
	for (int y = 0; y < current_map.height; y++)
	{
		for (int x = 0; x < current_map.width; x++)
		{
			
			//DrawTexture(floorTexture, x * TW, y * TW, RAYWHITE);
			const int tile_id = current_map.gameTile[y][x];
			if (tile_id > TILE::_VOID_)
			{
				DrawTextureRec(tilesTexture, (Rectangle){ (float)( (tile_id - 1) * TW), 0.0f, TW, TW }, (Vector2) { (float)(x * TW), (float)(y * TW) }, RAYWHITE); 
			}
			
		}
	}
}*/



void GameManager::draw_game_world()
{
	
	BeginMode2D(camera);
	
	//draw_map();
	
	/*
	// draw items
	if (item.active)
		item.draw();
	
	// draw enemies
	if (enemy.active)
		enemy.draw();*/
	
	dungeon.draw();
	
	if (player.active)
		player.draw();
	
	for (Projectile & current_projectile : projectile)
	{
		if (current_projectile.active)
			current_projectile.draw();
	}
	
	for (Particle & current_particle : particle)
	{
		if (current_particle.active)
			current_particle.draw();
	}
	
	for (DamageMarker & current_damageMarker : damageMarker)
	{
		if (current_damageMarker.active)
			current_damageMarker.draw();
	}
	
	
	EndMode2D();
	
	
	if (escape_sequence_activated() )
	{
		// efeito da tela ficando vermelha
		unsigned char int_value = (unsigned char)(redTint * 255.0f);
		DrawRectangle(0, 0, screenWidth, screenHeight, Color {int_value, 0, 0, int_value } ); 
	}
	
}


void GameManager::draw_bar(float x, float y, double hp, double max_hp, const Color & tint)
{
	
	const float sprite_width = barTexture.width;
	const float sprite_height = barTexture.height;
	
	// fundo
	DrawTextureRec(barBackgroundTexture, (Rectangle){ 0.0f, 0.0f, sprite_width, sprite_height }, (Vector2) { x, y }, RAYWHITE);
	
	if (hp > 0)
	{
		
		if (hp > max_hp) hp = max_hp;
		const float factor = (float)(hp / max_hp);
		const float blit_size = factor * sprite_width;
		
		DrawTextureRec(barTexture, (Rectangle){ 0.0f, 0.0f, blit_size, sprite_height }, (Vector2) { x, y }, tint);
		
	}
	
	std::string str = std::to_string( (int)hp) + " / " + std::to_string( (int)max_hp);
	DrawTextShadow(str.c_str(), x + 12, y, 8, RAYWHITE, BLACK, 1 );
	
}



void GameManager::draw_hud()
{
	
	// texto logo abaixo do jogador
	// se estiver encostando em um item e sem espaco, escreve que da pra trocar
	{
		
		const float text_x = screenWidth * 0.5f;
		const float text_y = screenHeight * 0.5f + 28.0f;
		
		GameMap & current_map = dungeon.get_current_floor();
		for (Item & current_item : current_map.item)
		{
			
			if (current_item.active && player.collision(current_item) && player.cannot_autopick_item(current_item) )
			{
				DrawTextShadow("Press 'x' to swap items", text_x - 60.0f, text_y, 8, RAYWHITE, BLACK, 1 );
				break;
			}
			
		}
		
		// dica de tecla pra descer escadas
		if (dungeon.currentFloorIndex == 0)
		{
			const int player_tile = get_tile(player);
			if (player_tile == TILE::STAIRS_DOWN)
				DrawTextShadow("Press 'space' to climb stairs", text_x - 70.0f, text_y, 8, RAYWHITE, BLACK, 1 );
		}
	
	}
	
	//----
	
	// ----- barras de hp, mana e exp
	const float hud_spacing = 10.0f;
	float hud_y = 10.0f;
	
	draw_bar(10.0f, hud_y, player.HP, player.maxHP, RED); hud_y += hud_spacing;
	
	if (player.maxMana > 0)
	{
		draw_bar(10.0f, hud_y, player.mana, player.maxMana, BLUE); hud_y += hud_spacing;
	}
	
	draw_bar(10.0f, hud_y, player.experience, player.maxExperience, GREEN); hud_y += hud_spacing;
	
	
	// icones ao lado das barras
	hud_y = 5.0f;
	const float sprite_width = hudIconsTexture.height;
	const float icon_x = 10.0f - sprite_width * 0.5f;
	
	DrawTextureRec(hudIconsTexture, (Rectangle){ 0.0f, 0.0f, sprite_width, sprite_width }, (Vector2) { icon_x, hud_y }, RAYWHITE); hud_y += hud_spacing;
	
	if (player.maxMana > 0)
	{
		DrawTextureRec(hudIconsTexture, (Rectangle){ sprite_width, 0.0f, sprite_width, sprite_width }, (Vector2) { icon_x, hud_y }, RAYWHITE); hud_y += hud_spacing;
	}
	
	DrawTextureRec(hudIconsTexture, (Rectangle){ sprite_width * 2.0f, 0.0f, sprite_width, sprite_width }, (Vector2) { icon_x, hud_y }, RAYWHITE); hud_y += hud_spacing;
	
	// ----
	// money
	std::string coins_string = "Coins: " + std::to_string(player.coins);
	DrawTextShadow(coins_string.c_str(), 10, hud_y + 10, 8, YELLOW, BLACK, 1 );
	
	std::string arrows_string = "Arrows: " + std::to_string(player.arrows);
	DrawTextShadow(arrows_string.c_str(), 10, hud_y + 20, 8, GREEN, BLACK, 1 );
	//----
	
	// armadura
	if (player.armorId != 0)
	{
		
		if (Texture2D * sprite = get_item_sprite(player.armorId) )
			DrawTextureRec(*sprite, (Rectangle){ 0.0f, 0.0f, TW, TW }, (Vector2) { 10, hud_y + 40 }, Color {255, 255, 255, 204 } ); 
		
		std::string armor_string = "AC: " + std::to_string(player.get_AC() );
		DrawTextShadow(armor_string.c_str(), 10, hud_y + 45 + 32, 8, RAYWHITE, BLACK, 1 );
		
	}
	
	draw_inventory();
	
	// marcador de andar
	std::string str = "Floor " + std::to_string(dungeon.currentFloorIndex + 1);
	DrawTextShadow(str.c_str(), 256 - 80, 10, 16, RAYWHITE, BLACK, 2);
	
	// mensagem quando morto
	if (!player.active)
	{
		
		DrawTexture(youdiedTexture, (screenWidth - youdiedTexture.width) / 2, (screenHeight - youdiedTexture.height) / 2, RAYWHITE);
		DrawTextShadow("Press 'enter' to restart", screenWidth * 0.5f - 90.0f, screenHeight * 0.5f + 50.0f, 16, RAYWHITE, BLACK, 1);
	
	}
	
	// tela de level up
	if (player.pointsToDistribute > 0)
		draw_level_up_menu();
	
	
	// contador de tempo na sequencia de fuga
	if (escape_sequence_activated() )
	{
		
		std::string count_str = std::to_string(escapeTimer / 60);
		
		Color color = escapeTimer <= 10 * 60 ? RED : RAYWHITE;
		DrawTextShadow(count_str.c_str(), screenWidth * 0.5f - 20.0f, 30.0f, 16, color, BLACK, 1);
		
		
		// seta indicando a escada
		int x = 0, y = 0;
		if (dungeon.find_current_upstairs(x, y) )
		{
			
			float angle = angle_between_points(player.position.x, player.position.y, x * TW + TW2, y * TW + TW2) + camera.rotation;
			float rad_angle = degrees_to_radians(angle);
			
			const float distance = TW;
			float draw_x = screenWidth * 0.5f + cos(rad_angle) * distance;
			float draw_y = screenHeight * 0.5f + sin(rad_angle) * distance;
			
			const float src_width = selectionArrowTexture.width;
			const float src_height = selectionArrowTexture.height;
			const float half_width = src_width * 0.5f;
			const float half_height = src_height * 0.5f;
			
			DrawTexturePro(selectionArrowTexture, Rectangle { 0.0f, 0.0f, src_width, src_height }, Rectangle { draw_x, draw_y, src_width, src_height }, Vector2 { 2.0f, half_height }, angle, Color {255, 255, 255, 191 } );
			
		}
		
	}
	
	
	if (dungeon.on_boss_floor() )
	{
		
		if (Enemy * boss = dungeon.find_boss() )
		{
			draw_bar(screenWidth * 0.5f - 33.0f, 10.0f, boss->HP, boss->get_max_HP(), RED);
		}
		
	}
	
}



void GameManager::draw_inventory()
{
	
	const Color translucent_color = { 255, 255, 255, 32 };
	const Color translucent_color2 = { 255, 255, 255, 178 };
	
	float x = 24.0f;
	float y = screenHeight - TW - 8.0f;
	
	for (int i = 0; i < MAX_INVENTORY_SLOTS; i++)
	{
		
		const bool is_selected_slot = player.selectedInventorySlot == i;
		
		DrawTextureEx(squareTexture, Vector2 {x, y} , 0.0f, 1.0f, is_selected_slot ? translucent_color2 : translucent_color);
		
		const int item_id = player.inventory[i].id;
		if (item_id > 0)
		{
			if (Texture2D * sprite = get_item_sprite(item_id) )
				DrawTextureEx(*sprite, Vector2 {x, y} , 0.0f, 1.0f, is_selected_slot ? RAYWHITE : translucent_color2);
		}
		
		// texto indicando atalho
		DrawTextShadow( std::to_string(i+1).c_str(), x, y + 24, 8, RAYWHITE, BLACK, 1);
		
		// quandidade
		if (player.inventory[i].amount > 1)
		{
			const float offset = player.inventory[i].amount < 10 ? 22 : 16;
			DrawTextShadow( std::to_string(player.inventory[i].amount).c_str(), x + offset, y + 24, 8, YELLOW, BLACK, 1);
		}
		
		x += TW + 4.0f;
		
	}
	
}


void GameManager::draw_level_up_menu()
{
	
	// fundo
	DrawTexture(levelUpMenuTexture, levelUpMenuPosition, 0.0f, RAYWHITE);
	
	// texto
	std::string level_str = "Level " + std::to_string(player.level);
	
	DrawTextShadow("Your are now ", levelUpMenuPosition + 80, 20, 16, RAYWHITE, BLACK, 2 );
	DrawTextShadow(level_str.c_str(), levelUpMenuPosition + 110, 40, 16, RAYWHITE, BLACK, 2 );
	
	// contador de atributos
	const float text_x = levelUpMenuPosition + 72.0f;
	const float base_text_y = 98.0f;
	
	//if (player.strength > 0)
		DrawTextShadow( std::to_string(player.strength).c_str(), text_x + (player.strength < 10 ? 5.0f : 0.0f), base_text_y + 0.0f * 34.0f, 8, RAYWHITE, BLACK, 1);
	
	//if (player.dexterity > 0)
		DrawTextShadow( std::to_string(player.dexterity).c_str(), text_x + (player.dexterity < 10 ? 5.0f : 0.0f), base_text_y + 1.0f * 34.0f, 8, RAYWHITE, BLACK, 1);
	
	//if (player.vitality > 0)
		DrawTextShadow( std::to_string(player.vitality).c_str(), text_x + (player.vitality < 10 ? 5.0f : 0.0f), base_text_y + 2.0f * 34.0f, 8, RAYWHITE, BLACK, 1);
	
	//if (player.magic > 0)
		DrawTextShadow( std::to_string(player.magic).c_str(), text_x + (player.magic < 10 ? 5.0f : 0.0f), base_text_y + 3.0f * 34.0f, 8, RAYWHITE, BLACK, 1);
	
	// selecao
	if (levelUpMenuPosition <= 0)
		DrawTexture(selectionArrowTexture, 30, 80 + levelUpMenuCursorPosition * 34, RAYWHITE );
	
}



void GameManager::draw_main_menu()
{
	
	DrawTexture(splashScreenTexture, 0, 0, RAYWHITE);  

	DrawTextShadow( "Press 'enter' to start", screenWidth * 0.5f - 80.0f, screenHeight * 0.5f + 10.0f, 16, RAYWHITE, BLACK, 1);	
	
	DrawTextShadow( "a game by Space.cpp", 5.0f, screenHeight - 16.0f - 5.0f, 16, RAYWHITE, BLACK, 1);	
	
}



void GameManager::draw_victory_screen()
{
	
	DrawTexture(endingTexture, 0, 0, RAYWHITE);  
	
	DrawTextShadow( "Congratulations!", screenWidth * 0.5f - 65.0f, 20.0f, 16, RAYWHITE, BLACK, 1);
	DrawTextShadow( "You defeated the guardian ", screenWidth * 0.5f - 100.0f, 40.0f, 16, RAYWHITE, BLACK, 1);
	DrawTextShadow( "of the dungeon", screenWidth * 0.5f - 65.0f, 60.0f, 16, RAYWHITE, BLACK, 1);
	DrawTextShadow( "and escaped alive!", screenWidth * 0.5f - 70.0f, 80.0f, 16, RAYWHITE, BLACK, 1);
	
	const float text_x = 20.0f;
	float text_y = 120.0f;
	
	std::string level_str = "You were level " + std::to_string(player.level);
	DrawTextShadow( level_str.c_str(), text_x, text_y, 16, RAYWHITE, BLACK, 1);
	text_y += 20.0f;
	
	std::string coins_str = "Collected " + std::to_string(player.coins) + " coins";
	DrawTextShadow( coins_str.c_str(), text_x, text_y, 16, RAYWHITE, BLACK, 1);
	text_y += 20.0f;
	
	std::string kills_str = "Killed " + std::to_string(player.kills) + " enemies";
	DrawTextShadow( kills_str.c_str(), text_x, text_y, 16, RAYWHITE, BLACK, 1);
	text_y += 40.0f;
	
	DrawTextShadow( "Press 'enter'", screenWidth * 0.5f - 50.0f, text_y, 16, RAYWHITE, BLACK, 1);	
	
}
