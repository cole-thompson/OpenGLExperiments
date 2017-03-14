#ifndef USERCONTROLS_HPP
#define USERCONTROLS_HPP

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

void computeMVPMatrices(GLFWwindow* window);
glm::mat4 getVMatrix();
glm::mat4 getPMatrix();

#endif