#include "Projectile.h"

Projectile::Projectile(Vector3 position, Vector3 velocity, int health, int shield, int damage, Entity* owner, Wave* hitsoundwave) :
	Entity(position, velocity, health, shield, 0, PROJECTILE), damage(damage), owner(owner) {
	if (hitsoundwave != nullptr) hitsound = LoadSoundFromWave(*hitsoundwave); 
}
Projectile::~Projectile() {
	if (hitsound.has_value()) UnloadSound(hitsound.value());
}

void Projectile::doDamage(Entity& entity) {
	bool ekilledbefor = entity.getEnableDelete(); // save if entity was killed before damage
	entity.damage(damage);
	if (ekilledbefor != entity.getEnableDelete()) {
		owner->killEntity(entity); //if entity gets killed, call killEntity function
	}
	enableDeleteEntity();
	if (hitsound.has_value()) {
		PlaySound(hitsound.value()); // Play hit sound if available
	}
}
Entity* Projectile::getOwner() const {
	return owner;
}