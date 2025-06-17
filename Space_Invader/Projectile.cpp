#include "Projectile.h"

Projectile::Projectile(Vector3 position, Vector3 velocity,int health, int shield, int damage, Entity* owner,const char* hitsoundpath) :
	Entity(position, velocity, health, shield, 0), damage(damage), owner(owner) {
	if (hitsoundpath != nullptr) hitsound = LoadSound(hitsoundpath);
}
Projectile::~Projectile() {
	if (hitsound.has_value()) UnloadSound(hitsound.value());
}

void Projectile::doDamage(Entity& entity) {
	bool ekilledbefor = entity.getEnableDelete();
	entity.damage(damage);
	if (ekilledbefor != entity.getEnableDelete()) owner->setExperience(owner->getExperience() + entity.getExperience());
	enableDeleteEntity();
}
