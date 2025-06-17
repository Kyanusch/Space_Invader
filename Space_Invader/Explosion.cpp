#include "Explosion.h"

Explosion::Explosion(Vector3 position, Vector3 velocity, int size, int deathtime) : Entity(position, velocity, 1, 0, 0), size(size), deathtime(deathtime) {
	for (int i = 0; i < GetRandomValue(size, 2 * size); i++) {
		auto vel = virtualCamera::sVector3(velocity) + virtualCamera::sVector3({GetRandomValue(-10 * size,10 * size)/20.0f, GetRandomValue(-10 * size,10 * size) / 20.0f , GetRandomValue(-10 * size,10 * size) / 20.0f});
		Color rdmColor = RED;
		switch (GetRandomValue(1, 5)) {		//particel color for explosion
		case 1: rdmColor= { 255, 69, 0, 255 }; break;     // Orange-Rot (Feuer)
		case 2: rdmColor= { 255, 140, 0, 255 }; break;   // Dunkelorange (glühend)
		case 3: rdmColor= { 255, 215, 0, 255 }; break;   // Goldgelb (Funken)
		case 4: rdmColor= { 178, 34, 34, 255 }; break;   // Feuerrot (Hitze)
		case 5: rdmColor= { 255, 255, 224, 200 }; break; // Rauchiges Gelb (transluzent)
		}
		particals.push_back(Worldpoint(position, vel.vec, rdmColor));
		Sound sound = Soundmanager::sounds.explosion;
		SetSoundVolume(sound, 25.0f * size/virtualCamera::projectPoint(getPosition()).distance);
		SetSoundPan(sound, 1 - getPosition().x / virtualCamera::worldWidth);
		PlaySound(sound);	
	}
}
Explosion::~Explosion() {
	particals.clear();
	//UnloadSound((Sound&)sound);
}
void Explosion::Update() {
	if (deathtime < lifetime) enableDeleteEntity();
	for (auto &p : particals) {
		p.Update();
	}
	lifetime++;																  
}
void Explosion::draw() const{
	for (auto& p : particals) {
		p.draw();
	}
}