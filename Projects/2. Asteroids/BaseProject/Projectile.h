#pragma once
#include <camera/camera.h>

#include "Settings.h"
#include "GameObject.h"
#include "Image.h"

enum Projectile_Type {
    PROJECTILE_PLAYER,
    PROJECTILE_ENEMY
};

class Projectile : public GameObject {

public:
    // Positioning
    glm::vec3 velocity;
    float direction;
    Projectile_Type ptype;

private:
    // Existance
    float lifetime = 1.0f;
    bool alive = true;

public: 
    Projectile(float direction, glm::vec3 t_velocity, Projectile_Type ptype);
    void update(Camera camera, float deltaTime);
    bool isAlive();
    void destroy();

private:
    void updateLifetime(float deltaTime);
};