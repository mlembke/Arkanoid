#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "Shader.h"

/**
* SpriteRenderer
* Class defines Sprite Renderer used for drawing sprites.
*/
class SpriteRenderer
{
public:
	SpriteRenderer() = default;
	explicit SpriteRenderer(const Shader& shader);

	~SpriteRenderer();

	void drawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	void init();
	Shader shader_;
	GLuint quadVAO_;
	
};

