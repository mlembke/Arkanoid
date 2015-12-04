#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

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
	for (auto i = 0; i < 4; i++)
	{
		GameLevel level;
		std::string file = "Resources/Levels/level" + std::to_string(i) + ".data";
		level.load(static_cast<const GLchar*>(file.c_str()), width_, static_cast<GLuint>(round(0.5 * height_)));
		levels_.push_back(level);
	}
	if (levels_.size() > 0)
		currentLevel_ = 1;
	// Set player
	glm::vec2 playerSize(100.0f, 20.0f);
	glm::vec2 playerVelocity(500.0f, 0.0f);
	glm::vec2 playerPosition(static_cast<GLfloat>(width_) / 2 - playerSize.x / 2,
		static_cast<GLfloat>(height_) - playerSize.y);
	player_ = std::make_unique<GameObject>(playerPosition, playerSize, ResourceManager::getInstance().getTexture("paddle"));
	player_->velocity_ = playerVelocity;
	glm::vec2 initialBallVelocity(100.f, -350.0f);
	GLfloat ballRadius = 12.0f;
	glm::vec2 ballPosition = playerPosition + glm::vec2(playerSize.x / 2 - ballRadius, -2 * ballRadius);
	balls_.push_back(std::move(std::make_unique<Ball>(ballPosition, ballRadius, initialBallVelocity, ResourceManager::getInstance().getTexture("ball"))));

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
		doCollisions();
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

GLboolean Game::checkCollision(GameObject& first, GameObject& second)
{
	bool collisionX = first.position_.x + first.size_.x >= second.position_.x &&
		second.position_.x + second.size_.x >= first.position_.x;

	bool collisionY = first.position_.y + first.size_.y >= second.position_.y &&
		second.position_.y + second.size_.y >= first.position_.y;

	return collisionX && collisionY;
}

void Game::doCollisions()
{
	for (auto& box : levels_[currentLevel_].bricks)
		for (auto& ball : balls_)
			if (!box.destroyed_ && !box.isSolid_ && checkCollision(*ball, box))
				box.destroyed_ = GL_TRUE;
}
