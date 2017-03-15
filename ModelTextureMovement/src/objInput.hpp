#ifndef OBJINPUT_HPP
#define OBJINPUT_HPP

#include <vector>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

bool loadObjFile(const char* path, std::vector<glm::vec3>* verticesOutput, std::vector<glm::vec2>* uvsOutput, std::vector<glm::vec3>* normalsOutput);


#endif