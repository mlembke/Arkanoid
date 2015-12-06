#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

const glm::vec2 INITIAL_BALL_VELOCITY(100.f, -350.0f);
const glm::vec2 INITIAL_PLAYER_SIZE(100.0f, 20.0f);
const glm::vec2 INITIAL_PLAYER_VELOCITY(500.0f, 0.0f);

Game& Game::getInstance()
{
	static Game instance;
	return instance;
}

void Game::init(GLuint width, GLuint height)
{
	// Create instance
	getInstance();
	// Setup
	width_ = width;
	height_ = height;
	// Load shaders
	ResourceManager::getInstance().loadShader("Resources/Shaders/sprite.vs", "Resources/Shaders/sprite.frag", nullptr, "sprite");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width_), static_cast<GLfloat>(height_), 0.0f, -1.0f, 1.0f);
	ResourceManager::getInstance().getShader("sprite").use().setInteger("image", 0);
	ResourceManager::getInstance().getShader("sprite").setMatrix4("projection", projection);
	// Load textures
	ResourceManager::getInstance().loadTexture("Resources/Textures/background.jpg", GL_FALSE, "background");
	ResourceManager::getInstance().loadTexture("Resources/Textures/ball.png", GL_TRUE, "ball");
	ResourceManager::getInstance().loadTexture("Resources/Textures/block.png", GL_FALSE, "block");
	ResourceManager::getInstance().loadTexture("Resources/Textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::getInstance().loadTexture("Resources/Textures/paddle.png", GL_TRUE, "paddle");
	// Load game levels
	for (auto i = 0; i < 4; ++i)
	{
		GameLevel level;
		std::string file = "Resources/Levels/level" + std::to_string(i + 1) + ".data";
		level.load(static_cast<const GLchar*>(file.c_str()), width_, static_cast<GLuint>(round(0.5 * height_)));
		levels_.push_back(level);
	}
	if (levels_.size() > 0)
		currentLevel_ = 0;
	// Set player
	const glm::vec2 initialPlayerPosition(static_cast<GLfloat>(width_) / 2 - INITIAL_PLAYER_SIZE.x / 2,
		static_cast<GLfloat>(height_) - INITIAL_PLAYER_SIZE.y);
	player_ = std::make_unique<GameObject>(initialPlayerPosition, INITIAL_PLAYER_SIZE, ResourceManager::getInstance().getTexture("paddle"));
	player_->velocity_ = INITIAL_PLAYER_VELOCITY;

	GLfloat ballRadius = 12.0f;
	glm::vec2 ballPosition = initialPlayerPosition + glm::vec2(INITIAL_PLAYER_SIZE.x / 2 - ballRadius, -2 * ballRadius);
	balls_.push_back(std::move(std::make_unique<Ball>(ballPosition, ballRadius, INITIAL_BALL_VELOCITY, ResourceManager::getInstance().getTexture("ball"))));

	// Set render-specific controls
	renderer_ = std::make_unique<SpriteRenderer>(ResourceManager::getInstance().getShader("sprite"));

}

void Game::processInput(GLfloat dt)
{
	if (state_ == GAME_ACTIVE) {
		GLfloat ds = player_->velocity_.x * dt;
		if (keys_[GLFW_KEY_LEFT])
		{
			if (player_->position_.x >= 0.0f) {
				player_->position_.x -= ds;
				for (auto& ball : balls_)
					if (ball->isStuck_)
						ball->position_.x -= ds;
			}
		}
		if (keys_[GLFW_KEY_RIGHT])
		{
			if (player_->position_.x <= width_ - player_->size_.x - 1) {
				player_->position_.x += ds;
				for (auto& ball : balls_)
					if (ball->isStuck_)
						ball->position_.x += ds;
			}
		}
		if (keys_[GLFW_KEY_SPACE])
			for (auto& ball : balls_)
				if (ball->isStuck_) {
					ball->isStuck_ = GL_FALSE;
					break;
				}
	}
}

void Game::update(GLfloat dt)
{
	for (auto& ball : balls_)
	{
		ball->move(dt, width_);

	}
	doCollisions();
	if (balls_[0]->position_.y >= height_)
	{
		resetLevel();
		resetPlayer();
	}
}

void Game::render()
{
	if (state_ == GAME_ACTIVE) {
		renderer_->drawSprite(ResourceManager::getInstance().getTexture("background"),
			glm::vec2(0, 0), glm::vec2(width_, height_), 0.0f);
		levels_[currentLevel_].draw(*renderer_);
		player_->draw(*renderer_);
		for (auto& ball : balls_)
			ball->draw(*renderer_);
	}
}

void Game::resetLevel()
{
	std::string file = "Resources/Levels/level" + std::to_string(currentLevel_ + 1) + ".data";
	levels_[currentLevel_].load(static_cast<const GLchar*>(file.c_str()), width_, static_cast<GLuint>(round(0.5 * height_)));
}

void Game::resetPlayer()
{
	player_->size_ = INITIAL_PLAYER_SIZE;
	auto initialPlayerPosition = glm::vec2(static_cast<GLfloat>(width_) / 2 - INITIAL_PLAYER_SIZE.x / 2,
		static_cast<GLfloat>(height_) - INITIAL_PLAYER_SIZE.y);
	player_->position_ = initialPlayerPosition;
	player_->velocity_ = INITIAL_PLAYER_VELOCITY;

	balls_.erase(balls_.begin() + 1, balls_.end());
	balls_[0]->reset(initialPlayerPosition + glm::vec2(INITIAL_PLAYER_SIZE.x / 2 - balls_[0]->radius_, -2 * balls_[0]->radius_), INITIAL_BALL_VELOCITY);

}

Direction Game::vectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	GLfloat max = 0.0f;
	GLuint bestMatch = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dotProduct = glm::dot(glm::normalize(target), compass[i]);
		if (dotProduct > max)
		{
			max = dotProduct;
			bestMatch = i;
		}
	}
	return static_cast<Direction>(bestMatch);
}

GLboolean Game::checkCollision(GameObject& first, GameObject& second)
{
	bool collisionX = first.position_.x + first.size_.x >= second.position_.x &&
		second.position_.x + second.size_.x >= first.position_.x;

	bool collisionY = first.position_.y + first.size_.y >= second.position_.y &&
		second.position_.y + second.size_.y >= first.position_.y;

	return collisionX && collisionY;
}

Game::Collision Game::checkCollision(Ball& ball, GameObject& other)
{
	glm::vec2 center(ball.position_ + ball.radius_);
	glm::vec2 aabb_half_extents(other.size_.x / 2, other.size_.y / 2);
	glm::vec2 aabb_center(other.position_.x + aabb_half_extents.x, other.position_.y + aabb_half_extents.y);

	auto difference = center - aabb_center;
	auto clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	auto closest = aabb_center + clamped;
	difference = closest - center;
	if (glm::length(difference) < ball.radius_)
		return std::make_tuple(GL_TRUE, vectorDirection(difference), difference);
	return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}

void Game::doCollisions()
{
	for (auto& ball : balls_) {
		for (auto& box : levels_[currentLevel_].bricks) {
			if (!box.destroyed_) {
				auto collision = checkCollision(*ball, box);
				if (std::get<0>(collision))
				{
					if (!box.isSolid_)
						box.destroyed_ = GL_TRUE;
					auto collisionDirection = std::get<1>(collision);
					auto differenceVector = std::get<2>(collision);
					if (collisionDirection == LEFT || collisionDirection == RIGHT)
					{
						ball->velocity_.x = -ball->velocity_.x;
						GLfloat penetration = ball->radius_ - std::abs(differenceVector.x);
						if (collisionDirection == LEFT)
							ball->position_.x += penetration;
						else
							ball->position_.x -= penetration;
					}
					else
					{
						ball->velocity_.y = -ball->velocity_.y;
						GLfloat penetration = ball->radius_ - std::abs(differenceVector.y);
						if (collisionDirection == UP)
							ball->position_.y -= penetration;
						else
							ball->position_.y += penetration;
					}
				}
			}
		}
		Collision result = checkCollision(*ball, *player_);
		if (!ball->isStuck_ && std::get<0>(result))
		{
			GLfloat centerBoard = player_->position_.x + player_->size_.x / 2;
			GLfloat distance = ball->position_.x + ball->radius_ - centerBoard;
			GLfloat percentageDistance = distance / (player_->size_.x / 2);

			GLfloat strength = 2.0f;
			glm::vec2 oldVelocity = ball->velocity_;
			ball->velocity_.x = INITIAL_BALL_VELOCITY.x * percentageDistance * strength;
			ball->velocity_.y = -abs(ball->velocity_.y);
			ball->velocity_ = glm::normalize(ball->velocity_) * glm::length(oldVelocity);
		}
	}
}
