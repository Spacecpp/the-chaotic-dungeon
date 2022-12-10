#ifndef DAMAGEMARKER_H
#define DAMAGEMARKER_H

#include "GameObject.h"

class DamageMarker: public GameObject
{
	
	private:
	
	public:
		int value;
		int timer;
		
		DamageMarker(float x, float y, int value);
		
		void update();
		void draw();

};
#endif
