#include "Asteroid.h"
#include <iostream>
//TO Do:	Velocity implementieren
Asteroid::Asteroid(Vector3 position, int health, float speed)
	: Entity(position, { 0,0,0 }, health, 0, 10, ASTEROID), size(health) {
	generateRandomTarget();
	auto pos = virtualCamera::sVector3(position);
	auto trg = virtualCamera::sVector3(target);
	auto vel = virtualCamera::normalize(trg - pos) * speed;
	setVelosity(vel.vec);
	size = GetRandomValue(20, 50);
	hitboxradius = size;
	spinVelocity = GetRandomValue(0, 100) / 5000.0f;
}

Asteroid::~Asteroid() = default;

void Asteroid::Update() {
	if (getPosition().z < 50.0f - virtualCamera::focalLenght) {
		Entity::enableDeleteEntity();
	}
	setPosition_X(getPosition().x + getVelocity().x);
	setPosition_Y(getPosition().y + getVelocity().y);
	setPosition_Z(getPosition().z + getVelocity().z);
	hitboxradius = size;
	if (rotation < (2 * PI)) rotation += spinVelocity;
	else rotation = 0;
}

void Asteroid::generateRandomTarget() {
	target = { (float)GetRandomValue(0,virtualCamera::worldWidth),(float)GetRandomValue(0,virtualCamera::worldHight),0 };
}
unsigned char Asteroid::transparency() const {
	if (getPosition().z > 7000) {				//color fade in
		return (10000 - getPosition().z) / 3000 * 255;
	}
	else if (getPosition().z - 100 < 30) {		//color fade out
		return ((getPosition().z + 100) / 30) * 255;
	}
	else return 255;
}

void Asteroid::draw() const {
	//Rotation
	virtualCamera::sVector3 Points3[4] = { points[0], points[1] ,points[2] ,points[3] };
	for (auto& p : Points3) {
		p = virtualCamera::rotatePointAroundAxis(p, spinAxis, rotation);
	}

	Vector2 Points2[4];
	auto pos = virtualCamera::sVector3(getPosition());
	for (int i = 0; i < 4; i++) {
		Points3[i] = Points3[i] * size + pos;
		Points2[i] = virtualCamera::projectPoint(Points3[i].vec).position2D;
	}

	Color color = { 138,124, cblue,transparency() };
	DrawLine(Points2[0].x, Points2[0].y, Points2[1].x, Points2[1].y, color);
	DrawLine(Points2[1].x, Points2[1].y, Points2[2].x, Points2[2].y, color);
	DrawLine(Points2[2].x, Points2[2].y, Points2[0].x, Points2[0].y, color);
	for (int i = 0; i < 3; i++) {
		DrawLine(Points2[i].x, Points2[i].y, Points2[3].x, Points2[3].y, color);
	}
}