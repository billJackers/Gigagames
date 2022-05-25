#ifndef SPRITE_H
#define SPRITE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "shader.h"
#include "texture.h"
#include "coordinate.h"

class Sprite
{
private:
	std::vector<float> verticesData;
	std::vector<unsigned int> indicesData;
	Shader shader;
	Texture texture;
	Coordinate position;

	// Vertex buffer objects
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

public:
	Sprite(std::vector<float> vertices, std::vector<unsigned int> indices, Shader _shader, Texture _texture, Coordinate startPos)
	{
		verticesData = vertices;
		indicesData = indices;
		shader = _shader;
		texture = _texture;
		position = startPos;

		// Bind objects
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(float), &verticesData[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(unsigned int), &indicesData[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Textures
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Unbinding
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void render()
	{
		bind();
		glDrawElements(GL_TRIANGLES, indicesData.size(), GL_UNSIGNED_INT, 0);
		unbind();
	};

	void bind()
	{
		glBindVertexArray(VAO);
		shader.use();
		texture.use();
	};

	void unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void update()
	{
		// Transformations
		this->bind();
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(position.x, position.y, 0.0f));
		shader.setMat4("transform", trans);
		this->unbind();
	};
};

#endif
