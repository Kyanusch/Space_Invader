#pragma once
#include <iostream>
#include "raylib.h"
#include "Projectile.h"

class Bullet : public Projectile{
public:
    Bullet(Vector3 position, Vector3 velocity, Entity* owner);
    virtual ~Bullet();
    void Update() override;
    void draw() const override;
};

