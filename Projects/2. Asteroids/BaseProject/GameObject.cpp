#include "GameObject.h"

GameObject::GameObject(std::vector<float> vertexData, std::vector<unsigned int> indexData, Shader shader, Image texture, glm::vec3 position) {
    indices = indexData;
    this->position = position;

    vertSize = glm::vec2(abs(vertexData[0]), abs(vertexData[0]));

    // Creating Objects to send to GPU
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Binding VAO 
    glBindVertexArray(VAO);

    // Adding vertices to VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(unsigned int), &indexData[0], GL_STATIC_DRAW);

    // Specifies the location and data format of the bound VBO to use when rendering
    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Textures
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Texture and shader
    this->shader = shader;
    this->texture = texture;
}

void GameObject::update(Camera camera, float deltaTime) {
    // Matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    view = glm::translate(view, position);
    glm::mat4 projection = glm::perspective(Settings::FOV, (float)Settings::WIDTH / Settings::HEIGHT, 0.1f, 100.0f);  // projection remains the same for all cubes

    // Uniforms
    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    glUseProgram(0);
}

void GameObject::draw() {
    bindAll();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);  // Draw
    unbindAll();
}

bool GameObject::collideswith(GameObject &other) {

    // collision x-axis?
    bool collisionX = position.x + vertSize.x >= other.position.x - other.vertSize.x &&
        other.position.x + other.vertSize.x >= position.x - vertSize.x;
    // collision y-axis?
    bool collisionY = position.y - vertSize.y <= other.position.y + other.vertSize.y &&
        other.position.y - other.vertSize.y <= position.y + vertSize.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

void GameObject::bindAll() {
    glBindVertexArray(VAO);  // Bind the VAO
    shader.use();  // Bind Shader
    texture.use();  // Bind textures on corresponding texture units
}

void GameObject::unbindAll() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void GameObject::inBounds() {
    // std::cout << position.x << " " << position.y << std::endl;

    float max_x = 13.5f;
    float max_y = 10.0f;

    if (abs(position.x) > max_x) {
        position.x = -abs(position.x) / position.x * max_x;
    }
    if (abs(position.y) > max_y) {
        position.y = -abs(position.y) / position.y * max_y;
    }
}