#pragma once
#include "raylib.h"
#include <iostream>
#include "virtualCamera.h"
#include "Bullet.h"
#include "Player.h"
#include "Asteroid.h"
#include "Worldpoint.h"
#include "Explosion.h"
#include "Gridboarder.h"
//#include "spaceship.h"
#include <vector>
#include <algorithm>

//parameters for grid generation
#define distortion 30.0f
#define linesX 10
#define linesY 5
#define linesZ 50

class Gameround{
private:
	float difficultie;
	double lastAsteroidSpawnTime = 0;
	bool CanSpawnAsteroid();
	Player* player; 
	std::vector<std::shared_ptr<Entity>> entities;
	static bool sortZ(const std::shared_ptr<Entity>& e1, const std::shared_ptr<Entity>& e2);
	
//grids
	const float stepX = virtualCamera::worldWidth / linesX;
	const float stepY = virtualCamera::worldHight / linesY;
	const float stepZ = 12500.0f / linesZ;
	Gridboarder gridU = Gridboarder({ 0.0f,- distortion,-250.0f }, {200.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 200.0f }, 10, 50, distortion);
	Gridboarder gridD = Gridboarder({ 0.0f,virtualCamera::worldHight + 30.0f,-250.0f }, { 200.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 200.0f }, 10, 50, distortion);
	Gridboarder gridL = Gridboarder({ -30.0f,0.0f,-250.0f }, { 0.0f, 200.0f, 0.0f }, { 0.0f, 0.0f, 200.0f }, 5, 50, distortion);
	Gridboarder gridR = Gridboarder({ virtualCamera::worldWidth + 30.0f, 0.0f,-250.0f }, { 0.0f, 200.0f, 0.0f }, { 0.0f, 0.0f, 200.0f }, 5, 50, distortion);

	void updatePlayermechanics();
public:
	Gameround(int score, float difficultie, std::string playername);
	virtual ~Gameround();
	void Update();
	void Draw();
	Player* getPlayer();
	double playerDeathTime = 0; // time when player died
	Gameround* getThis();
	void spawnAsteroid();
};

