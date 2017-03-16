#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <glew.h>
#include <glfw3.h>
void initCommonShaderProgram();

GLuint getCommonShaderProgram();

GLuint loadShaders(const char* vertexShaderFile, const char* fragmentShaderFile);

#endif