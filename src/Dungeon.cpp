#include "Dungeon.h"
#include "lists.h"
#include "Enemy.h"

Dungeon::Dungeon(): currentFloorIndex(0)
{
	
}


void Dungeon::reset()
{
	gameMap.clear();
	currentFloorIndex = 0;
}



void Dungeon::advance_floor()
{
	
	currentFloorIndex++;
	if (currentFloorIndex > (int)gameMap.size() )
		generate_next_floor();
	
}

bool Dungeon::go_back_floor()
{
	
	if (currentFloorIndex > 0)
	{
		currentFloorIndex--;
		return true;
	}
	return false;
}


void Dungeon::generate_next_floor()
{
	
	gameMap.push_back(GameMap() );
	
	const int current_depth = gameMap.size();
	
	if (current_depth == FINAL_FLOOR)
	{
		gameMap.back().generate_final_floor();
	}
	
	else
	{
		
		const int map_size = 20 + (current_depth - 1) * 2;
		const int room_count = 6 + (current_depth - 1);
		gameMap.back().generate_random(map_size, map_size, room_count, current_depth);
		
	}
	
}


void Dungeon::go_to_floor(int floor)
{
	
	if (floor >= 1)
	{
		
		currentFloorIndex = floor - 1;
		
		while (currentFloorIndex > (int)gameMap.size() )
			generate_next_floor();
		
	}
	
}


GameMap & Dungeon::get_current_floor()
{
	/*
	if (currentFloorIndex >= 0 && currentFloorIndex < gameMap.size() )
		return gameMap[currentFloorIndex];
	return NULL;
	*/
	
	// se nao tem andar valido, cria um
	if (currentFloorIndex < 0)
		currentFloorIndex = 0;
		
	while (currentFloorIndex >= (int)gameMap.size() )
		generate_next_floor();
	
	return gameMap[currentFloorIndex];
		
}


bool Dungeon::find_current_upstairs(int & x, int & y)
{
	
	GameMap & current_map = get_current_floor();
	return current_map.find_upstairs(x, y);
	//return false;
	
}

bool Dungeon::find_current_downstairs(int & x, int & y)
{
	
	GameMap & current_map = get_current_floor();
	return current_map.find_downstairs(x, y);
	//return false;
	
}


bool Dungeon::on_boss_floor()
{
	return (currentFloorIndex + 1 == FINAL_FLOOR);
}


Enemy * Dungeon::find_boss()
{
	
	GameMap & current_map = get_current_floor();
	
	for (Enemy & current_enemy : current_map.enemy)
	{
		if (current_enemy.active && current_enemy.id == ENEMY::BOSS)
			return &current_enemy;
	}
	
	return NULL;
	
}


Item * Dungeon::create_item(const Item & O)
{
	GameMap & current_map = get_current_floor();
	return current_map.create_item(O);
}


void Dungeon::update()
{
	
	GameMap & current_map = get_current_floor();
	current_map.update();
	
}


void Dungeon::update_collision()
{
	GameMap & current_map = get_current_floor();
	current_map.update_collision();
}


void Dungeon::draw()
{
	GameMap & current_map = get_current_floor();
	current_map.draw();
}

/*
void Dungeon::draw_objects()
{
	GameMap & current_map = get_current_floor();
	current_map.draw_objects();
}*/

