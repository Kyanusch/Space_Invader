#pragma once
#include <iostream>
#include "raylib.h"
#include "virtualCamera.h"
#include "Soundmanager.h"

class Entity {
protected:
	float hitboxradius = 0.0f;
	void updatePosition();
public:
	enum EntityType {
		DEFAULT,
		PLAYER,
		ASTEROID,
		ENEMY,
		PROJECTILE,
		EXPLOSION,
		WORLDPOINT
	};

	typedef struct killCounts {
		int killedAsteroids;
		int killedEnemies;
	} killCounts;

	Entity(Vector3 position, Vector3 velocity, int health, int shield, int experience);
	Entity(Vector3 position, Vector3 velocity, int health, int shield, int experience, EntityType type);
	virtual ~Entity();
	virtual void draw() const = 0;
	virtual void Update() = 0;
	Entity* getThisEntity() const;
	//Position
	Vector3 getPosition() const;
	void setPosition(Vector3 newposition);
	void setPosition_X(float x);
	void setPosition_Y(float y);
	void setPosition_Z(float z);

	//velocity
	Vector3 getVelocity() const;
	void setVelosity(Vector3 newvelocity);
	void setVelocity_X(float x);
	void setVelocity_Y(float y);
	void setVelocity_Z(float z);

	int getHealth();
	int getShield();
	void setShield(int newShield);
	virtual void damage(int damage);
	int getExperience();
	virtual void setExperience(int exp);
	killCounts getKillCounts();
	float getHitboxradius() const;

	void killEntity(Entity& entity);
	EntityType getType() const;
	void enableDeleteEntity();
	bool getEnableDelete();
private:
	Vector3	position;
	Vector3 velocity;
	int health;
	int shield;
	int experience;
	bool enableDelete = false;
	EntityType type;
	killCounts kills = { 0,0 };
};
