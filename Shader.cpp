#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

Shader& Shader::use()
{
	// Sets the this object as the current active shader
	// program to use for subsequent drawing commands.
	// If id equals 0, OpenGL will resort to its old drawing patterns.
	glUseProgram(id_);
	return *this;
}

void Shader::compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource)
{
	// Vertex Shader
	GLuint sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, nullptr);
	glCompileShader(sVertex);
	checkCompileErrors(sVertex, "VERTEX");
	// Fragment Shader
	GLuint sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, nullptr);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");
	// If geometry shader source code is given, also compile geometry shader
	GLuint gShader;
	if (geometrySource != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, nullptr);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
	}

	// Shader Program
	id_ = glCreateProgram();
	glAttachShader(id_, sVertex);
	glAttachShader(id_, sFragment);
	if (geometrySource != nullptr)
		glAttachShader(id_, gShader);
	glLinkProgram(id_);
	checkCompileErrors(id_, "PROGRAM");
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr)
		glDeleteShader(gShader);
}

void Shader::setFloat(const GLchar* name, GLfloat value, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform1f(glGetUniformLocation(id_, name), value);
}

void Shader::setInteger(const GLchar* name, GLint value, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform1i(glGetUniformLocation(id_, name), value);
}

void Shader::setVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform2f(glGetUniformLocation(id_, name), x, y);
}

void Shader::setVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform2f(glGetUniformLocation(id_, name), value.x, value.y);
}

void Shader::setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform3f(glGetUniformLocation(id_, name), x, y, z);
}

void Shader::setVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform3f(glGetUniformLocation(id_, name), value.x, value.y, value.z);
}

void Shader::setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform4f(glGetUniformLocation(id_, name), x, y, z, w);
}

void Shader::setVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform4f(glGetUniformLocation(id_, name), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader)
{
	if (useShader)
		use();
	glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(GLuint object, std::string type)
{
	GLint success = false;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, nullptr, infoLog);
			std::cerr << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, nullptr, infoLog);
			std::cerr << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}
