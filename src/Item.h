#ifndef ITEM_H
#define ITEM_H

#include "GameObject.h"

class Item: public GameObject
{
	
	public:
		bool is_chest;
		int id;
		int amount;
		
		Item();
		Item(float x, float y, int id, int amount, bool is_chest);
		
		void draw();
		
		float get_radius();
		
		bool is_armor();
		bool is_stackable();
	
};
#endif
