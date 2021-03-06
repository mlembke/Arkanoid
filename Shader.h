#pragma once
#define GLEW_STATIC
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

/**
 * Shader
 * Represents class managing vertex, fragment and geometry shaders.
 */
class Shader
{
public:
	Shader() = default;
	~Shader() = default;

	// Sets the current shader as active
	Shader& use();
	// Compiles the shader from given source code
	void compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr);
	void setFloat(const GLchar* name, GLfloat value, GLboolean useShader = false);
	void setInteger(const GLchar* name, GLint value, GLboolean useShader = false);
	void setVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void setVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader = false);
	void setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void setVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader = false);
	void setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void setVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader = false);
	void setMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader = false);

	GLuint id_;

private:
	void checkCompileErrors(GLuint object, std::string type);
};

