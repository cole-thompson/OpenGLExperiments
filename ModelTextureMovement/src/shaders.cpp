/*
 * shaders.cpp
 * for access include header file shaders.hpp
 * This file has code for inputting shader code from text files
 */

#include <fstream>
#include <string>
#include <vector>

#include <glew.h>
#include "shaders.hpp"


/*
 * Simple function that takes a shader file path and returns a String representation of that file
 */
std::string fileToString(const char *file) {
	std::ifstream fileStream(file, std::ios::in);

	if (fileStream.is_open()) {
		std::string text((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
		fileStream.close();
		return text;
	}
	else {
		fprintf(stderr, "Shader File Not Found: %s\n", file);
		return "";
	}
}

/*
 * LoadShaders takes the filepaths of a vertex shader and fragment shader, and returns a GLuint program with linked shaders
 */
GLuint LoadShaders(const char* vertexShaderFile, const char* fragmentShaderFile) {
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Get GLSL shader code from file
	std::string vertexGLSLString = fileToString(vertexShaderFile);
	const char* vertexGLSLCode = vertexGLSLString.c_str();
	std::string fragmentGLSLString = fileToString(fragmentShaderFile);
	const char* fragmentGLSLCode = fragmentGLSLString.c_str();

	//Flags for error catching
	GLint result = GL_FALSE;
	int logLength;

	//Compile vertex shader
	glShaderSource(vertexShader, 1, &vertexGLSLCode, NULL);
	glCompileShader(vertexShader);

	//Check vertex shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		std::vector<char> errorMessage(logLength + 1);
		glGetShaderInfoLog(vertexShader, logLength, NULL, &errorMessage[0]);
		fprintf(stderr, "%s\n", &errorMessage[0]);
	}

	//Compile fragment shader
	glShaderSource(fragmentShader, 1, &fragmentGLSLCode, NULL);
	glCompileShader(fragmentShader);

	//Check fragment shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		std::vector<char> errorMessage(logLength + 1);
		glGetShaderInfoLog(fragmentShader, logLength, NULL, &errorMessage[0]);
		fprintf(stderr, "%s\n", &errorMessage[0]);
	}

	//Link program to return
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	//Check program
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		std::vector<char> errorMessage(logLength + 1);
		glGetShaderInfoLog(program, logLength, NULL, &errorMessage[0]);
		fprintf(stderr, "%s\n", &errorMessage[0]);
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}