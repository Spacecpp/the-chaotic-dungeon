#ifndef PARTICLE_H
#define PARTICLE_H

#include "GameObject.h"

class Particle: public GameObject
{
	
	private:
	
	public:
		int lifeTime, totalLifeTime;
		float size;
		Color color;
		
		Particle(float x, float y, float angle, float speed, int lifeTime, float size, const Color & color);
		
		void update();
		void draw();


};
#endif
