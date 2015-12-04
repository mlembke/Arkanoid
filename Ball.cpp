#include "Ball.h"



Ball::Ball() : GameObject()
{
}

Ball::Ball(glm::vec2 position, GLfloat radius, glm::vec2 velocity, Texture2D sprite) : GameObject(position, glm::vec2(2 * radius, 2 * radius), sprite, glm::vec3(1.0f), velocity), radius_(radius), isStuck_(GL_TRUE)
{
}

Ball::~Ball()
{
}

glm::vec2 Ball::move(GLfloat dt, GLuint windowWidth)
{
	if (!isStuck_)
	{
		position_ += velocity_ * dt;
		if (position_.x <= 0.0f)
		{
			velocity_.x = -velocity_.x;
			position_.x = 0.0f;
		}
		else if (position_.x + size_.x >= windowWidth - 1)
		{
			velocity_.x = -velocity_.x;
			position_.x = windowWidth - size_.x - 1;
		}

		if (position_.y <= 0.0f)
		{
			velocity_.y = -velocity_.y;
			position_.y = 0.0f;
		}
	}
	return position_;
}

void Ball::reset(glm::vec2 position, glm::vec2 velocity)
{
	position_ = position;
	velocity_ = velocity;
	isStuck_ = GL_TRUE;
}
