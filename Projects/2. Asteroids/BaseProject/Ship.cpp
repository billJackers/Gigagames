#include "Ship.h"

Ship::Ship() : 
    GameObject(  // Default construction inputs
    {
        // Positions          // Texture
        -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, // BL
         0.5f, -0.5f,  0.0f,  1.0f, 0.0f, // BR
         0.5f,  0.5f,  0.0f,  1.0f, 1.0f, // TR
        -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, // TL
    }, {
         0,  1,  2,  // 1st triangle
         0,  2,  3,  // 2nd triangle
    }, 
    Shader("shaders/ship.vs", "shaders/ship.fs"), 
    Image("assets/ship.png", GL_RGBA),
    glm::vec3(0.0f, 0.0f, Settings::ENTITY_DEPTH)  // Position
    )
{
    // Init Positions
    velocity = glm::vec3(0.0f);
    acceleration = glm::vec3(7.0f);
    direction = 90.0f;
}

void Ship::move(Movement dir, float deltaTime) {
    float angle = glm::radians(direction);

    glm::vec3 dp = glm::vec3(0.0f);

    if (dir == SHIP_FORWARD) {
        dp.x = cosf(angle);
        dp.y = sinf(angle);
        velocity += dp * acceleration * deltaTime;
    }
    if (dir == SHIP_BACKWARD) {
        dp.x = -cosf(angle);
        dp.y = -sinf(angle);
        velocity += dp * acceleration * deltaTime;
    }
    if (dir == SHIP_ROTATE_LEFT) {
        direction += 270 * deltaTime;
    }
    if (dir == SHIP_ROTATE_RIGHT) {
        direction -= 270 * deltaTime;
    }
}

void Ship::update(Camera camera, float deltaTime) {
    // New position
    position += velocity * deltaTime;
    slow(deltaTime);
    inBounds();

    // Matrices
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, direction - 90, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 view = camera.GetViewMatrix();
    view = glm::translate(view, position);

    glm::mat4 projection = glm::perspective(Settings::FOV, (float) Settings::WIDTH / Settings::HEIGHT, 0.1f, 100.0f);

    // Uniforms
    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    glUseProgram(0);
}

void Ship::kill() {
    alive = false;
}

bool Ship::isAlive() {
    return alive;
}

void Ship::revive() {
    alive = true;
    position = glm::vec3(0.0f, 0.0f, Settings::ENTITY_DEPTH);
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}


void Ship::slow(float deltaTime) {
    glm::vec3 unit = glm::normalize(velocity);
    if (abs(velocity.x) > 0) {
        velocity.x -= unit.x * acceleration.x / 4 * deltaTime;
    }
    if (abs(velocity.y) > 0) {
        velocity.y -= unit.y * acceleration.y / 4 * deltaTime;
    }
}
