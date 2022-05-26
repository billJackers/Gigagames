#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>
#include <vector>

#include "sprite.h"
#include "texture.h"

// Ship settings
const float SHIP_SPEED = 1.25f;
const float SHIP_WIDTH = 0.15f;
const float SHIP_HEIGHT = SHIP_WIDTH * 7.0f / 8.0f;

// Bullet settings
const float BULLET_SPEED = 1.4f;
const float BULLET_WIDTH = 0.025f;
const float BULLET_HEIGHT = BULLET_WIDTH * 1.2f;
const float RELOAD_TIME = 0.75f;

// Alien settings
const float ALIEN_SPEED_MULTIPLIER = 1.2f;
const float ALIEN_DROP_SPEED = 20.0f;
const float ALIEN_WIDTH = SHIP_WIDTH * 0.75;
const float ALIEN_HEIGHT = ALIEN_WIDTH * 0.65f;

class Game
{
public:
    Sprite ship;
    std::vector<Sprite> bullets;
    float lastTimeFired;

    std::vector<Sprite> aliens;
    std::vector<Sprite> alienBullets;
    float lastAlienFired;
    float nextFiringTime;

    int score;
    bool gameOver;

private:
    float alienSpeed;
    int fleetDirection;

public:
    Game()
        : score(0)
    {
        ship = Sprite(
            {
                // Positions          // Texture
                -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, // BL
                 -1.0f + SHIP_WIDTH, -1.0f, 0.0f,  1.0f, 0.0f, // BR
                 -1.0f + SHIP_WIDTH, -1.0f + SHIP_HEIGHT, 0.0f,  1.0f, 1.0f, // TR
                -1.0f,  -1.0f + SHIP_HEIGHT, 0.0f,  0.0f, 1.0f, // TL
            }, {
                 0,  1,  2,  // 1st triangle
                 0,  2,  3,  // 2nd triangle
            },
            Shader("spriteVertShader.txt", "spriteFragShader.txt"),
            Texture("resources/textures/ship.png", GL_RGBA),
            Coordinate(0.0f, 0.0f),
            SHIP_WIDTH,
            SHIP_HEIGHT
            );

        lastTimeFired = 0.0f;

        this->spawnFleet();
        fleetDirection = 1;
        alienSpeed = 0.00025;

        lastAlienFired = 0.0f;
        // Random number between 1 and 4
        nextFiringTime = 1 + (std::rand() % 3);

        gameOver = false;
    }

    void render()
    {
        // Render ship
        ship.render();
        // Render bullets
        for (Sprite& bullet : this->bullets)
        {
            bullet.render();
        }
        for (Sprite& alienBullet : this->alienBullets)
        {
            alienBullet.render();
        }
        // Render aliens
        for (Sprite& alien : this->aliens)
        {
            alien.render();
        }
    }

    void processInput(GLFWwindow* window, float dt)
    {
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            if (ship.position.x >= 0)
                ship.position.x -= SHIP_SPEED * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            if (ship.position.x <= 2.0f - SHIP_WIDTH)
                ship.position.x += SHIP_SPEED * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && glfwGetTime() - lastTimeFired >= RELOAD_TIME)
        {
            this->fireBullet();
            lastTimeFired = glfwGetTime();
        }
    }

    void fireBullet()
    {
        Sprite newBullet = Sprite(
            {
                // Positions          // Texture
                -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, // BL
                 -1.0f + BULLET_WIDTH, -1.0f, 0.0f,  1.0f, 0.0f, // BR
                 -1.0f + BULLET_WIDTH, -1.0f + BULLET_HEIGHT, 0.0f,  1.0f, 1.0f, // TR
                -1.0f,  -1.0f + BULLET_HEIGHT, 0.0f,  0.0f, 1.0f, // TL
            }, {
                 0,  1,  2,  // 1st triangle
                 0,  2,  3,  // 2nd triangle
            },
            Shader("spriteVertShader.txt", "spriteFragShader.txt"),
            Texture("resources/textures/projectile.png", GL_RGBA),
            Coordinate(this->ship.position.x + (SHIP_WIDTH - BULLET_WIDTH) / 2, SHIP_HEIGHT),
            BULLET_WIDTH,
            BULLET_HEIGHT
            );
        this->bullets.push_back(newBullet);
    }

    void fireAlienBullet()
    {
        int alien = std::rand() % aliens.size();
        Sprite newAlienBullet = Sprite(
            {
                // Positions          // Texture
                -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, // BL
                 -1.0f + BULLET_WIDTH, -1.0f, 0.0f,  1.0f, 0.0f, // BR
                 -1.0f + BULLET_WIDTH, -1.0f + BULLET_HEIGHT, 0.0f,  1.0f, 1.0f, // TR
                -1.0f,  -1.0f + BULLET_HEIGHT, 0.0f,  0.0f, 1.0f, // TL
            }, {
                 0,  1,  2,  // 1st triangle
                 0,  2,  3,  // 2nd triangle
            },
            Shader("spriteVertShader.txt", "spriteFragShader.txt"),
            Texture("resources/textures/alien_projectile.png", GL_RGBA),
            Coordinate(this->aliens[alien].position.x + (ALIEN_WIDTH / 2), this->aliens[alien].position.y + (ALIEN_HEIGHT / 2)),
            BULLET_WIDTH,
            BULLET_HEIGHT
            );
        alienBullets.push_back(newAlienBullet);
    }

    void updateBullets(float dt)
    {

        for (int i = 0; i < this->bullets.size(); i++)
        {
            bullets[i].position.y += BULLET_SPEED * dt;
            // Delete bullet if it goes offscreen
            if (bullets[i].position.y >= 2.0f)
                this->bullets.erase(bullets.begin() + i);
        }

        for (int i = 0; i < this->alienBullets.size(); i++)
        {
            alienBullets[i].position.y -= BULLET_SPEED * dt;
            if (alienBullets[i].position.y < 0.0f)
                alienBullets.erase(alienBullets.begin() + i);
        }
    }

    void spawnFleet()
    {
        float availableSpaceX = 2.0f - (2.0f * ALIEN_WIDTH);
        int numAliensX = (int) availableSpaceX / (ALIEN_WIDTH);

        float availableSpaceY = 2.0f - (3.0f * ALIEN_HEIGHT) - (SHIP_HEIGHT);
        int numRows = (int) availableSpaceY / (2.0f * ALIEN_HEIGHT);

        for (int row = 0; row < numRows; row++)
        {
            for (int i = 0; i < numAliensX; i++)
            {
                Sprite newAlien = Sprite(
                {
                    // Positions          // Texture
                    -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, // BL
                    -1.0f + ALIEN_WIDTH, -1.0f, 0.0f,  1.0f, 0.0f, // BR
                    -1.0f + ALIEN_WIDTH, -1.0f + ALIEN_HEIGHT, 0.0f,  1.0f, 1.0f, // TR
                    -1.0f,  -1.0f + ALIEN_HEIGHT, 0.0f,  0.0f, 1.0f, // TL
                }, {
                    0,  1,  2,  // 1st triangle
                    0,  2,  3,  // 2nd triangle
                },
                Shader("spriteVertShader.txt", "spriteFragShader.txt"),
                Texture("resources/textures/alien.png", GL_RGBA),
                Coordinate(ALIEN_WIDTH + 2 * ALIEN_WIDTH * i, 1.0f + (ALIEN_HEIGHT + 2 * ALIEN_HEIGHT * row)),
                ALIEN_WIDTH,
                ALIEN_HEIGHT
                );
                this->aliens.push_back(newAlien);
            }
        }
    }

    void spawnBarriers()
    {
        // TODO       
    }

    void updateAliens(float dt)
    {
        for (Sprite& alien : this->aliens)
        {
            alien.position.x += alienSpeed * fleetDirection;

            // Drop all aliens
            if (alien.position.x <= 0 || alien.position.x >= 2.0f - ALIEN_WIDTH)
            {
                fleetDirection *= -1;
                for (Sprite& a : this->aliens)
                {
                    a.position.y -= ALIEN_DROP_SPEED * dt;
                }
            }
        }
    }

    void checkBulletAlienCollisions()
    {
        bool collided = false;
        for (int i = 0; i < this->bullets.size(); i++)
        {
            if (collided) break;
            for (int j = 0; j < this->aliens.size(); j++)
            {
                if (bullets[i].collidesWith(aliens[j]))
                {
                    this->bullets.erase(bullets.begin() + i);
                    this->aliens.erase(aliens.begin() + j);
                    score++;
                    collided = true;
                    break;
                }
            }
        }
    }

    void checkAlienShipCollisions()
    {
        for (Sprite& alien : this->aliens)
        {
            if (ship.collidesWith(alien) || alien.position.y < 0.0f)
            {
                gameOver = true;
                return;
            }
        }
    }

    void checkBulletShipCollisions()
    {
        for (Sprite& alienBullet : this->alienBullets)
        {
            if (ship.collidesWith(alienBullet))
            {
                gameOver = true;
                return;
            }
        }
    }

    void update(float dt)
    {
        this->checkBulletAlienCollisions();
        this->checkAlienShipCollisions();
        this->checkBulletShipCollisions();
        this->updateBullets(dt);
        this->updateAliens(dt);

        if (glfwGetTime() - lastAlienFired >= nextFiringTime)
        {
            this->fireAlienBullet();
            lastAlienFired = glfwGetTime();
            nextFiringTime = 1 + std::rand() % 3;
        }

        if (aliens.size() == 0)
        {
            bullets.clear();
            spawnFleet();
            alienSpeed *= ALIEN_SPEED_MULTIPLIER;
        }
    }
};

#endif