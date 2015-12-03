#define GLEW_STATIC
#include <iostream>
#include <windows.h>
#include <array>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "ResourceManager.h"

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;
void controls(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


int main(int argc, char** argv)
{
	OutputDebugStringW(L"main()\n");
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // Because of using OpenGL 3.1
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Arkanoid", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}
//	glGetError();
	glfwSetKeyCallback(window, controls);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	OutputDebugStringW(L"main(): First use of ResourceManager - getInsance().init()\n");
//	ResourceManager::getInstance().init();
	OutputDebugStringW(L"main(): First use of Game - getInstance().init()\n");
	Game::getInstance().init(SCREEN_WIDTH, SCREEN_HEIGHT);
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	Game::getInstance().state_ = GAME_ACTIVE;
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Check and call events
		glfwPollEvents();
		Game::getInstance().processInput(deltaTime);
		Game::getInstance().update(deltaTime);
		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Game::getInstance().render();
		glfwSwapBuffers(window);
	}
	ResourceManager::getInstance().clear();
//	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
