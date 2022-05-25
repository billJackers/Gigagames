#ifndef TEXTURE_H
#define TEXTURE_H

#include "stb_image.h"

#include <glad/glad.h>

class Texture
{
public:
	unsigned int ID;

	int width, height;
	int nrChannels;
	unsigned int internalFormat;
	unsigned int imageFormat;

	unsigned int Wrap_S;
	unsigned int Wrap_T;
	unsigned int Filter_Min;
	unsigned int Filter_Max;

	const char* textureSource;

	Texture()
	{

	};

	Texture(const char* _textureSource)
		: width(0), height(0), internalFormat(GL_RGB), imageFormat(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR), textureSource(_textureSource)
	{
		glGenTextures(1, &this->ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);

		unsigned char* data = stbi_load(textureSource, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	};

	void use()
	{
		glBindTexture(GL_TEXTURE_2D, ID);
	}
};

#endif