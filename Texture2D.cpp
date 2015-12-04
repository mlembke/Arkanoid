#include "Texture2D.h"

Texture2D::Texture2D() : width_(0), height_(0), internalFormat_(GL_RGB), imageFormat_(GL_RGB), wrapSAxis_(GL_REPEAT), wrapTAxis_(GL_REPEAT), filterMin_(GL_LINEAR), filterMax_(GL_LINEAR)
{
	glGenTextures(1, &id_);
}


void Texture2D::generate(GLuint width, GLuint height, unsigned char* data)
{
	width_ = width;
	height_ = height;

	glBindTexture(GL_TEXTURE_2D, id_);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat_, width, height, 0, imageFormat_, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapSAxis_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapTAxis_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax_);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, id_);
}
