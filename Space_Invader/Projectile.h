#pragma once
#include "entity.h"
#include <optional>
#include <iostream>
class Projectile : public Entity {
private:
	int damage;
	Entity* owner;
	std::optional<Sound> hitsound;

public:
	Projectile(Vector3 position, Vector3 velocity, int health, int shield, int damage, Entity* owner, const char* hitsoundpath = nullptr);
	virtual ~Projectile();
	virtual void doDamage(Entity& entity);
	Entity* getOwner() const;
};
