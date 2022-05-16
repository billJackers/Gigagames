#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <iostream>

#include "coordinate.h"
#include "snake_node.h"
#include "randomizer.h"
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SnakeGame
{
public:
	bool gameOver;
	SnakeNode* head;
	SnakeNode* tail;
	SnakeNode* prevTail;
	Coordinate applePos;
	unsigned int NUM_ROWS;
	unsigned int NUM_COLS;

	Shader shader;

	int score;

private:
	Randomizer* xRandomizer;
	Randomizer* yRandomizer;

	bool shouldAdd;

	unsigned int VBO, VAO, EBO;

	float nodeWidth, nodeHeight;

	float snakeSpeed;

public:
	SnakeGame()
		: score(0), gameOver(false)
	{
		shouldAdd = false;
		xRandomizer = new Randomizer(0, NUM_COLS);
		yRandomizer = new Randomizer(0, NUM_ROWS);

		SnakeNode* head = new SnakeNode(15, 15, nullptr, RIGHT);
		SnakeNode* curNode = head;

		for (int i = 14; i >= 10; i--)
		{
			curNode->next = new SnakeNode(i, NUM_COLS / 2, nullptr, RIGHT);
			curNode = curNode->next;
		}

		applePos.x = xRandomizer->generate();
		applePos.y = yRandomizer->generate();

		nodeWidth = 2.0f / NUM_COLS;
		nodeHeight = 2.0f / NUM_ROWS;

		snakeSpeed = 0.05f;
	};

	~SnakeGame()
	{
		delete head;
		delete prevTail;
	};

	void initShader(const char* vertShaderSource, const char* fragShaderSource)
	{
		shader = Shader(vertShaderSource, fragShaderSource);

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
	};

	void render()
	{
		SnakeNode* cur = head;

		glm::mat4 trans = glm::mat4(1.0f);

		// Render apple
		// Apple
		trans = glm::translate(trans, glm::vec3(applePos.x * nodeWidth, applePos.y * nodeWidth, 0.0f));

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
	};

	void update(float& dt)
	{
		if (head == nullptr) return;
		if (dt >= snakeSpeed)
		{
			if (collision())
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

			dt = 0.0f;

			if (shouldAdd)
			{
				tail->next = prevTail;
				tail = prevTail;

				shouldAdd = false;
			}

			if (head->position.x == applePos.x && head->position.y == applePos.y)
			{
				score++;
				applePos.x = xRandomizer->generate();
				applePos.y = yRandomizer->generate();
				shouldAdd = true;

				prevTail = new SnakeNode(tail->position.x, tail->position.y, nullptr, tail->direction);
			}


		}
	};

	bool collision()
	{
		if (this->head == nullptr) return false;
		if (head->position.x >= NUM_COLS || head->position.x < 0) return true;
		if (head->position.y >= NUM_ROWS || head->position.y < 0) return true;
		SnakeNode* cur = head->next;
		while (cur != nullptr)
		{
			if (head->position.collision(cur->position))
				return true;
		}

		return false;
	};
};

#endif