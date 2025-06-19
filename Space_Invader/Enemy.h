#pragma once
#include "Entity.h"
#include "Bullet.h"
#include "virtualCamera.h"

class Enemy : public Entity {
public:
    Enemy(Vector3 position, Entity* target);

    void Update() override;
    void draw() const override;

    void setTarget(Entity* target);
    
    Bullet Shoot(); // Shoot a bullet towards the target
    bool CanShoot();

private:
    Entity* targetEntity;
    double lastShotTime;
    double nextShotInterval;
    Vector3 spawnPosition;

    // Movement boundaries
    Vector3 boundaryMin = {300.0f, 200.0f, 300.0f};  // Minimum boundary point
    Vector3 boundaryMax = {virtualCamera::worldWidth - 300.0f, virtualCamera::worldHight -200.0f, 700.0f}; // Maximum boundary point
    Vector3 targetLocation;                           // Current movement target
    float distance2TargetLocation;                          // Current distance to target
    const float MAX_SPEED = 25.0f;                   // Maximum movement speed
    const float MIN_SPEED = 1.0f;                    // Minimum movement speed
    const float ARRIVAL_THRESHOLD = 4.0f;            // Distance threshold to consider target reached

    double getRandomShotInterval();
	void playShootSound();
    Vector3 getRandomMovement();
    void generateNewTargetLocation();
    float calculateSpeed() const;
    bool hasReachedTarget() const;
};