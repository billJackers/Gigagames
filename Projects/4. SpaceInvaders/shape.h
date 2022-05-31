#ifndef SHAPE_H
#define SHAPE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "shader.h"
#include "coordinate.h"
#include "sprite.h"

class Shape
{
public:
	std::vector<float> verticesData;
	std::vector<unsigned int> indicesData;
	Shader shader;
	glm::vec4 color;
	Coordinate position;

	float width;
	float height;

	// Vertex buffer objects
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

	Shape()
	{

	}

	Shape(std::vector<float> vertices, std::vector<unsigned int> indices, Shader _shader, glm::vec4 _color, Coordinate startPos, float _width, float _height)
	{
		verticesData = vertices;
		indicesData = indices;
		shader = _shader;
		color = _color;
		position = startPos;
		width = _width;
		height = _height;

		// Bind objects
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(float), &verticesData[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(unsigned int), &indicesData[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Unbinding
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void render()
	{
		bind();
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(position.x, position.y, 0.0f));
		shader.setMat4("transform", trans);
		shader.setVec4("aColor", color);
		
		glDrawElements(GL_TRIANGLES, indicesData.size(), GL_UNSIGNED_INT, 0);
		unbind();
	};

	void bind()
	{
		glBindVertexArray(VAO);
		shader.use();
	};

	void unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}


};

#endif