#include <stdio.h>
#include <stdlib.h>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>


#include "shaders.hpp"
#include "userControls.hpp"
#include "textureInput.hpp"
#include "shapeBuffers.hpp"
#include "objInput.hpp"


//global variables
GLFWwindow* window;
int windowW;
int windowH;

//other functions

//when user resizes window
void window_size_callback(GLFWwindow* window, int width, int height) {
	windowH = height;
	windowW = width;
	//glfwSetCursorPos(window, windowW / 2, windowH / 2);
}
int initializeWindow() {
	if (!glfwInit()) {
		fprintf(stderr, "GLFW not initializing");
		return -1;
	}

	//set up GLFW window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	//OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);	//4x antialiasing
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	//macs
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//no old OpenGL
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);	//reszeable window

												//create GLFWwindow and OpenGL context
												//for full screen, make the monitor parameter glfwGetPrimaryMonitor()
	windowW = 1024;
	windowH = 768;
	window = glfwCreateWindow(windowW, windowH, ".OBJ Model, .DDS Texture, and User Movement", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "GLFWwindow not opening");
		glfwTerminate();
		return -1;
	}

	//make window current context
	glfwMakeContextCurrent(window);

	//initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "GLEW Not initializing");
		glfwTerminate();
		return -1;
	}

	//Keyboard
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//Mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, windowW / 2, windowH / 2);


	//set up callbacks for window
	glfwSetWindowSizeCallback(window, window_size_callback);

	//if decorated window, there is a frame around the window
	int frame_left_size, frame_top_size, framme_right_size, frame_bottom_size;
	glfwGetWindowFrameSize(window, &frame_left_size, &frame_top_size, &framme_right_size, &frame_bottom_size);

	//window settings
	//glfwSetWindowSizeLimits(window, 200, 200, 400, 400);
	//glfwSetWindowAspectRatio(window, 16, 9);

	//background
	glClearColor(0.0f, 0.2f, 0.0f, 0.0f);
}

//returns size, or -1 if object didn't load
int loadObjectBuffers(const char* objFilePath, GLuint *vertexArrayObject, GLuint * objVertexbuffer, GLuint * objUVbuffer) {
	glGenVertexArrays(1, vertexArrayObject);
	glBindVertexArray(*vertexArrayObject);
	
	std::vector<glm::vec3> objVertices;
	std::vector<glm::vec2> objUvs;
	std::vector<glm::vec3> objNormals;
	if (!loadObjFile(objFilePath, &objVertices, &objUvs, &objNormals)) {
		return -1;
	}

	//vertex buffer object openGL VBO setup
	glGenBuffers(1, objVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *objVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, (objVertices.size() * sizeof(glm::vec3)), &objVertices[0], GL_STATIC_DRAW);

	//uv buffer setup
	glGenBuffers(1, objUVbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *objUVbuffer);
	glBufferData(GL_ARRAY_BUFFER, (objUvs.size() * sizeof(glm::vec2)), &objUvs[0], GL_STATIC_DRAW);

	return objVertices.size();
}

//draws the triangles in the vertex buffer with the UV's, uses shaders
void drawTriangles(GLuint* vertexbuffer, GLuint* uvbuffer, int numVertices) {
	//attribute buffer 1 for vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//attribute buffer 2 for UV's
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

glm::mat4 getModelMatrix() {
	//translation*rotation*scale matrices for model
	return glm::mat4(1.0);
}

//MAIN
int main() {
	if (initializeWindow() == -1) {
		return -1;
	}
	
	//acccepts fragment closer to camera
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//No triangles who's normals don't face the camera
	//glEnable(GL_CULL_FACE);


	//create shaders from files with shaders.cpp
	GLuint shaderProgram = loadShaders("recources/shaders/mvpVertexShaderUV.vert", "recources/shaders/textureFragmentShader.frag");

	// MVP uniform variable. these are used to communicate with shader. MVP in vertexShader
	GLuint mvpMatrixUniform = glGetUniformLocation(shaderProgram, "MVP");
	
	//create textures from files with textureInput.cpp
	GLuint Texture = loadDDSTexture("recources/textures/checkers.DDS");
	//mySampler2D uniform variable 
	GLuint textureUniform = glGetUniformLocation(shaderProgram, "mySampler2D");
	
	//create a vertex array object out of a .obj file and bind it, add two attribute buffers for verticies and uv's
	//UV coordinates correspond to Texture
	GLuint vertexArrayObject, objVertexbuffer, objUVbuffer;
	int monkeySize = loadObjectBuffers("recources/objects/monkey.obj", &vertexArrayObject, &objVertexbuffer, &objUVbuffer);
	if (monkeySize == -1) {
		return -1;
	}

	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//clear screen

		glUseProgram(shaderProgram);	//use shader

		computeMVPMatrices(window);
		glm::mat4 MVP = getPMatrix() * getVMatrix() * getModelMatrix();
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, &MVP[0][0]);	//link MVP matrix to uniform variable for shader
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(textureUniform, 0);

		drawTriangles(&objVertexbuffer, &objUVbuffer, monkeySize);

		//front and back buffers
		glfwSwapBuffers(window);

		//poll for events
		glfwPollEvents();
	}

	glDeleteBuffers(1, &objVertexbuffer);
	glDeleteBuffers(1, &objUVbuffer);
	glDeleteProgram(shaderProgram);
	glDeleteTextures(1, &textureUniform);
	glDeleteVertexArrays(1, &vertexArrayObject);

	//close window, end program
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}