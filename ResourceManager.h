#pragma once
#define GLEW_STATIC
#include <unordered_map>
#include <string>
#include <GL/glew.h>

#include "Texture2D.h"
#include "Shader.h"

/**
 * Resource Manager
 * Class defines Resource Manager. Resource Manager manages shaders and textures including data storage and loading.
 * Based on Meyer's singleton pattern.
 */
class ResourceManager
{
public:
	~ResourceManager() = default;
	static ResourceManager& getInstance();
	
	void init();
	void clear();
	Shader& loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);
	Shader& getShader(std::string name);
	Texture2D& loadTexture(const GLchar* file, GLboolean alpha, std::string name);
	Texture2D& getTexture(std::string name);
private:
	ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
	Texture2D loadTextureFromFile(const GLchar* file, GLboolean alpha);

	std::unordered_map<std::string, Shader> shaders_;
	std::unordered_map<std::string, Texture2D> textures_;
};

