#pragma once
#include "Entity.h"
#include "Bullet.h"

class Player : public Entity {
public:
    Player(Vector3 position, int score, std::string name);
    const std::string name;
    double shootCooldown;
    void Update();       
    bool CanShoot();
    Bullet Shoot() const; 
    void draw() const override;

    void setExperience(int exp);
    int getScore();
    void addScore(int s); 
    
private:
    float speedfactor = 3;
    float speedlimit = 5;
    int score = 0;
    double lastShotTime;
};


