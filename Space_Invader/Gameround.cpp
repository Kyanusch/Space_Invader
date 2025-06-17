#include "Gameround.h"

Gameround::Gameround(int score, float difficultie) : player(new Player({ virtualCamera::worldWidth / 2, virtualCamera::worldHight / 2, 0.0f}, score)), difficultie(difficultie){
    SetTargetFPS(180);
    virtualCamera::initialize({ virtualCamera::worldWidth/2,virtualCamera::worldHight/2,-250.0f }, { virtualCamera::worldWidth / 2, virtualCamera::worldHight / 2, 1000.0f }, { 0,1,0 }, 250.0f);
    /*entities.push_back(std::make_shared<Worldpoint>(Worldpoint({ 0.0f,0.0f,0.0f }, GREEN)));
    entities.push_back(std::make_shared<Worldpoint>(Worldpoint({ virtualCamera::worldWidth,0.0f,0.0f }, BLUE)));
    entities.push_back(std::make_shared<Worldpoint>(Worldpoint({ 0.0f,virtualCamera::worldHight,0.0f }, BLUE)));
    entities.push_back(std::make_shared<Worldpoint>(Worldpoint({ virtualCamera::worldWidth ,virtualCamera::worldHight,0.0f }, BLUE)));
    entities.push_back(std::make_shared<Worldpoint>(Worldpoint({ 0.0f,0.0f,1000.0f }, RED)));
    entities.push_back(std::make_shared<Worldpoint>(Worldpoint({ virtualCamera::worldWidth,0.0f,1000.0f }, RED)));
    entities.push_back(std::make_shared<Worldpoint>(Worldpoint({ 0.0f,virtualCamera::worldHight,1000.0f }, RED)));
    entities.push_back(std::make_shared<Worldpoint>(Worldpoint({ virtualCamera::worldWidth ,virtualCamera::worldHight,1000.0f }, RED)));*/

    
}
Gameround::~Gameround() {
    //UnloadTexture(playerTex);
}

bool Gameround::sortZ(const std::shared_ptr<Entity>& e1, const std::shared_ptr<Entity>& e2) {
    return e1->getPosition().z > e2->getPosition().z;
}

void Gameround::Update() {
    updatePlayermechanics();
    
    spawnAsteroid(); 

    //sorting entities after there z coordinate for better drawing
    std::sort(entities.begin(), entities.end(), sortZ);
    

    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getEnableDelete()) {
            //player->addScore(entities[i]->getExperience());
            entities.erase(entities.begin() + i);
            i--;
        }
        else {
            entities[i]->Update();
            //check if colision in Z plane
            if (i != entities.size() - 1) {
                if (entities[i]->getPosition().z <= entities[i + 1]->getPosition().z + 2 * entities[i + 1]->getHitboxradius()) {  //Collisions: possible because entities is sorted 4 Z
                    if (CheckCollisionCircles({ entities[i]->getPosition().x ,entities[i]->getPosition().y }, entities[i]->getHitboxradius(), { entities[i + 1]->getPosition().x ,entities[i + 1]->getPosition().y }, entities[i + 1]->getHitboxradius())) {
                        //collision with Projectile
                        if (auto projectile_ptr = std::dynamic_pointer_cast<Projectile>(entities[i])) { 
                            projectile_ptr->doDamage(*entities[i + 1]->getThisEntity());
                            entities.push_back(std::make_shared<Explosion>(Explosion(projectile_ptr->getPosition(), projectile_ptr->getVelocity(), 2, 120)));
                        }
                    }
                }
            }
            //collision Player with Asteroid
            if (auto asteroid_ptr = std::dynamic_pointer_cast<Asteroid>(entities[i])) { 
                if (asteroid_ptr->getPosition().z < player->getHitboxradius() + player->getPosition().z && asteroid_ptr->getPosition().z > player->getPosition().z - player->getHitboxradius()) {
                    if (CheckCollisionCircles({ player->getPosition().x, player->getPosition().y }, player->getHitboxradius(), { asteroid_ptr->getPosition().x, asteroid_ptr->getPosition().y }, asteroid_ptr->getHitboxradius())) {
                        auto eVelocity = (virtualCamera::sVector3{ asteroid_ptr->getVelocity() } + virtualCamera::sVector3{ player->getVelocity()}) /2;     //explosion velocity
                        entities.push_back(std::make_shared<Explosion>(Explosion(asteroid_ptr->getPosition(),eVelocity.vec , 10, 220)));    //creat Explosion
                        player->damage(asteroid_ptr->getHealth());     //damage player
                        asteroid_ptr->enableDeleteEntity();     //deleting astroind after collision with player
                    }
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
    gridU.draw();
    gridD.draw();
    gridL.draw();
    gridR.draw();
    for (auto e : entities) {
        e->draw();
    }
    player->draw();
}

Player* Gameround::getPlayer() {
    return player;
}

Gameround* Gameround::getThis() {
    return this;
}
bool Gameround::CanSpawnAsteroid() {
    double now = GetTime();
    if (now - lastAsteroidSpawnTime >= 10.0/difficultie) {
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
    if(CanSpawnAsteroid()) entities.push_back(std::make_shared<Asteroid>(Asteroid({ x, y, 10000.0f}, health, speed)));
}

