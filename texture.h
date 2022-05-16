#pragma once

#include "shader.h"

#include "stb/stb_image.hpp"

class Texture
{
public:
	GLuint id;
	const char* type;
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void bind();
	void unbind();
	void dispose();
};