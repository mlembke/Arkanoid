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
	ResourceManager::getInstance().loadTexture("Resources/Textures/awesomeface.png", GL_TRUE, "face");
	// Set render-specific controls
	renderer_ = std::make_unique<SpriteRenderer>(ResourceManager::getInstance().getShader("spirite"));

}

void Game::processInput(GLfloat dt)
{
}

void Game::update(GLfloat dt)
{
}

void Game::render()
{
	renderer_->drawSprite(ResourceManager::getInstance().getTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}