#include "Enemy.h"
#include <cmath>
#include "raylib.h" // For GetRandomValue

Enemy::Enemy(Vector3 position, Entity* target)
    : Entity(position, {0,0,0}, 300, 0, 1000, Entity::ENEMY),
      targetEntity(target),
      lastShotTime(0.0),
      nextShotInterval(getRandomShotInterval())
      
{
    hitboxradius = 50.0f; // Set hitbox radius for collision detection
    generateNewTargetLocation(); // Generate initial target location
}

void Enemy::Update() {
    if (hasReachedTarget()) {
        generateNewTargetLocation();
    }

    // Calculate direction to current target location
    virtualCamera::sVector3 targetLoc(targetLocation);
    virtualCamera::sVector3 pos(getPosition());
    virtualCamera::sVector3 toTarget = targetLoc - pos;

    // Calculate distance to target
    distance2TargetLocation = virtualCamera::lenght(toTarget);

    // Normalize direction vector
	toTarget = virtualCamera::normalize(toTarget);

    // Calculate speed based on distance
    float speed = calculateSpeed();

    // Update velocity
    virtualCamera::sVector3 newVelocity = toTarget * speed;
    setVelosity(newVelocity.vec);

	// Update position  (we had also the possibility to use Entity::updatePosition() here but never mind)
	virtualCamera::sVector3 newPos = pos + newVelocity;
    
    setPosition(newPos.vec);
}

void Enemy::generateNewTargetLocation() {
    // Generate random position within boundary box
    targetLocation.x = GetRandomValue((int)boundaryMin.x, (int)boundaryMax.x);
    targetLocation.y = GetRandomValue((int)boundaryMin.y, (int)boundaryMax.y);
    targetLocation.z = GetRandomValue((int)boundaryMin.z, (int)boundaryMax.z);
}

float Enemy::calculateSpeed() const {
    // Calculate speed based on distance to target
    // Uses linear interpolation between MIN_SPEED and MAX_SPEED
    // Maximum reference distance is 500 units
    const float MAX_REFERENCE_DISTANCE = 10000.0f; // max distance to target for max speed
    float normalizedDistance = std::min(distance2TargetLocation / MAX_REFERENCE_DISTANCE, 1.0f);
    return MIN_SPEED + normalizedDistance * (MAX_SPEED - MIN_SPEED);
}

bool Enemy::hasReachedTarget() const {
    return distance2TargetLocation < ARRIVAL_THRESHOLD;
}

void Enemy::draw() const {
    virtualCamera::sVector3 Points3[9] = {
        virtualCamera::sVector3({25,15,0}),
        virtualCamera::sVector3({-25,15,0}),
        virtualCamera::sVector3({ 25,-15,0 }),
        virtualCamera::sVector3({ -25,-15,0 }),
        virtualCamera::sVector3({15,8,-40}),
        virtualCamera::sVector3({-15,8,-40}),
        virtualCamera::sVector3({ 15,-8,-40 }),
        virtualCamera::sVector3({ -15,-8,-40 }),
		virtualCamera::sVector3({ 0,0,-1 })
    };
    /*for (auto& p : Points3) {
        p = p * 5;
    }*/
    Vector3 targetPosition = targetEntity->getPosition();
    virtualCamera::sVector3 toTarget = virtualCamera::sVector3(targetPosition) - virtualCamera::sVector3(getPosition());

    //Rotation yaw
    float yawAngle = atanf(toTarget.vec.x / toTarget.vec.z);
    for (auto& p : Points3) {
        p = virtualCamera::rotatePointAroundAxis(p, { 0.0f,1.0f,0.0f }, yawAngle);
    }
    //Rotation pitch
    float pitchAngle = atanf(toTarget.vec.y / toTarget.vec.z);
    for (auto& p : Points3) {
        p = virtualCamera::rotatePointAroundAxis(p, { -1.0f,0.0f,0.0f }, pitchAngle);
    }
 
    Vector2 Points2[9];
    auto pos = virtualCamera::sVector3(getPosition());
    for (int i = 0; i < 9; i++) {
        Points3[i] = Points3[i] + pos;
        Points2[i] = virtualCamera::projectPoint(Points3[i].vec).position2D;
    }

    DrawLine(Points2[0].x, Points2[0].y, Points2[3].x, Points2[3].y, GRAY);
    DrawLine(Points2[1].x, Points2[1].y, Points2[2].x, Points2[2].y, GRAY);

    DrawLine(Points2[0].x, Points2[0].y, Points2[4].x, Points2[4].y, WHITE);
    DrawLine(Points2[1].x, Points2[1].y, Points2[5].x, Points2[5].y, WHITE);
    DrawLine(Points2[2].x, Points2[2].y, Points2[6].x, Points2[6].y, WHITE);
    DrawLine(Points2[3].x, Points2[3].y, Points2[7].x, Points2[7].y, WHITE);
    for (int i = 4; i < 8; i++) {
        DrawLine(Points2[i].x, Points2[i].y, Points2[8].x, Points2[8].y, VIOLET);
    }
   
}

void Enemy::setTarget(Entity* target) {
    targetEntity = target;
}

double Enemy::getRandomShotInterval() {
    int max = 70;
    int min = 10;
    return GetRandomValue(min, max)/100.0f;
}

Vector3 Enemy::getRandomMovement() {
    float x = (GetRandomValue(0, 1000) / 1000.0f) * 0.1f - 0.05f;
    float y = (GetRandomValue(0, 1000) / 1000.0f) * 0.1f - 0.05f;
    float z = (GetRandomValue(0, 1000) / 1000.0f) * 0.1f - 0.05f;
    return { x, y, z };
}

Bullet Enemy::Shoot() {
    const int damage = 10; // Damage dealt by the bullet
	const float bulletSpeed = 10.0f; // Speed of the bullet
    lastShotTime = GetTime(); // Update last shot time
    nextShotInterval = getRandomShotInterval();
    if (!targetEntity) return Bullet({ 0.0f,0.0f,20000.0f }, { 0.0f,0.0f,0.0f }, getThisEntity(), { 0,0,0,0 }); // "dark Bullet" if no target

    // Calculate direction to target
    Vector3 pos = targetEntity->getPosition();
    virtualCamera::sVector3 toTarget = virtualCamera::sVector3(pos) - virtualCamera::sVector3(getPosition());
    
	float length = virtualCamera::lenght(toTarget);         
    if (length == 0) return Bullet({ 0.0f,0.0f,20000.0f }, { 0.0f,0.0f,0.0f }, getThisEntity(), { 0,0,0,0 }); // "dark Bullet"  if target is inside our position

	virtualCamera::sVector3 velocityToTarget = virtualCamera::normalize(toTarget) * bulletSpeed; // Normalize and set velocity
    
	playShootSound();   // Play shooting sound

    return Bullet(getPosition(), velocityToTarget.vec, damage, getThisEntity(), BLUE);
}

bool Enemy::CanShoot() {
	if (!targetEntity) return false; // Cannot shoot if no target is set
    if (targetEntity->getEnableDelete()) return false; // Cannot shoot if target is deleted
	float distance2Entity = virtualCamera::lenght(virtualCamera::sVector3(targetEntity->getPosition()) - virtualCamera::sVector3(getPosition()));
	if (distance2Entity > 2500.0f) return false; // Cannot shoot if target is too far away
    return (GetTime() - lastShotTime >= nextShotInterval);
}

void Enemy::playShootSound() {
    Sound sound = LoadSoundFromWave(Soundmanager::sounds.laserM);
    auto results = virtualCamera::projectPoint(getPosition());	// get relative position of explosion to camera
    SetSoundVolume(sound, 1 / (1 + results.distance / 100)); // set sound volume based on distance and size
    SetSoundPan(sound, ((virtualCamera::worldWidth - results.position2D.x) / virtualCamera::worldWidth)); // set sound pan based on relative position
    PlaySound(sound);
}