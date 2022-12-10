#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <vector>
#include "Enemy.h"
#include "Item.h"

class GameMap
{
	
	private:
	
	public:
		class RoomData
		{
			
			public:
				int x, y, width, height;
				uint8_t type;
				bool connectedToUpstairs;
				//std::vector<RoomData*> connection;
				
				RoomData(): x(0), y(0), width(0), height(0), type(0), connectedToUpstairs(false)
				{
				}
				
				int get_left() { return x; }
				int get_top() { return y; }
				int get_right() { return x + width - 1; }
				int get_bottom() { return y + height - 1; }
				
				int distance(const RoomData & other)
				{
					
					int left1 = this->x;
					int right1 = left1 + this->width - 1;
					int top1 = this->y;
					int bottom1 = top1 + this->height - 1;
					
					int left2 = other.x;
					int right2 = left2 + other.width - 1;
					int top2 = other.y;
					int bottom2 = top2 + other.height - 1;
					
					
					int distance_h = 0, distance_v = 0;
					
					if (left1 > right2)
						distance_h = left1 - right2;
					
					else if (left2 > right1)
						distance_h = left2 - right1;
					
					
					if (top1 > bottom2)
						distance_v = top1 - bottom2;
					
					else if (top2 > bottom1)
						distance_h = top2 - bottom1;
					
					return distance_h + distance_v;
					
				}
			
		};
		
		int width, height;
		int depth;
		std::vector< std::vector<int> > gameTile;
		std::vector<RoomData> room;
		
		std::vector<Enemy> enemy;
		std::vector<Item> item;
		
		
		GameMap();
		void resize(int w, int h);
		bool load(const char * filename);
		
		bool coordinates_out_of_bounds(int x, int y);
		bool tile_is_solid(int x, int y);
		int get_tile_world_position(float x, float y);
		void set_tile(int x, int y, int id);
		void dig_tile(int x, int y);
		
		void generate_random(int width, int height, int room_count, int depth);
		void generate_final_floor();
		
		bool find_upstairs(int & x, int & y);
		bool find_downstairs(int & x, int & y);
		
		void connect_rooms(RoomData & room1, RoomData & room2);
		
		
		void update();
		void update_collision();
		
		void draw();
		void draw_tiles();
		void draw_items();
		void draw_objects();
		
		Enemy * create_enemy(const Enemy & O);
		Item * create_item(const Item & O);

};
#endif
