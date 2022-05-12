#include "Asteriod.h"

std::vector<float> Asteroid::getVertices(Asteroid_Type atype) {
    std::vector<float> vertices;

    switch (atype) {
        case ASTEROID_BIG:
            vertices = {
                // Positions          // Texture
                -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, // BL
                 1.0f, -1.0f,  0.0f,  1.0f, 0.0f, // BR
                 1.0f,  1.0f,  0.0f,  1.0f, 1.0f, // TR
                -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, // TL
            };
            break;
        case ASTEROID_MEDIUM:
            vertices = {
                // Positions          // Texture
                -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, // BL
                 0.5f, -0.5f,  0.0f,  1.0f, 0.0f, // BR
                 0.5f,  0.5f,  0.0f,  1.0f, 1.0f, // TR
                -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, // TL
            };
            break;
        case ASTEROID_SMALL:
            vertices = {
                // Positions          // Texture
                -0.3f, -0.3f,  0.0f,  0.0f, 0.0f, // BL
                 0.3f, -0.3f,  0.0f,  1.0f, 0.0f, // BR
                 0.3f,  0.3f,  0.0f,  1.0f, 1.0f, // TR
                -0.3f,  0.3f,  0.0f,  0.0f, 1.0f, // TL
            };
            break;
    }
    return vertices;
}

Asteroid::Asteroid(float t_direction, glm::vec3 t_position, Asteroid_Type t_atype) :
    GameObject(
        getVertices(t_atype), 
        {
             0,  1,  2,  // 1st triangle
             0,  2,  3,  // 2nd triangle
        },
        Shader("shaders/asteroid.vs", "shaders/asteroid.fs"),
        Image("assets/asteroid1.png", GL_RGBA),
        t_position  // Position
        )
{
    aType = t_atype;
    direction = t_direction;

    // calculate velocity based on direction
    float angle = glm::radians(direction);
    velocity = glm::vec3(cosf(angle), sinf(angle), 0.0f) * 3.0f;
}


void Asteroid::update(Camera camera, float deltaTime) {
    //std::cout << position.x << " " << position.y << std::endl;
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
}

void Asteroid::destroy() {
    alive = false;
}

bool Asteroid::isAlive() {
    return alive;
}