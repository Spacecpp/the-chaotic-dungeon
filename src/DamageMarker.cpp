#include "DamageMarker.h"
#include <string>

DamageMarker::DamageMarker(float x, float y, int value): GameObject(x, y), value(value), timer(80)
{
	this->angle = 270.0f + GetRandomValue(-45.0f, 45.0f);
	accelerate_angular(this->angle, GetRandomValue(2.0f, 3.0f) );
}

void DamageMarker::update()
{
	
	GameObject::update();
	velocity.y += 0.1f;
	
	timer--;
	if (timer <= 0)
		destroy();
	
}


void DamageMarker::draw()
{
	
	DrawText( std::to_string(value).c_str(), position.x, position.y, 16, RAYWHITE );
	
}
