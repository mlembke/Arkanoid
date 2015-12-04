#pragma once
#include "GameObject.h"
class Ball :
	public GameObject
{
public:
	Ball();
	Ball(glm::vec2 position, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
	virtual ~Ball();

	glm::vec2 move(GLfloat dt, GLuint windowWidth);
	void reset(glm::vec2 position, glm::vec2 velocity);
	
	GLfloat radius_;
	GLboolean isStuck_;
};

