#pragma once
#include "entity.h"
#include "Worldpoint.h"
#include <vector>
class Explosion : public Entity{
private:
	int lifetime = 0;
	int deathtime;
	int size;
	std::vector<Worldpoint> particals;
public:
	Explosion(Vector3 position, Vector3 velocity, int size, int deathtime);
	~Explosion();
	void Update() override;
	void draw() const override;
};

