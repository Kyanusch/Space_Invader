#include "Worldpoint.h"

Worldpoint::Worldpoint(Vector3 position, Vector3 velocity, Color color) : Entity(position, velocity, 1, 0, 0), color(color){}
Worldpoint::Worldpoint(Vector3 position, Color color) : Entity(position, {0.0f,0.0f,0.0f}, 1, 0, 0, WORLDPOINT), color(color) {
	staticMode = true;
}

void Worldpoint::draw() const {
	virtualCamera::camResults results = virtualCamera::projectPoint(getPosition());
	if(staticMode && results.inView) DrawCircle(results.position2D.x, results.position2D.y, 5, color);
	else if(results.inView) DrawCircle(results.position2D.x, results.position2D.y, 1500/results.distance + 0.5f, color);
}
void Worldpoint::Update() {
	updatePosition();
};