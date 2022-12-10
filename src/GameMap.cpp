#include "GameMap.h"
#include "FileHandler.h"
//#include "utilities.h"
#include "lists.h"
#include "item_data.h"
#include "globals.h"
#include "utilities.h"
#include "utilities_raylib.h"
#include "raylib.h"

GameMap::GameMap(): width(0), height(0)
{
}



void GameMap::resize(int w, int h)
{
	
	width = w;
	height = h;
	
	gameTile.resize(h);
	for (std::vector<int> & row : gameTile)
	{
		row.resize(w, 0);
	}
	
}


bool GameMap::load(const char * filename)
{
	
	FileHandler file;
	if (file.open(filename, false) )
	{
		
		int w = 0, h = 0;
		
		file.read_variable(w);
		file.read_variable(h);
		
		if (w > 0 && h > 0)
		{
			
			resize(w, h);
			
			for (int y = 0; y < h; y++)
			{
				for (int x = 0; x < w; x++)
				{
					
					file.read_variable(gameTile[y][x]);
					
				}
				
			}
		
		}
		
		file.close();
		
		return true;
		
	}
	
	return false;
	
}


bool GameMap::coordinates_out_of_bounds(int x, int y)
{
	return (x < 0 || y < 0 || x >= width || y >= height);
}


bool GameMap::tile_is_solid(int x, int y)
{
	if (!coordinates_out_of_bounds(x, y) )
		return gameTile[y][x] == TILE::WALL;
	return true;
}



int GameMap::get_tile_world_position(float x, float y)
{
	
	const int tile_x = (int)(x / TW);
	const int tile_y = (int)(y / TW);
	
	if (!coordinates_out_of_bounds(tile_x, tile_y) )
		return gameTile[tile_y][tile_x];
	
	return 0;
	
}


void GameMap::set_tile(int x, int y, int id)
{
	
	if (!coordinates_out_of_bounds(x, y) )
		gameTile[y][x] = id;
	
}



// optimize this later
void GameMap::dig_tile(int x, int y)
{
	
	for (int y1 = y - 1; y1 <= y + 1; y1++)
	{
		
		for (int x1 = x - 1; x1 <= x + 1; x1++)
		{
			
			if (!coordinates_out_of_bounds(x1, y1) )
			{
				
				int & current_tile = gameTile[y1][x1];
				
				if (x1 == x && y1 == y) // cavando
				{
					if (current_tile == TILE::WALL || current_tile == TILE::_VOID_) 
						current_tile = TILE::FLOOR;
				}
				
				else if (current_tile == TILE::_VOID_) // preenche com paredes ao redor
					current_tile = TILE::WALL;
			}
			
		}
	
	}
	
}



void GameMap::generate_random(int width, int height, int room_count, int depth)
{
	
	this->depth = depth;
	
	//printf("depth = %d ", depth);
	
	resize(width, height);
	
	room.clear();
	room.resize(room_count);
	
	// cria os dados de cada sala
	for (int i = 0; i < room_count; i++)
	{
		
		room[i].x = GetRandomValue(1, width - 3);
		room[i].y = GetRandomValue(1, height - 3);
		room[i].width = GetRandomValue(2, width / 3);
		room[i].height = GetRandomValue(2, height / 3);
		
		// impede overflow
		if (room[i].x + room[i].width > width - 2)
			room[i].width = std::max(width - 2 - room[i].x, 2);
		
		if (room[i].y + room[i].height > height - 2)
			room[i].height = std::max(height - 2 - room[i].y, 2);
		
		room[i].type = 0;
		room[i].connectedToUpstairs = false;
		
	}
	
	// gera os tiles
	for (int i = 0; i < room_count; i++)
	{
		
		const int first_x = room[i].x, first_y = room[i].y, last_x = room[i].x + room[i].width - 1, last_y = room[i].y + room[i].height - 1;
		
		for (int y = first_y - 1; y <= last_y + 1; y++)
		{
			for (int x = first_x - 1; x <= last_x + 1; x++)
			{
				
				if (!coordinates_out_of_bounds(x, y) )
				{
					
					int & current_tile = gameTile[y][x];
					
					bool is_floor = (x >= first_x && x <= last_x && y >= first_y && y <= last_y);
					if (is_floor)
						current_tile = TILE::FLOOR;
					
					else if (current_tile == TILE::_VOID_)
						current_tile = TILE::WALL;
					
				}
				
			}
			
		}
		
	}
	
	// posiciona as escadas, tentando posicionar o mais distante possivel uma da outra
	// escada pra cima
	int upstairs_room_index = GetRandomValue(0, room_count - 1);
	RoomData & upstairs_room = room[upstairs_room_index];
	
	{
		int x = GetRandomValue(upstairs_room.x, upstairs_room.x + upstairs_room.width - 1);
		int y = GetRandomValue(upstairs_room.y, upstairs_room.y + upstairs_room.height - 1);
		gameTile[y][x] = TILE::STAIRS_UP;
		upstairs_room.type = 1;
		upstairs_room.connectedToUpstairs = true;
		
	}
	
	// ---- cria escada pra baixo
	int best_distance = 0;
	RoomData * best_room = &room[0];
	
	for (RoomData & current_room : room)
	{
		
		//int distance = abs(current_room.x - upstairs_room.x) + abs(current_room.y - upstairs_room.y);
		int distance = upstairs_room.distance(current_room);
		//printf ("%d\n", distance);
		
		if (distance > best_distance)
		{
			best_distance = distance;
			best_room = &current_room;
		}
		
	}
	//printf ("best %d\n", best_distance);
	
	if (best_room)
	{
		
		int x, y;
		do 
		{
			x = GetRandomValue(best_room->x, best_room->x + best_room->width - 1);
			y = GetRandomValue(best_room->y, best_room->y + best_room->height - 1);
		} while (gameTile[y][x] == TILE::STAIRS_UP);
		
		gameTile[y][x] = TILE::STAIRS_DOWN;
		best_room->type = 2;
		
	}
	// ----------------
	
	// ---- cria corredores ligando salas orfas
	// marca quais salas estao mescladas com a sala da escada pra cima
	for (RoomData & current_room : room)
	{
		
		if (&current_room != &upstairs_room)
		{
			const int distance = upstairs_room.distance(current_room);
			if (distance <= 0)
				current_room.connectedToUpstairs = true;
		}
		
	}
	
	// liga salas desconectadas em alguma que for conectada
	for (RoomData & current_room : room)
	{
		
		if (!current_room.connectedToUpstairs)
		{
			
			// busca a sala conectada mais proxima
			int best_distance = -1;
			RoomData * best_room = NULL;
			
			for (RoomData & current_room2 : room)
			{
				
				if (&current_room != &current_room2 && current_room2.connectedToUpstairs)
				{
					
					const int distance = current_room.distance(current_room2);
					if (distance < best_distance || best_distance <= -1)
					{
						
						best_distance = distance;
						best_room = &current_room2;
					}
					
				}
				
			}
			
			// se achou a sala, conecta
			if (best_room)
			{
				
				connect_rooms(current_room, *best_room);
				current_room.connectedToUpstairs = true;
				
			}
			else printf("Failed to choose room to connect to\n");
			
		}
		
	}
	
	
	// cria inimigos 
	
	/*
	int min_enemy_id = 0, max_enemy_id = ENEMY::COUNT;
	
	if (depth <= 2)
	{
		min_enemy_id = ENEMY::RAT;
		max_enemy_id = ENEMY::SLIME;
	}*/
	
	
	//const int enemiesPerRoom = depth >= 21 ? 2 : 1;
	int enemiesPerRoom = 1;
	
	if (depth >= 16)
		enemiesPerRoom = 2;
	
	if (depth >= 26)
		enemiesPerRoom = 3;
	
	std::vector<int> validEnemies;
	
	if (depth <= 4)
		validEnemies.push_back(ENEMY::RAT);
	
	if (depth <= 6)
		validEnemies.push_back(ENEMY::SLIME);
	
	if (depth >= 3 && depth <= 12)
		validEnemies.push_back(ENEMY::GOBLIN);
	
	if (depth >= 4)
		validEnemies.push_back(ENEMY::IMP);		
	
	if (depth >= 4 && depth <= 15)
		validEnemies.push_back(ENEMY::SKELETON);
	
	if (depth >= 5)
		validEnemies.push_back(ENEMY::SKELETON_ARCHER);
	
	if (depth >= 10 && depth <= 20)
		validEnemies.push_back(ENEMY::GHOST);
	
	if (depth >= 15)
		validEnemies.push_back(ENEMY::SKELETON_WARRIOR);
	
	if (depth >= 20)
	{
		validEnemies.push_back(ENEMY::DEATH_BLADE);
		validEnemies.push_back(ENEMY::DEATH_WIZARD);
	}
	
	if (depth >= 22)
		validEnemies.push_back(ENEMY::DRAGON);
	
	if (depth >= 26)
		validEnemies.push_back(ENEMY::REAPER);
	
	if (depth >= 8 && depth <= 25)
		validEnemies.push_back(ENEMY::MIMIC);
	
	
	if (!validEnemies.empty() )
	{
		
		for (RoomData & current_room : room)
		{
			
			if (current_room.type != 1) // nunca gera inimigos na sala da escada pra cima
			{
				
				for (int i = 0; i < enemiesPerRoom; i++)
				{
					
					int x = GetRandomValue(current_room.x, current_room.x + current_room.width - 1);
					int y = GetRandomValue(current_room.y, current_room.y + current_room.height - 1);
					
					//int id = GetRandomValue(min_enemy_id, max_enemy_id);
					int index = GetRandomValue(0, validEnemies.size() - 1);
					int id = validEnemies[index];
					
					float fx = (x * TW) + TW2;
					float fy = (y * TW) + TW2;
					
					if (Enemy * e = create_enemy( Enemy(fx, fy, id) ) )
					{
						if (depth >= 21 && e->id != ENEMY::MIMIC)
						{
							if (random_chance(50) )
								e->state = ENEMY_STATE::WANDERING;
						}
					}
				
				}
			
			}
			
		}
	
	}
	
	//gameTile[0][0] = 1;
	
	// cria baus com items
	int items_created = 0, max_items = depth;
	
	
	std::vector<int> validItems;
	
	validItems.push_back(ITEM::COINS);
	
	// ammo
	if (depth <= 7)
		validItems.push_back(ITEM::ARROW);
		
	if (depth >= 3)
		validItems.push_back(ITEM::ARROW_QUIVER);
	
	// armor
	if (depth >= 2 && depth <= 5)
		validItems.push_back(ITEM::LEATHER_ARMOR);
	
	if (depth >= 5 && depth <= 10)
		validItems.push_back(ITEM::CHAIN_MAIL);
	
	if (depth >= 12 && depth <= 18)
		validItems.push_back(ITEM::DRAGON_LEATHER_ARMOR);
	
	if (depth >= 19 && depth <= 25)
		validItems.push_back(ITEM::PLATE_MAIL);
	
	if (depth >= 26 && depth <= 28)
		validItems.push_back(ITEM::GOLDEN_PLATE);
	
	// potion
	if (depth >= 4)
		validItems.push_back(ITEM::HP_POTION);
	
	if (depth >= 6)
		validItems.push_back(ITEM::MANA_POTION);
	
	if (depth >= 7 && depth <= 15)
	{
		validItems.push_back(ITEM::PISS_POTION);
		validItems.push_back(ITEM::POISON_POTION);
	}
	
	if (depth >= 12 || depth == 6)
		validItems.push_back(ITEM::EXPLOSION_POTION);
	
	if (depth >= 25)
		validItems.push_back(ITEM::EXPERIENCE_POTION);
	
	//----
	if (depth >= 3 && depth <= 9)
		validItems.push_back(ITEM::SHORT_BOW);
	
	if (depth >= 4 && depth <= 8)
	{
		validItems.push_back(ITEM::LONG_SWORD);
		validItems.push_back(ITEM::BOOK_FIREBOLT);
	}
	
	if (depth >= 5 && depth <= 9)
	{
		validItems.push_back(ITEM::BOOK_FROSTBOLT);
		validItems.push_back(ITEM::BOOK_LIGHTNING);
	}
	
	if (depth >= 10 && depth <= 20)
		validItems.push_back(ITEM::LONG_BOW);
	
	if (depth >= 15 && depth <= 24)
		validItems.push_back(ITEM::BASTARD_SWORD);
	
	if (depth >= 25 && depth <= 29)
		validItems.push_back(ITEM::GREAT_SWORD);
	
	if (depth >= 15 && depth <= 25)
	{
		validItems.push_back(ITEM::BOOK_FIREBALL);
		validItems.push_back(ITEM::BOOK_FREEZING_CLOUD);
	}
	
	if (depth >= 17 && depth <= 28)
		validItems.push_back(ITEM::BOOK_CHAIN_LIGHTNING);
	
	
	
	
	if (!validItems.empty() )
	{
		
		std::vector<RoomData*> availableRooms;
		
		for (int i = 0; i < max_items; i++)
		{
			
			if (availableRooms.empty() )
			{
				
				for (RoomData & current_room : room)
				{
					if (current_room.type == 0) // nunca gera items na sala da escada pra cima
						availableRooms.push_back(&current_room);
				}
				
			}
			
			if (availableRooms.empty() )
			{
				printf("Error finding rooms\n");
				break;
			}
			
			// escolhe uma sala aleatoriamente
			int room_index = GetRandomValue(0, availableRooms.size() - 1);
			RoomData & current_room = *availableRooms[room_index];
			
			int x = GetRandomValue(current_room.x, current_room.x + current_room.width - 1);
			int y = GetRandomValue(current_room.y, current_room.y + current_room.height - 1);
			//int id = GetRandomValue(1, ITEM::COUNT - 1);
			
			int item_index = GetRandomValue(0, validItems.size() - 1);
			int id = validItems[item_index];
			int amount = 1;
			
			if (item_is_stackable(id) )
			{
				
				if (depth >= 5 && depth <= 12)
					amount = GetRandomValue(1, 3);
				
				else if (depth >= 13 && depth <= 20)
					amount = GetRandomValue(1, 5);
				
				else if (depth >= 21)
					amount = GetRandomValue(1, 7);
				
			}
			
			if (id == ITEM::COINS)
				amount = GetRandomValue(depth, depth * 2);
			
			float fx = (x * TW) + TW2;
			float fy = (y * TW) + TW2;
			
			create_item( Item(fx, fy, id, amount, true) );
			
			items_created++;
			
			
			// remove a sala da lista
			availableRooms.erase( availableRooms.begin() + room_index);
		
		}
	
	}
	
	//printf("%d items created\n", items_created);
	
	//printf("\n");
	
}


void GameMap::generate_final_floor()
{
	
	resize(30, 30);
	
	room.clear();
	room.resize(1);
	
	
	room[0].x = 1;
	room[0].y = 1;
	room[0].width = width - 2;
	room[0].height = height - 2;
	
	room[0].type = 0;
	room[0].connectedToUpstairs = false;
	
	
	
	// gera os tiles
	const int first_x = room[0].x, first_y = room[0].y, last_x = room[0].x + room[0].width - 1, last_y = room[0].y + room[0].height - 1;
	
	for (int y = first_y - 1; y <= last_y + 1; y++)
	{
		for (int x = first_x - 1; x <= last_x + 1; x++)
		{
			
			if (!coordinates_out_of_bounds(x, y) )
			{
				
				int & current_tile = gameTile[y][x];
				
				bool is_floor = (x >= first_x && x <= last_x && y >= first_y && y <= last_y);
				if (is_floor)
					current_tile = TILE::FLOOR;
				
				else if (current_tile == TILE::_VOID_)
					current_tile = TILE::WALL;
				
			}
			
		}
		
	}
	
	
	// posiciona as escadas, tentando posicionar o mais distante possivel uma da outra
	// escada pra cima
	RoomData & upstairs_room = room[0];
	{
		int x = GetRandomValue(upstairs_room.x, upstairs_room.x + upstairs_room.width - 1);
		int y = GetRandomValue(upstairs_room.y, upstairs_room.y + upstairs_room.height - 1);
		gameTile[y][x] = TILE::STAIRS_UP;
		upstairs_room.type = 1;
		upstairs_room.connectedToUpstairs = true;
		
	}
	
	// cria o boss
	const float offset = 5;
	int x = GetRandomValue(upstairs_room.x + offset, upstairs_room.x + upstairs_room.width - 1 - offset);
	int y = GetRandomValue(upstairs_room.y + offset, upstairs_room.y + upstairs_room.height - 1 - offset);
	int id = ENEMY::BOSS;
	
	float fx = (x * TW) + TW2;
	float fy = (y * TW) + TW2;
	
	create_enemy( Enemy(fx, fy, id) );
	// ----
	
}



bool GameMap::find_upstairs(int & x, int & y)
{
	
	for (int y1 = 0; y1 < height; y1++)
	{
		for (int x1 = 0; x1 < width; x1++)
		{
			if (gameTile[y1][x1] == TILE::STAIRS_UP)
			{
				x = x1; y = y1;
				return true;
			}
		}
	}
	return false;
	
}



bool GameMap::find_downstairs(int & x, int & y)
{
	
	for (int y1 = 0; y1 < height; y1++)
	{
		for (int x1 = 0; x1 < width; x1++)
		{
			if (gameTile[y1][x1] == TILE::STAIRS_DOWN)
			{
				x = x1; y = y1;
				return true;
			}
		}
	}
	return false;
	
}



void GameMap::connect_rooms(RoomData & room1, RoomData & room2)
{
	
	// determina que direcao tem que cavar
	int dig_horizontal = 0, dig_vertical = 0;
	
	if (room1.get_right() < room2.get_left() )
		dig_horizontal = 1;
	
	else if (room2.get_right() < room1.get_left() )
		dig_horizontal = -1;
	
	if (room1.get_bottom() < room2.get_top() )
		dig_vertical = 1;
	
	else if (room2.get_bottom() < room1.get_top() )
		dig_vertical = -1;
	
	
	int x = -1, y = -1;
	
	while (dig_horizontal != 0 || dig_vertical != 0)
	{
	
		if (dig_horizontal != 0)
		{
			
			// coordenadas iniciais
			if (x == -1 && y == -1)
			{
				x = dig_horizontal == 1 ? room1.get_right() + 1 : room1.get_left() - 1;
				y = room1.y + room1.height / 2;
			}
			
			do
			{
				
				dig_tile(x, y);
				x += dig_horizontal;
				
			} while ( x < room2.get_left() || x > room2.get_right() );
			
			dig_horizontal = 0;
			
			if (y < room2.get_top() )
				dig_vertical = 1;
			
			else if (y > room2.get_bottom() )
				dig_vertical = -1;
			
		}
		
		if (dig_vertical != 0)
		{
			
			// seta as coordenadas iniciais caso nao tenha sido setado anteriormente
			if (x == -1 && y == -1)
			{
				x = room1.x + room1.width / 2;
				y = dig_vertical == 1 ? room1.get_bottom() + 1 : room1.get_top() - 1;
			}
			
			do
			{
				
				dig_tile(x, y);
				y += dig_vertical;
				
			} while ( y < room2.get_top() || y > room2.get_bottom() );
			
			dig_vertical = 0;
			
			if (x < room2.get_left() )
				dig_horizontal = 1;
			
			else if (x > room2.get_right() )
				dig_horizontal = -1;
			
		}
		
	}
	
}



void GameMap::update()
{
	
	for (Enemy & current_enemy : enemy)
	{
		if (current_enemy.active)
			current_enemy.update();
	}
	
}


void GameMap::update_collision()
{
	
	for (Enemy & current_enemy : enemy)
	{
		if (current_enemy.active)
			gameManager.resolve_character_collision(current_enemy);
	}
	
}


void GameMap::draw()
{
	
	draw_tiles();
	draw_objects();
	
}

void GameMap::draw_tiles()
{
	
	// visible region
	int first_x = (int)( (gameManager.player.position.x - screenWidth * 0.5f) / TW);
	int first_y = (int)( (gameManager.player.position.y - screenHeight * 0.5f) / TW);
	int last_x = first_x + 9;
	int last_y = first_y + 9;
	
	// compensacao devido a rotacao
	if (gameManager.escape_sequence_activated() )
	{
		first_x -= 2;
		first_y -= 2;
		last_x += 2;
		last_y += 2;
	}
	
	if (first_x < 0) first_x = 0;
	if (first_y < 0) first_y = 0;
	if (last_x >= this->width) last_x = this->width - 1;
	if (last_y >= this->height) last_y = this->height - 1;
	
	
	const float blit_y = ( round_up_( (float)depth / 5.0f) - 1.0f) * TW;
	
	for (int y = first_y; y <= last_y; y++)
	{
		for (int x = first_x; x <= last_x; x++)
		{
			
			const int tile_id = this->gameTile[y][x];
			if (tile_id > TILE::_VOID_)
			{
				
				const float draw_x = (float)(x * TW);
				const float draw_y = (float)(y * TW);
				
				// desenha o chao
				if (tile_id != TILE::WALL)
					DrawTextureRec(tilesTexture, (Rectangle){ 0.0f, blit_y, TW, TW }, (Vector2) { draw_x, draw_y }, RAYWHITE); 
				
				if (tile_id == TILE::WALL)
					DrawTextureRec(tilesTexture, (Rectangle){ TW, blit_y, TW, TW }, (Vector2) { draw_x, draw_y }, RAYWHITE); 
				
				
				if (tile_id == TILE::STAIRS_UP || tile_id == TILE::STAIRS_DOWN)
					DrawTextureRec(tilesTexture, (Rectangle){ (float)( (tile_id - 1) * TW), 0.0f, TW, TW }, (Vector2) { draw_x, draw_y }, RAYWHITE); 
			
			}
			
		}
	}
	
}


void GameMap::draw_objects()
{
	
	draw_items();
	
	for (Enemy & current_enemy : enemy)
	{
		if (current_enemy.active)
			current_enemy.draw();
	}
	
}


void GameMap::draw_items()
{
	
	for (Item & current_item : item)
	{
		if (current_item.active)
			current_item.draw();
	}
	
}



//-------

Enemy * GameMap::create_enemy(const Enemy & O)
{
	
	for (Enemy & object : enemy)
	{
		if (!object.active) { object = O; return &object; }
	}
	
	enemy.push_back(O);
	return &enemy.back();
	
}



Item * GameMap::create_item(const Item & O)
{
	
	for (Item & object : item)
	{
		if (!object.active) { object = O; return &object; }
	}
	
	item.push_back(O);
	return &item.back();
	
}
