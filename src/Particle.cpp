#include "Particle.h"
#include "globals.h"
#include <stdio.h>

Particle::Particle(float x, float y, float angle, float speed, int lifeTime, float size, const Color & color): GameObject(x, y, angle),
	lifeTime(lifeTime), totalLifeTime(lifeTime), size(size), color(color)
{
	accelerate_angular(angle, speed);
}


void Particle::update()
{
	
	GameObject::update();
	
	lifeTime--;
	if (lifeTime <= 0)
		destroy();
	
}



void Particle::draw()
{
	
	const float factor = (float)lifeTime / (float)totalLifeTime;
	
	const float src_width = particleTexture.width;
	const float src_height = particleTexture.height;
	
	const float draw_width = factor * size;
	const float half_width = draw_width * 0.5f;
	
	DrawTexturePro(particleTexture, Rectangle { 0.0f, 0.0f, src_width, src_height }, 
									Rectangle { position.x - half_width, position.y - half_width, draw_width, draw_width }, 
									Vector2 { 0.0f, 0.0f }, 0.0f, color);
	
}
