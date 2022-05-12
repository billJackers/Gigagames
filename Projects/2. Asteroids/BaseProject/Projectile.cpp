#include "Projectile.h"

Projectile::Projectile(float t_direction, glm::vec3 t_position, Projectile_Type t_ptype) :
    GameObject({
            // Positions          // Texture
            -0.2f, -0.2f,  0.0f,  0.0f, 0.0f, // BL
             0.2f, -0.2f,  0.0f,  1.0f, 0.0f, // BR
             0.2f,  0.2f,  0.0f,  1.0f, 1.0f, // TR
            -0.2f,  0.2f,  0.0f,  0.0f, 1.0f, // TL
        }, {
             0,  1,  2,  // 1st triangle
             0,  2,  3,  // 2nd triangle
        },
        Shader("shaders/projectile.vs", "shaders/projectile.fs"),
        Image("assets/projectile.png", GL_RGBA),
        t_position  // Position
     )
{
    direction = t_direction;

    // calculate velocity based on direction
    float angle = glm::radians(direction);
    velocity = glm::vec3(cosf(angle), sinf(angle), 0.0f) * 20.0f;

    ptype = t_ptype;
}


void Projectile::update(Camera camera, float deltaTime) {
    // New position
    inBounds();  // make sure we are in bounds
    position += velocity * deltaTime;

    // Matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(Settings::FOV, (float)Settings::WIDTH / Settings::HEIGHT, 0.1f, 100.0f);

    // Transformations
    model = glm::rotate(model, direction - 90, glm::vec3(0.0f, 0.0f, 1.0f));
    view = glm::translate(view, position);

    // Uniforms
    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    glUseProgram(0);
    
    // check if dead
    updateLifetime(deltaTime);
}

bool Projectile::isAlive() {
    return alive;
}

void Projectile::destroy() {
    lifetime = 0.0f;
}

void Projectile::updateLifetime(float deltaTime) {
    lifetime -= deltaTime;
    if (lifetime < 0) {
        alive = false;
    }
}