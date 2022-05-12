#include "Game.h"

Game::Game() {
    score = 0;
    srand(time(NULL));
}

void Game::reload() {

    player.revive();

    asteroids.clear();
    projectiles.clear();

    for (int i = 0; i < level + 3; i++)
        spawnAsteroid(ASTEROID_BIG);
}

void Game::handleInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.move(SHIP_FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player.move(SHIP_BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player.move(SHIP_ROTATE_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player.move(SHIP_ROTATE_RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        shoot(PROJECTILE_PLAYER);
}

void Game::update(GLFWwindow* window) {
    // updates deltaTime
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // checks for win or loss
    checkState();

    // update projectiles
    updateProjectiles();
    updateAsteroids();
    updateCooldown();

    // Rendering
    player.update(camera, deltaTime);
    player.draw();

    // Key Input
    handleInput(window);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

float Game::getDeltaTime() {
    return deltaTime;
}

void Game::updateProjectiles() {
    // std::cout << projectiles.size() << std::endl;

    for (int i = projectiles.size() - 1; i >= 0; i--) {
        projectiles[i].update(camera, deltaTime);
        projectiles[i].draw();

        // Check collisions with asteroids
        for (int j = asteroids.size() - 1; j >= 0; j--) {
            if (projectiles[i].collideswith(asteroids[j])) {

                score += 10;

                // Spawning child asteroids
                Asteroid_Type asize = asteroids[j].aType;
                glm::vec3 apos = asteroids[j].position;

                switch (asize)
                {
                case ASTEROID_BIG:
                    spawnAsteroid(ASTEROID_MEDIUM, apos);
                    spawnAsteroid(ASTEROID_MEDIUM, apos);
                    break;
                case ASTEROID_MEDIUM:
                    spawnAsteroid(ASTEROID_SMALL, apos);
                    spawnAsteroid(ASTEROID_SMALL, apos);
                    break;
                case ASTEROID_SMALL:
                    break;
                }

                // killing asteroids
                asteroids[j].destroy();
                projectiles[i].destroy();
                break;
            }
        }

        if (!projectiles[i].isAlive()) {
            projectiles.erase(projectiles.begin() + i);
        }
    }
}

void Game::checkState() {
    if (!player.isAlive()) {
        std::cout << "You died. Score: " << score << std::endl;
        score = 0;
        level = 0;
        reload();
    }
    if (asteroids.empty()) {
        level++;
        std::cout << "Next level: " << level << std::endl;
        reload();
    }
}

void Game::updateCooldown() {
    if (cooldown > 0) {
        cooldown -= deltaTime;
    }
}

void Game::shoot(Projectile_Type ptype) {
    if (cooldown > 0) {
        return;
    }
    cooldown = 0.25f;
    Projectile p = Projectile(player.direction, player.position, ptype);
    projectiles.push_back(p);
}

void Game::spawnAsteroid(Asteroid_Type asize, glm::vec3 pos) {  // Asteroid at specified position
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);  // random float between 0 and 1
    float angle = r * 360;

    Asteroid a = Asteroid(angle, pos, asize);
    asteroids.push_back(a);
}

void Game::spawnAsteroid(Asteroid_Type asize) {  // Asteroid at random position
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float xPos = (r * 20) - 10.0f;

    r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float yPos = (r * 20) - 10.0f;
    spawnAsteroid(asize, glm::vec3(xPos, yPos, Settings::ENTITY_DEPTH));
}

void Game::updateAsteroids() {
    for (int i = asteroids.size() - 1; i >= 0; i--) {
        asteroids[i].update(camera, deltaTime);
        asteroids[i].draw();

        if (asteroids[i].collideswith(player)) {
            player.kill();
        }

        if (!asteroids[i].isAlive()) {
            asteroids.erase(asteroids.begin() + i);
        }
    }
}