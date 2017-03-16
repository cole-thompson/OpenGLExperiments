#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include "shaders.hpp"
#include "textureInput.hpp"
#include "objInput.hpp"
#include "sprite3D.hpp"

/*
* This is my implementation of a simple Sprite that is loaded from a obj file
* Each sprite can have a texture
* Model matrix for each step, which allows for transformations
*/

Sprite3d::Sprite3d(const char* objFP, const char* ddsFilePath) {
	//create textures from files with textureInput.cpp
	Texture = loadDDSTexture(ddsFilePath);
	//mySampler2D uniform variable 
	textureUniform = glGetUniformLocation(getCommonShaderProgram(), "mySampler2D");

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	std::vector<glm::vec3> objVertices;
	std::vector<glm::vec2> objUvs;
	std::vector<glm::vec3> objNormals;

	objFilePath = objFP;
	if (!loadObjFile(objFilePath, &objVertices, &objUvs, &objNormals)) {
		fprintf(stderr, "Sprite not created, obj file not loaded.");
		objVertices = {glm::vec3(0)};
		objUvs = {glm::vec2(0)};
		objNormals = {glm::vec3(0)};
	}

	numVertices = objVertices.size();

	//vertex buffer object openGL VBO setup
	glGenBuffers(1, &objVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, objVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, (objVertices.size() * sizeof(glm::vec3)), &objVertices[0], GL_STATIC_DRAW);

	//uv buffer setup
	glGenBuffers(1, &objUVbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, objUVbuffer);
	glBufferData(GL_ARRAY_BUFFER, (objUvs.size() * sizeof(glm::vec2)), &objUvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	modelMatrix = glm::mat4(1.0f);
	loc = glm::vec3(0.0f);
}

Sprite3d::~Sprite3d() {
	glBindVertexArray(vertexArrayObject);
	glDeleteBuffers(1, &objVertexbuffer);
	glDeleteBuffers(1, &objUVbuffer);
	glDeleteTextures(1, &textureUniform);
	glDeleteVertexArrays(1, &vertexArrayObject);
}

//draws the triangles in the vertex buffer with the UV's, uses shaders
void Sprite3d::drawSpriteTriangles() {
	glBindVertexArray(vertexArrayObject);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(textureUniform, 0);

	//attribute buffer 1 for vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, objVertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	//attribute buffer 2 for UV's
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, objUVbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

float Sprite3d::getSpeed() {
	return speed;
}