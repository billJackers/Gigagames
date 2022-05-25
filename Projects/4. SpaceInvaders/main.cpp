#include <iostream>

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// My headers
#include "shader.h"
#include "texture.h"
#include "sprite.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(600, 800, "Space Invaders", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create FLW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	Shader spriteShader = Shader("spriteVertShader.txt", "spriteFragShader.txt");
	Sprite s = Sprite(
		{
			// Positions          // Texture
			-1.0f, -1.0f,  0.0f,  0.0f, 0.0f, // BL
			 1.0f, -1.0f,  0.0f,  1.0f, 0.0f, // BR
			 1.0f,  1.0f,  0.0f,  1.0f, 1.0f, // TR
			-1.0f,  1.0f,  0.0f,  0.0f, 1.0f, // TL
		},
		{
			 0,  1,  2,  // 1st triangle
			 0,  2,  3,  // 2nd triangle
		},
		spriteShader,
		Texture("resources/textures/gigachad.jpg"),
		Coordinate(0.0f, 0.0f)
		);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		s.render();
		s.update();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}