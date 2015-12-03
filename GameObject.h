#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"

/**
 * GameObject
 * Class representing every object in the game.
 */
class GameObject
{
public:
	GameObject();
	GameObject(glm::vec2 position, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	virtual ~GameObject();

	virtual void draw(SpriteRenderer& renderer);
private:
	glm::vec2 position_;
	glm::vec2 size_;
	glm::vec2 velocity_;
	glm::vec3 color_;
	GLfloat rotation_;
	GLboolean isSolid_;
	GLboolean destroyed_;
	Texture2D sprite_;
};

