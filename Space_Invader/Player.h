#pragma once
#include "Entity.h"
#include "Bullet.h"

class Player : public Entity {
private:
    float speedfactor = 3;
    float speedlimit = 5;
    int score = 0;
    double lastShotTime;
public:
    Player(Vector3 position, int score);
    double shootCooldown;
    void Update();       
    bool CanShoot();
    Bullet Shoot() const; 
    void draw() const override;

    void setExperience(int exp);
    int getScore();
    void addScore(int s);
};

