#pragma once
#include <iostream>
#include <random>
#include "entity.h"
#include "raylib.h"
class Asteroid : public Entity {
private:
	Vector3 target;
	float spawnPos[2];
	const Vector3 spinAxis = { GetRandomValue(-100, 100) / 100.0f, GetRandomValue(-100, 100) / 100.0f, GetRandomValue(-100, 100) / 100.0f };
	float rotation = 0.0f;
	float spinVelocity;
	float size = 30;
	void generateRandomTarget();
	unsigned char transparency() const;
	unsigned char cblue = (unsigned char)GetRandomValue(100, 150);
	virtualCamera::sVector3 points[4] = {
		virtualCamera::sVector3({GetRandomValue(-50,50) / 100.0f,GetRandomValue(-50,50) / 100.0f,GetRandomValue(-50,50) / 100.0f}),
		virtualCamera::sVector3({GetRandomValue(-50,50) / 100.0f,GetRandomValue(-50,50) / 100.0f,GetRandomValue(-50,50) / 100.0f}),
		virtualCamera::sVector3({GetRandomValue(-50,50) / 100.0f,GetRandomValue(-50,50) / 100.0f,GetRandomValue(-50,50) / 100.0f}),
		virtualCamera::sVector3({GetRandomValue(-50,50) / 100.0f,GetRandomValue(-50,50) / 100.0f,GetRandomValue(-50,50) / 100.0f})
	};

public:
	Asteroid(Vector3 position, int health, float speed);
	virtual ~Asteroid();
	void Update() override;
	void draw() const;
};
