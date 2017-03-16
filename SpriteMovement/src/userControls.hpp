#ifndef USERCONTROLS_HPP
#define USERCONTROLS_HPP

#include <glew.h>
#include <glm.hpp>
#include <glfw3.h>
#include "Sprite3d.hpp"

//scale of the bounds of the world
#define WORLDSIZE 10

void computeMVPMatrices(GLFWwindow* window, Sprite3d & controlledSprite);
glm::mat4 getVMatrix();
glm::mat4 getPMatrix();

#endif