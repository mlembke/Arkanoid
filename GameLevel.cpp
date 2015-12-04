#include "GameLevel.h"
#include <fstream>
#include <sstream>

void GameLevel::load(const GLchar* file, GLuint levelWidth, GLuint levelHeight)
{
	bricks.clear();
	GLuint tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector < std::vector<GLuint> > tileData;
	if (fstream)
	{
		while (getline(fstream, line))
		{
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode)
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::draw(SpriteRenderer& renderer)
{
	for (auto& tile : bricks)
	{
		if (!tile.destroyed_)
			tile.draw(renderer);
	}
}

GLboolean GameLevel::isCompleted()
{
	for (auto& tile : bricks)
		if (!tile.isSolid_ && !tile.destroyed_)
			return GL_FALSE;
	return GL_TRUE;
}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
	GLuint height = tileData.size();
	GLuint width = tileData[0].size(); // init() is called only if tileData.size() > 0
	GLfloat unitWidth = static_cast<GLfloat>(levelWidth) / width;
	GLfloat unitHeight = static_cast<GLfloat>(levelHeight) / height;
	for (GLuint y = 0; y < height; ++y)
	{
		for (GLuint x = 0; x < width; ++x)
		{
			if (tileData[y][x] == 1)
			{
				glm::vec2 position(unitWidth * x, unitHeight * y);
				glm::vec2 size(unitWidth, unitHeight);
				glm::vec3 color(0.8f, 0.8f, 0.7f);
				GameObject gameObject(position, size, ResourceManager::getInstance().getTexture("block_solid"), color);
				gameObject.isSolid_ = GL_TRUE;
				bricks.push_back(gameObject);
			}
			else if (tileData[y][x] > 1)
			{
				glm::vec3 color = glm::vec3(1.0f);
				if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);

				glm::vec2 position(unitWidth * x, unitHeight * y);
				glm::vec2 size(unitWidth, unitHeight);
				bricks.push_back(GameObject(position, size, ResourceManager::getInstance().getTexture("block"), color));
			}
		}
	}
}
