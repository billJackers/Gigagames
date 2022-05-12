#pragma once
#include <camera/camera.h>
#include <vector>

#include "Settings.h"
#include "GameObject.h"

enum Asteroid_Type {
    ASTEROID_BIG,
    ASTEROID_MEDIUM,
    ASTEROID_SMALL
};

class Asteroid : public GameObject {

public:
    // Positioning
    glm::vec3 velocity;
    float direction;
    Asteroid_Type aType;

private:
    bool alive = true;

public:
    Asteroid(float t_direction, glm::vec3 t_position, Asteroid_Type t_atype);
    void update(Camera camera, float deltaTime);
    bool isAlive();
    void destroy();

private:
    std::vector<float> getVertices(Asteroid_Type t_atype);
};
