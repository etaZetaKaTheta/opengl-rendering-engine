#include "texture.h"

Texture::Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType)
{
	type = texType;
	int texWidth, texHeight, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &texWidth, &texHeight, &numColCh, 0);

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, format, pixelType, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);


	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.id, uniform);
	shader.activate();
	glUniform1i(texUni, unit);
}

void Texture::bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::dispose()
{
	glDeleteTextures(1, &id);
}