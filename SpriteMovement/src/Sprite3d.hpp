
#ifndef SPRITE3D_HPP
#define SPRITE3D_HPP

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

/*
 * This is my implementation of a simple Sprite that is loaded from a obj file
 * Each sprite can have a texture
 * Model matrix for each step, which allows for transformations
 */

class Sprite3d {
private:
	const char* objFilePath;
	const char* ddsFilePath;

	GLuint vertexArrayObject;
	GLuint objVertexbuffer;
	GLuint objUVbuffer;
	int numVertices;

	GLuint Texture;
	GLuint textureUniform;

	
	float speed = 3.0f;

public:
	glm::mat4 modelMatrix;
	glm::vec3 loc;

	Sprite3d(const char* objFP, const char* ddsFilePath);
	~Sprite3d();
	void drawSpriteTriangles();

	float getSpeed();

};


#endif
