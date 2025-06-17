#include "Bullet.h"

Bullet::Bullet(Vector3 position, Vector3 velocity, Entity* owner) : Projectile(position, velocity,1,0,100, owner){
}
Bullet::~Bullet() = default;
void Bullet::draw() const{
	//DrawEllipse(getPosition().x, getPosition().y + (50 / getPosition().z +1), 4 / getPosition().z, 10 / getPosition().z, RED);
	//DrawEllipse(getPosition().x, getPosition().y, 4 / getPosition().z, 10 / getPosition().z, RED);
	virtualCamera::camResults results = virtualCamera::projectPoint(getPosition());
	//results.position2D = { results.position2D.x + GetScreenWidth() / 2, results.position2D.y + GetScreenHeight() / 2 };
	float circleSize = 1000 / results.distance;
	if (1000 / results.distance < 1.0f) circleSize = 1.0f;
	DrawCircle(results.position2D.x, results.position2D.y, circleSize, RED);
}
void Bullet::Update() { 
	if (getPosition().z > 10000) {
		Entity::enableDeleteEntity();
	}
	else {
		updatePosition();
		hitboxradius = 5;
	}
	
}
