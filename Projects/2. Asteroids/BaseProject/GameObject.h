#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include <shaders/shader.h>
#include <camera/camera.h>

#include "Settings.h"
#include "Image.h"

class GameObject {
public:

    Shader shader;
    Image texture;
    std::vector<unsigned int> indices;
    unsigned int VBO, VAO, EBO;

    glm::vec2 vertSize;
    glm::vec3 position;

    GameObject(std::vector<float> vertexData, std::vector<unsigned int> indexData, Shader shader, Image texture, glm::vec3 position);
    void update(Camera camera, float deltaTime);
    void draw();
    void inBounds();
    bool collideswith(GameObject &other);

private:
    void bindAll();
    void unbindAll();
};