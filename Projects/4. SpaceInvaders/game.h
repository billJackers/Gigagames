#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>
#include <irrklang/irrKlang.h>

#include <vector>

#include "sprite.h"
#include "shape.h"
#include "alien.h"
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
const float ALIEN_SPEED_MULTIPLIER = 1.4f;
const float ALIEN_DROP_SPEED = 20.0f;
const float ALIEN_WIDTH = SHIP_WIDTH * 0.75;
const float ALIEN_HEIGHT = ALIEN_WIDTH * 0.65f;

// Barrier settings
const float BARRIER_WIDTH = 0.5f;
const float BARRIER_HEIGHT = 0.1f;

class Game
{
public:
    Sprite ship;
    Sprite background;
    std::vector<Sprite> bullets;
    float lastTimeFired;

    std::vector<Alien> aliens;
    std::vector<Sprite> alienBullets;
    float lastAlienFired;
    float nextFiringTime;

    std::vector<Shape> barriers;

    Shader spriteShader;
    Shader shapeShader;

    int score;
    bool gameOver;

private:
    float alienSpeed;
    int fleetDirection;

    irrklang::ISoundEngine* backgroundMusic = irrklang::createIrrKlangDevice();
    irrklang::ISoundEngine* soundEffects = irrklang::createIrrKlangDevice();

public:
    Game()
        : score(0)
    {
        // Start background music
        backgroundMusic->setSoundVolume(1.0f);
        backgroundMusic->play2D("resources/audio/temp.mp3", true);
        soundEffects->setSoundVolume(0.2f);

        shapeShader = Shader("shapeVertShader.txt", "shapeFragShader.txt");
        spriteShader = Shader("spriteVertShader.txt", "spriteFragShader.txt");

        ship = Sprite(
            {
                // Positions          // Texture
                -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, // BL
                 -1.0f + SHIP_WIDTH, -1.0f, 0.0f,  1.0f, 0.0f, // BR
                 -1.0f + SHIP_WIDTH, -1.0f + SHIP_HEIGHT, 0.0f,  0.95f, 1.0f, // TR
                -1.0f,  -1.0f + SHIP_HEIGHT, 0.0f,  0.0f, 0.95f, // TL
            }, {
                 0,  1,  2,  // 1st triangle
                 0,  2,  3,  // 2nd triangle
            },
            spriteShader,
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
        nextFiringTime = 1 + (std::rand() % 3);

        spawnBarriers();

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
        for (Alien& alien : this->aliens)
        {
            alien.render();
        }

        for (Shape& barrier : this->barriers)
        {
            barrier.render();
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
        soundEffects->play2D("resources/audio/shoot.wav", false);
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
            spriteShader,
            Texture("resources/textures/projectile.png", GL_RGBA),
            Coordinate(this->ship.position.x + (SHIP_WIDTH - BULLET_WIDTH) / 2, SHIP_HEIGHT),
            BULLET_WIDTH,
            BULLET_HEIGHT
            );
        this->bullets.push_back(newBullet);
    }

    void fireAlienBullet()
    {
        soundEffects->play2D("resources/audio/shoot.wav", false);
        int alien = std::rand() % aliens.size();
        if (aliens[alien].type != SHOOTER)
            return;
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
            spriteShader,
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
                const char* alienImage;
                AlienType type;
                if (row <= numRows / 2)
                {
                    alienImage = "resources/textures/alien.png";
                    type = NORMAL;
                }
                else
                {
                    alienImage = "resources/textures/alien2.png";
                    type = SHOOTER;
                }

                Alien newAlien = Alien(
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
                spriteShader,
                Texture(alienImage, GL_RGBA),
                Coordinate(ALIEN_WIDTH + 2 * ALIEN_WIDTH * i, 1.0f + (ALIEN_HEIGHT + 2 * ALIEN_HEIGHT * row)),
                ALIEN_WIDTH,
                ALIEN_HEIGHT,
                type
                );
                this->aliens.push_back(newAlien);
            }
        }
    }

    void spawnBarriers()
    {
        //for (int i = 0; i < 2.0f / BARRIER_WIDTH; i++)
        //{
        //Shape newBarrier = Shape(
        //    {
        //        // Positions          
        //        0.5f,  0.5f, 0.0f,  // top right
        //        0.5f, -0.5f, 0.0f,  // bottom right
        //        -0.5f, -0.5f, 0.0f,  // bottom left
        //        -0.5f,  0.5f, 0.0f   // top left 
        //    }, {
        //        0, 1, 3,   // first triangle
        //        1, 2, 3    // second triangle
        //    },
        //    shapeShader,
        //    glm::vec4(0, 255, 0, 1.0f),
        //    Coordinate(0.0f, 0.2f),
        //    BARRIER_WIDTH,
        //    BARRIER_HEIGHT
        //);
        ////}

        //this->barriers.push_back(newBarrier);
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

    void checkBulletBarrierCollisions()
    {
        bool collided = false;
        for (int i = 0; i < this->bullets.size(); i++)
        {
            if (collided) break;
            for (int j = 0; j < this->barriers.size(); j++)
            {
                if (bullets[i].collidesWithShape(barriers[j]))
                {
                    this->barriers.erase(barriers.begin() + i);
                    this->aliens.erase(aliens.begin() + j);
                    score++;
                    collided = true;
                    break;
                }
            }
        }

        collided = false;
        for (int i = 0; i < this->alienBullets.size(); i++)
        {
            if (collided) break;
            for (int j = 0; j < this->barriers.size(); j++)
            {
                if (alienBullets[i].collidesWithShape(barriers[j]))
                {
                    this->alienBullets.erase(alienBullets.begin() + i);
                    this->barriers.erase(barriers.begin() + j);
                    score++;
                    collided = true;
                    break;
                }
            }
        }
    }

    void update(float dt)
    {
        this->checkBulletAlienCollisions();
        this->checkAlienShipCollisions();
        this->checkBulletShipCollisions();
        this->checkBulletBarrierCollisions();
        this->updateBullets(dt);
        this->updateAliens(dt);

        if (glfwGetTime() - lastAlienFired >= nextFiringTime)
        {
            this->fireAlienBullet();
            lastAlienFired = glfwGetTime();
            nextFiringTime = 1;
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