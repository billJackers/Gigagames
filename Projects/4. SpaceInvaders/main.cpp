#include <iostream>

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// My headers
#include "shader.h"
#include "texture.h"
#include "sprite.h"
#include "game.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(700, 800, "Space Invaders", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create FLW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to intialize GLAD" << std::endl;
		return -1;
	}

	Game spaceInvaders = Game();

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window) && !spaceInvaders.gameOver)
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		spaceInvaders.processInput(window, deltaTime);
		spaceInvaders.update(deltaTime);
		spaceInvaders.render();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	std::cout << "GAME OVER" << std::endl;
	std::cout << "SCORE: " << spaceInvaders.score << std::endl;

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}