#include "Gameround.h"

Gameround::Gameround(int score, float difficultie, std::string playername) : player(new Player({ virtualCamera::worldWidth / 2, virtualCamera::worldHight / 2, 0.0f }, score, playername)), difficultie(difficultie) {
	SetTargetFPS(180);
	virtualCamera::initialize({ virtualCamera::worldWidth / 2,virtualCamera::worldHight / 2,-250.0f }, { virtualCamera::worldWidth / 2, virtualCamera::worldHight / 2, 1000.0f }, { 0,1,0 }, 250.0f);
}	
Gameround::~Gameround() {
	//UnloadTexture(playerTex);
}

bool Gameround::sortZ(const std::shared_ptr<Entity>& e1, const std::shared_ptr<Entity>& e2) {
	return e1->getPosition().z > e2->getPosition().z;
}

void Gameround::Update() {
	if (!player->getEnableDelete()) updatePlayermechanics(); //update player mechanics if player is not dead
	
	spawnAsteroid();

	//sorting entities after there z coordinate for better drawing
	std::sort(entities.begin(), entities.end(), sortZ);

	for (size_t i = 0; i < entities.size() ; i++) {
		if (entities[i]->getEnableDelete()) {	// delete entity if marked for delition
			entities.erase(entities.begin() + i);
			i--;
		}
		else {
			entities[i]->Update();

			//check if colision in Z plane
			if (i != entities.size() - 1) {	// if not last entity
				if (entities[i]->getPosition().z <= entities[i + 1]->getPosition().z + 2 * entities[i + 1]->getHitboxradius()) {  //Collisions: possible because entities is sorted after Z position
					//check if entities collide in XY plane
					if (CheckCollisionCircles({ entities[i]->getPosition().x ,entities[i]->getPosition().y }, entities[i]->getHitboxradius(), { entities[i + 1]->getPosition().x ,entities[i + 1]->getPosition().y }, entities[i + 1]->getHitboxradius())) {
						//collision with Projectile
						if (auto projectile_ptr = std::dynamic_pointer_cast<Projectile>(entities[i])) {
							if (projectile_ptr->getOwner() != entities[i + 1]->getThisEntity() && entities[i + 1]->getType() != Entity::PROJECTILE) { //only check for collision if projectile is not from entity and entity is not a projectile
								if (projectile_ptr->getOwner()->getType() != entities[i + 1]->getType()) {	//only check collision if owner is another entity type then entity[i + 1]
									projectile_ptr->doDamage(*entities[i + 1]->getThisEntity());
									entities.push_back(std::make_shared<Explosion>(Explosion(projectile_ptr->getPosition(), projectile_ptr->getVelocity(), 2, 120)));
								}
							}
						}
					}
				}
			}

			// collisions with player
			if (!player->getEnableDelete()) { //only check for collision if player is not dead
				if (auto asteroid_ptr = std::dynamic_pointer_cast<Asteroid>(entities[i])) {	//collision player with Asteroid
					if (asteroid_ptr->getPosition().z < player->getHitboxradius() + player->getPosition().z && asteroid_ptr->getPosition().z > player->getPosition().z - player->getHitboxradius()) {
						if (CheckCollisionCircles({ player->getPosition().x, player->getPosition().y }, player->getHitboxradius(), { asteroid_ptr->getPosition().x, asteroid_ptr->getPosition().y }, asteroid_ptr->getHitboxradius())) {
							auto eVelocity = (virtualCamera::sVector3{ asteroid_ptr->getVelocity() } + virtualCamera::sVector3{ player->getVelocity() }) / 2;     //explosion velocity
							entities.push_back(std::make_shared<Explosion>(Explosion(asteroid_ptr->getPosition(), eVelocity.vec, 10, 220)));    //creat Explosion
							player->damage(asteroid_ptr->getHealth());     //damage player
							asteroid_ptr->enableDeleteEntity();     //deleting astroind after collision with player
						}
					}
				}
				else if (auto projectile_ptr = std::dynamic_pointer_cast<Projectile>(entities[i])) { //collision player with Projectile
					if (projectile_ptr->getOwner() != player->getThisEntity()) { //only check for collision if projectile is not from player
						if (projectile_ptr->getPosition().z < player->getHitboxradius() + player->getPosition().z && projectile_ptr->getPosition().z > player->getPosition().z - player->getHitboxradius()) {
							if (CheckCollisionCircles({ player->getPosition().x, player->getPosition().y }, player->getHitboxradius(), { projectile_ptr->getPosition().x, projectile_ptr->getPosition().y }, projectile_ptr->getHitboxradius())) {
								projectile_ptr->doDamage(*player); //damage player
								entities.push_back(std::make_shared<Explosion>(Explosion(projectile_ptr->getPosition(), projectile_ptr->getVelocity(), 2, 120))); //create explosion
							}
						}
					}
				}
				if (player->getEnableDelete()) { //if player is dead, create explosion. This clause gets logically only executed once
					playerDeathTime = GetTime(); //set player death time
					entities.push_back(std::make_shared<Explosion>(Explosion(player->getPosition(), { 0.0f,0.0f,0.0f }, 50, 880)));
				}
			}

			if (auto enemy_ptr = std::dynamic_pointer_cast<Enemy>(entities[i])) { //if Enemy can shoot he shoots
				if (enemy_ptr->CanShoot()) {
					entities.push_back(std::make_shared<Bullet>(enemy_ptr->Shoot()));
				}
			}
		}
	}
}
void Gameround::updatePlayermechanics() {//Player Mechanics
	player->Update();
	if (IsKeyDown(KEY_SPACE) && player->CanShoot()) {
		entities.push_back(std::make_shared<Bullet>(player->Shoot()));
	}
}

void Gameround::Draw() {
	gridU.draw(); //draw grids
	gridD.draw();
	gridL.draw();
	gridR.draw();
	/*myEnemy->draw();*/
	for (auto e : entities) {   //draw all entities
		e->draw();
	}
	if (!player->getEnableDelete()) { //draw player if not dead
		player->draw();
	}
}

Player* Gameround::getPlayer() {
	return player;
}

Gameround* Gameround::getThis() {
	return this;
}

bool Gameround::CanSpawnAsteroid() {
	double now = GetTime();
	if (now - lastAsteroidSpawnTime >= 10.0 / difficultie) {
		lastAsteroidSpawnTime = now;
		return true;
	}
	return false;
}

void Gameround::spawnAsteroid() {
	float x = virtualCamera::worldWidth * GetRandomValue(0, 1);
	float y = virtualCamera::worldHight * GetRandomValue(0, 1);
	int health = 50;
	int speed = (float)GetRandomValue(1, 5);
	if (CanSpawnAsteroid()) entities.push_back(std::make_shared<Asteroid>(Asteroid({ x, y, 10000.0f }, health, speed)));
}

void Gameround::spawnEnemy() {
    // Generate random spawn position within the world boundaries
    float x = GetRandomValue(0, (int)virtualCamera::worldWidth);
    float y = GetRandomValue(0, (int)virtualCamera::worldHight);
    float z = 15000.0f; // Fixed z position for enemies

    // Create new enemy and add to entities list
    entities.push_back(std::make_shared<Enemy>(Enemy({ x, y, z }, player->getThisEntity())));
}