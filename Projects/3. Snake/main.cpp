#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "snake_node.h"

#include <iostream>
#include <random>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void update();
bool collided();

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;
int NUM_ROWS = 30;
int NUM_COLS = 30;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float timeElapsed = 0.0f;

float snakeSpeed = 0.05f;

bool fullScreen = false;

SnakeNode* head = new SnakeNode(15, 15, nullptr, RIGHT);
SnakeNode* tail;
SnakeNode* prevTail;

bool shouldAdd = false;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> xDistr(0, NUM_COLS - 1);
std::uniform_int_distribution<> yDistr(0, NUM_ROWS - 1);

int appleX;
int appleY;

bool gameOver = false;
int score = 0;

int main()
{
    appleX = xDistr(gen);
    appleY = yDistr(gen);

    SnakeNode* curNode = head;

    for (int i = 14; i >= 10; i--)
    {
        curNode->next = new SnakeNode(i, NUM_COLS / 2, nullptr, RIGHT);
        curNode = curNode->next;
    }

    tail = curNode;

    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Snake", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader("snakeVertShader.txt", "snakeFragShader.txt");

    float nodeWidth = 2.0f / NUM_COLS;
    float nodeHeight = 2.0f / NUM_ROWS;

    float vertices[] = {
        -1.0f + nodeWidth, -1.0f + nodeHeight, 0.0f, // Top right
        -1.0f + nodeWidth, -1.0f, 0.0f, // Bottom right
        -1.0f, -1.0f, 0.0f, // Bottom left
        -1.0f, -1.0f + nodeHeight, 0.0f // Top left
    };

    int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window) && !gameOver)
    {
        processInput(window);

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        timeElapsed += deltaTime;

        update();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SnakeNode* cur = head;

        glm::mat4 trans = glm::mat4(1.0f);

        // Render apple
        // Apple
        trans = glm::translate(trans, glm::vec3(appleX * nodeWidth, appleY * nodeWidth, 0.0f));

        shader.use();
        shader.setMat4("transform", trans);

        glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
        shader.setVec3("color", red);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Render snake
        while (cur != nullptr)
        {
            trans = glm::mat4(1.0f);
            trans = glm::translate(trans, glm::vec3(cur->position.x * nodeWidth, cur->position.y * nodeHeight, 0.0f));

            shader.use();
            shader.setMat4("transform", trans);

            glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
            shader.setVec3("color", green);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            cur = cur->next;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        delete cur;
    }

    delete head;
    delete prevTail;
    delete curNode;

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && head->position.x != head->next->position.x)
        head->direction = UP;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && head->position.y != head->next->position.y)
        head->direction = LEFT;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && head->position.x != head->next->position.x)
        head->direction = DOWN;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && head->position.y != head->next->position.y)
        head->direction = RIGHT;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void update()
{
    if (timeElapsed >= snakeSpeed)
    {
        if (collided())
        {
            gameOver = true;
            std::cout << "Game over" << std::endl;
            std::cout << "Score: " << score << std::endl;
        }
        SnakeNode* cur = head;

        while (cur != nullptr)
        {
            if (cur->direction == UP)
                cur->position.y++;
            if (cur->direction == DOWN)
                cur->position.y--;
            if (cur->direction == RIGHT)
                cur->position.x++;
            if (cur->direction == LEFT)
                cur->position.x--;

            if (cur->next != nullptr)
            {
                Direction temp = cur->next->direction;
                cur->next->prevDirection = temp;
                cur->next->direction = cur->prevDirection;
            }

            cur = cur->next;
        }

        head->prevDirection = head->direction;

        timeElapsed = 0.0f;

        if (shouldAdd)
        {
            tail->next = prevTail;
            tail = prevTail;

            shouldAdd = false;
        }

        if (head->position.x == appleX && head->position.y == appleY)
        {
            score++;
            appleX = xDistr(gen);
            appleY = yDistr(gen);
            shouldAdd = true;

            prevTail = new SnakeNode(tail->position.x, tail->position.y, nullptr, tail->direction);
        }

        delete cur;
    }

}

bool collided()
{
    if (head->position.x >= NUM_COLS || head->position.x < 0) return true;
    if (head->position.y >= NUM_ROWS || head->position.y < 0) return true;

    SnakeNode* c = head->next;

    while (c != nullptr)
    {
        if (head->position.x == c->position.x && head->position.y == c->position.y)
        {
            delete c;
            return true;
        }
        c = c->next;
    }
    delete c;
    return false;
}