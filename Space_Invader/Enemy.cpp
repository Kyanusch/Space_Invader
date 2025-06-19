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

    // Update position
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
    virtualCamera::sVector3 Points3[7] = {
        virtualCamera::sVector3({-20,0,0}),
        virtualCamera::sVector3({0,5,0}),
        virtualCamera::sVector3({ 20,0,0 }),
        virtualCamera::sVector3({ 0,-12,0 }),
        virtualCamera::sVector3({ -53,0,0 }),
        virtualCamera::sVector3({ 53,0,0 }),
        virtualCamera::sVector3({ 0,0,-40 })
    };
    /*for (auto& p : Points3) {
        p.vec.z *= -1 ;
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
 
    Vector2 Points2[7];
    auto pos = virtualCamera::sVector3(getPosition());
    for (int i = 0; i < 7; i++) {
        Points3[i] = Points3[i] + pos;
        Points2[i] = virtualCamera::projectPoint(Points3[i].vec).position2D;
    }

    DrawLine(Points2[0].x, Points2[0].y, Points2[1].x, Points2[1].y, RAYWHITE);
    DrawLine(Points2[1].x, Points2[1].y, Points2[2].x, Points2[2].y, RAYWHITE);
    DrawLine(Points2[2].x, Points2[2].y, Points2[3].x, Points2[3].y, RAYWHITE);
    DrawLine(Points2[3].x, Points2[3].y, Points2[0].x, Points2[0].y, RAYWHITE);
    DrawLine(Points2[0].x, Points2[0].y, Points2[4].x, Points2[4].y, RAYWHITE);
    DrawLine(Points2[2].x, Points2[2].y, Points2[5].x, Points2[5].y, RAYWHITE);
    for (int i = 0; i < 6; i++) {
        DrawLine(Points2[i].x, Points2[i].y, Points2[6].x, Points2[6].y, WHITE);
    }
    //// Calculate orientation: sum of movement direction and direction to target
    //Vector3 v = getVelocity();
    //virtualCamera::sVector3 moveDir(v);
    //virtualCamera::sVector3 toTarget;
    //if (targetEntity) {
    //    Vector3 t = targetEntity->getPosition();
    //    Vector3 p = getPosition();
    //    toTarget = virtualCamera::sVector3({t.x - p.x, t.y - p.y, t.z - p.z});
    //} else {
    //    toTarget = virtualCamera::sVector3({0, 0, 1});
    //}
    //virtualCamera::sVector3 orientation = moveDir + toTarget;
    //orientation = virtualCamera::normalize(orientation);

    //// Use rotatePointAroundAxis to orient the model
    //// Main body (sphere wireframe)
    //virtualCamera::sVector3 center(getPosition());
    //float bodyRadius = 50.0f;

    //// Draw cockpit sphere wireframe (8 points)
    //for (int i = 0; i < 8; ++i) {
    //    float angle1 = (float)i / 8.0f * 2.0f * 3.1415926f;
    //    float angle2 = (float)(i+1) / 8.0f * 2.0f * 3.1415926f;
    //    virtualCamera::sVector3 p1 = center + (orientation * bodyRadius);
    //    virtualCamera::sVector3 p2 = center + (orientation * bodyRadius);
    //    // Rotate around Y axis for the ring
    //    p1 = virtualCamera::rotatePointAroundAxis(p1 - center, Vector3{0,1,0}, angle1) + center;
    //    p2 = virtualCamera::rotatePointAroundAxis(p2 - center, Vector3{0,1,0}, angle2) + center;
    //    // Project and draw line
    //    auto res1 = virtualCamera::projectPoint(p1.vec);
    //    auto res2 = virtualCamera::projectPoint(p2.vec);
    //    if (res1.inView && res2.inView) {
    //        DrawLine((int)res1.position2D.x, (int)res1.position2D.y, (int)res2.position2D.x, (int)res2.position2D.y, BLUE);
    //    }
    //}

    //// Draw wings (vertical rectangles, TIE-Fighter style)
    //float wingOffset = 70.0f;
    //float wingHeight = 100.0f;
    //float wingDepth = 50.0f;

    //for (int side = -1; side <= 1; side += 2) {
    //    Vector3 baseVec = getPosition();
    //    baseVec.x += wingOffset * side;
    //    virtualCamera::sVector3 base(baseVec);
    //    virtualCamera::sVector3 top(baseVec); top.vec.y += wingHeight/2;
    //    virtualCamera::sVector3 bottom(baseVec); bottom.vec.y -= wingHeight/2;
    //    virtualCamera::sVector3 frontTop = top + (orientation * (wingDepth/2));
    //    virtualCamera::sVector3 frontBottom = bottom + (orientation * (wingDepth/2));
    //    virtualCamera::sVector3 backTop = top - (orientation * (wingDepth/2));
    //    virtualCamera::sVector3 backBottom = bottom - (orientation * (wingDepth/2));

    //    auto p1 = virtualCamera::projectPoint(frontTop.vec);
    //    auto p2 = virtualCamera::projectPoint(frontBottom.vec);
    //    auto p3 = virtualCamera::projectPoint(backTop.vec);
    //    auto p4 = virtualCamera::projectPoint(backBottom.vec);

    //    if (p1.inView && p2.inView) DrawLine((int)p1.position2D.x, (int)p1.position2D.y, (int)p2.position2D.x, (int)p2.position2D.y, BLUE);
    //    if (p3.inView && p4.inView) DrawLine((int)p3.position2D.x, (int)p3.position2D.y, (int)p4.position2D.x, (int)p4.position2D.y, BLUE);
    //    if (p1.inView && p3.inView) DrawLine((int)p1.position2D.x, (int)p1.position2D.y, (int)p3.position2D.x, (int)p3.position2D.y, BLUE);
    //    if (p2.inView && p4.inView) DrawLine((int)p2.position2D.x, (int)p2.position2D.y, (int)p4.position2D.x, (int)p4.position2D.y, BLUE);
    //}
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