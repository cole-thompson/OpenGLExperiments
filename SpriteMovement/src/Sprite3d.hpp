
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
	GLFWwindow* window;
	static const char* vertexShader;
	static const char* fragmentShader;
	static GLuint shader;

	// MVP uniform variable for Sprite3d. these are used to communicate with shader. MVP in mvpVertexShader
	static GLuint mvpMatrixUniform;
	//mySampler2D uniform variable 
	static GLuint textureUniform;

	const char* objFilePath;
	const char* ddsFilePath;

	GLuint vertexArrayObject;
	GLuint objVertexbuffer;
	GLuint objUVbuffer;
	int numVertices;


	GLuint Texture;
	
	float speed = 3.0f;

public:
	glm::mat4 modelMatrix;
	glm::vec3 loc;

	Sprite3d(GLFWwindow* glfwWindow, const char* objFP, const char* ddsFilePath);
	~Sprite3d();
	void drawSpriteTriangles();

	float getSpeed();

	static void initShader();
	static GLuint* getShader();
};


#endif
