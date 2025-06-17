#include "Entity.h"

Entity::Entity(Vector3 position, Vector3 velocity,int health, int shield, int experience) :
	position(position),
	velocity(velocity),
	health(health),
	shield(shield),
	experience(experience){}

Entity::~Entity() = default;

void Entity::enableDeleteEntity() {
	enableDelete = true;
}
bool Entity::getEnableDelete() {
	return enableDelete;
}
Entity* Entity::getThisEntity() const{
	return const_cast<Entity*>(this);
}
//Position
Vector3 Entity::getPosition() const {
	return position;
}
void Entity::setPosition(Vector3 newposition) {
	position = newposition;
}
void Entity::setPosition_X(float x){
	position.x = x;
}
void Entity::setPosition_Y(float y) {
	position.y = y;
}
void Entity::setPosition_Z(float z) {
	position.z = z;
}
void Entity::updatePosition() {
	setPosition_X(getPosition().x + getVelocity().x);
	setPosition_Y(getPosition().y + getVelocity().y);
	setPosition_Z(getPosition().z + getVelocity().z);
}
//Velocity
Vector3 Entity::getVelocity() const {
	return velocity;
}
void Entity::setVelosity(Vector3 newvelocity) {
	velocity = newvelocity;
}
void Entity::setVelocity_X(float x) {
	velocity.x = x;
}
void Entity::setVelocity_Y(float y) {
	velocity.y = y;
}
void Entity::setVelocity_Z(float z) {
	velocity.z = z;
}
//stats
int Entity::getHealth() {
	return health;
}
int Entity::getShield() {
	return shield;
}
void Entity::damage(int damage) {
	if (shield > 0) {
		shield -= damage;
	}
	else {
		health -= damage;
	}
	if (health <= 0) {
		enableDeleteEntity();
	}
}
int Entity::getExperience() {
	return experience;
}
void Entity::setExperience(int exp) {
	experience = exp;
}
float Entity::getHitboxradius() const {
	return hitboxradius;
}