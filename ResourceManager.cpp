#include "ResourceManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <SOIL/soil.h>

ResourceManager& ResourceManager::getInstance()
{
	static ResourceManager instance;
	return instance;
}


void ResourceManager::init()
{
	OutputDebugStringW(L"ResourceManager::init()\n");
	getInstance();
}

void ResourceManager::clear()
{
	OutputDebugStringW(L"ResourceManager::clear()\n");
	for (auto& var : shaders_)
		glDeleteProgram(var.second.id_);

	for (auto& var : textures_)
		glDeleteTextures(1, &var.second.id_);
}

Shader& ResourceManager::loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name)
{
	OutputDebugStringW(L"ResourceManager::loadShader()\n");
	shaders_[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return shaders_[name];
}

Shader& ResourceManager::getShader(std::string name)
{
	return shaders_[name];
}

Texture2D& ResourceManager::loadTexture(const GLchar* file, GLboolean alpha, std::string name)
{
	textures_[name] = loadTextureFromFile(file, alpha);
	return textures_[name];
}

Texture2D& ResourceManager::getTexture(std::string name)
{
	return textures_[name];
}

Shader ResourceManager::loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile)
{
	OutputDebugStringW(L"ResourceManager::loadDataFromFile()\n");
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		// Open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// If geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cerr << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();
	// 2. Now create shader object from source code
	Shader shader;
	shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar* file, GLboolean alpha)
{
	OutputDebugStringW(L"ResourceManager::loadTextureFromFile()\n");
	// Create Texture object
	Texture2D texture;
	if (alpha)
	{
		texture.internalFormat_ = GL_RGBA;
		texture.imageFormat_ = GL_RGBA;
	}
	// Load image
	int width;
	int height;
	auto forceChannels = texture.imageFormat_ == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB;
	unsigned char* image = SOIL_load_image(file, &width, &height, nullptr, forceChannels);

	std::cout << "forceChannels = " << forceChannels << "\n";
	// Now generate texture
	texture.generate(width, height, image);
	// And finally free image data
	SOIL_free_image_data(image);
	return texture;
}
