#include "Explosion.h"

Explosion::Explosion(Vector3 position, Vector3 velocity, int size, int deathtime) : Entity(position, velocity, 1, 0, 0, EXPLOSION), size(size), deathtime(deathtime) {
	for (int i = 0; i < GetRandomValue(size, 2 * size); i++) {
		auto vel = virtualCamera::sVector3(velocity) + virtualCamera::sVector3({ GetRandomValue(-10 * size,10 * size) / 20.0f, GetRandomValue(-10 * size,10 * size) / 20.0f , GetRandomValue(-10 * size,10 * size) / 20.0f });
		Color rdmColor = RED;
		switch (GetRandomValue(1, 5)) {		//particel color for explosion
		case 1: rdmColor = { 255, 69, 0, 255 }; break;     // Orange-Rot (Feuer)
		case 2: rdmColor = { 255, 140, 0, 255 }; break;   // Dunkelorange (glühend)
		case 3: rdmColor = { 255, 215, 0, 255 }; break;   // Goldgelb (Funken)
		case 4: rdmColor = { 178, 34, 34, 255 }; break;   // Feuerrot (Hitze)
		case 5: rdmColor = { 255, 255, 224, 200 }; break; // Rauchiges Gelb (transluzent)
		}
		particals.push_back(Worldpoint(position, vel.vec, rdmColor));
	}
	// stereo sound
	sound = LoadSoundFromWave(Soundmanager::sounds.explosion);
	auto results = virtualCamera::projectPoint(getPosition());	// get relative position of explosion to camera
	SetSoundVolume(sound, 1 / (1 + results.distance / 100)); // set sound volume based on distance and size
	SetSoundPan(sound, ((virtualCamera::worldWidth - results.position2D.x) / virtualCamera::worldWidth)); // set sound pan based on relative position
	PlaySound(sound);
}
Explosion::~Explosion() {
	particals.clear();
}
void Explosion::Update() {
	auto results = virtualCamera::projectPoint(getPosition());	// get relative position of explosion to camera
	SetSoundVolume(sound, 1 / (1 + results.distance / 100)); // set sound volume based on distance and size
	SetSoundPan(sound, ((virtualCamera::worldWidth - results.position2D.x) / virtualCamera::worldWidth)); // set sound pan based on relative position
	if (deathtime < lifetime) enableDeleteEntity();	// delete explosion after deathtime
	for (auto& p : particals) {
		p.Update();
	}
	lifetime++;
}
void Explosion::draw() const {
	for (auto& p : particals) {
		p.draw();
	}
}