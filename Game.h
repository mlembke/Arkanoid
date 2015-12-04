#pragma once
#define GLEW_STATIC
#include <memory>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GameState.h"
#include "GameLevel.h"
#include "SpriteRenderer.h"

/**
 * Game
 * Class representing Game instance. Based on Meyer's singleton pattern.
 */

class Game
{
public:
	~Game() = default;

	static Game& getInstance();
	void init(GLuint width, GLuint height);
	void processInput(GLfloat dt);
	void update(GLfloat dt);
	void render();

	GameState state_;
	GLboolean keys_[1024];
	GLuint width_;
	GLuint height_;
	// Sprite renderer
	std::unique_ptr<SpriteRenderer> renderer_;

	std::vector<GameLevel> levels_;
	GLuint currentLevel_;
	std::unique_ptr<GameObject> player_;

private:
	Game() : state_(GAME_MENU), width_(0), height_(0), currentLevel_(0) {}
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;


};

