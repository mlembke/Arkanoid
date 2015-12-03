#pragma once
#define GLEW_STATIC
#include <GL/glew.h>

/**
 * GameState
 * Represents two dimensional texture.
 */
class Texture2D
{
public:
	Texture2D();
	~Texture2D() = default;

	void generate(GLuint width, GLuint height, unsigned char* data);
	void Bind() const;

	GLuint id_;
	GLuint width_;
	GLuint height_;
	GLuint internalFormat_;
	GLuint imageFormat_;
	GLuint wrapSAxis_;
	GLuint wrapTAxis_;
	GLuint filterMin_;
	GLuint filterMax_;
};

