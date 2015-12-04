#pragma once
#define GLEW_STATIC
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"


/**
* GameLevel
* Class representing level in the game.
*/
class GameLevel
{
public:
	GameLevel() = default;
	~GameLevel() = default;

	void load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);
	void draw(SpriteRenderer& renderer);
	GLboolean isCompleted();

	std::vector<GameObject> bricks;
private:
	void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);


};

