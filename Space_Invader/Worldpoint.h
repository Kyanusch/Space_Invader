#pragma once
#include "entity.h"
#include "virtualCamera.h"
class Worldpoint : public Entity {
private:
	Color color;
	bool staticMode = false;
public:
	Worldpoint(Vector3 position, Vector3 velocity, Color color);
	Worldpoint(Vector3 position, Color color);	// static point, no velocity, no movement
	void draw() const override;
	void Update() override;
};
