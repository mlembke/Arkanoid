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
	// Configure shaders
	// Load textures
	// Set render-specific controls

}

void Game::processInput(GLfloat dt)
{
}

void Game::update(GLfloat dt)
{
}

void Game::render()
{
	
}