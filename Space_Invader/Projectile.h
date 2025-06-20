#pragma once
#include "entity.h"
#include <optional>
#include <iostream>
class Projectile : public Entity {
private:
	int damage;
	Entity* owner;
	std::optional<Sound> hitsound; // Optional

public:
	Projectile(Vector3 position, Vector3 velocity, int health, int shield, int damage, Entity* owner, Wave* hitsoundwave = nullptr);
	virtual ~Projectile();
	virtual void doDamage(Entity& entity); //damages the entity and checks if it was killed in the process
	Entity* getOwner() const;
};
