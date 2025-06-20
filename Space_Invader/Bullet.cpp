#include "Bullet.h"

Bullet::Bullet(Vector3 position, Vector3 velocity, Entity* owner, Color color) : Projectile(position, velocity, 1, 0, 100, owner), color(color) {	
	hitboxradius = 5;
}
Bullet::Bullet(Vector3 position, Vector3 velocity, int damage, Entity* owner, Color color) : Projectile(position, velocity, 1, 0, damage, owner), color(color) {	
	hitboxradius = 5;
}
Bullet::~Bullet() = default;

void Bullet::draw() const {
	virtualCamera::camResults results = virtualCamera::projectPoint(getPosition());
	if (!results.inView) return; // Do not draw if not in view
	float circleSize = 1000 / results.distance;
	if (1000 / results.distance < 1.0f) circleSize = 1.0f; // Ensure circle size is at least 1 pixel
	DrawCircle(results.position2D.x, results.position2D.y, circleSize, color);
}

void Bullet::Update() {
	// if the bullet is out of bounds, enable delete
	if (getPosition().x > virtualCamera::worldWidth || getPosition().x < 0) Entity::enableDeleteEntity();
	else if (getPosition().y > virtualCamera::worldHight || getPosition().y < 0) Entity::enableDeleteEntity();
	else if (getPosition().z > 10000 || getPosition().z < + virtualCamera::position.vec.z) Entity::enableDeleteEntity();
	else {
		updatePosition();	
	}
}