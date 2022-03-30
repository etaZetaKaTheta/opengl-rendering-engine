#pragma once

#include "glad/glad.h"
#include "stb/stb_image.hpp"

#include "shader.h"

class Texture
{
public:
	GLuint id;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	void texUnit(Shader shader, const char* uniform, GLuint unit);
	void bind();
	void unbind();
	void dispose();
};