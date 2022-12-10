#ifndef DUNGEON_H
#define DUNGEON_H


#include "GameMap.h"
class Enemy;

class Dungeon
{
	
	public:
		int currentFloorIndex;
		std::vector<GameMap> gameMap;
		
		
		Dungeon();
		
		void reset();
		void advance_floor();
		bool go_back_floor();
		void generate_next_floor();
		
		void go_to_floor(int floor);
		
		GameMap & get_current_floor();
		bool find_current_upstairs(int & x, int & y);
		bool find_current_downstairs(int & x, int & y);
		bool on_boss_floor();
		Enemy * find_boss();
		
		Item * create_item(const Item & O);
		
		void update();
		void update_collision();
		
		void draw();
		//void draw_objects();
	
};
#endif



