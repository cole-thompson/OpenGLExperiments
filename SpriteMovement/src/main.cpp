#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


#include "shaders.hpp"
#include "userControls.hpp"
#include "textureInput.hpp"
#include "shapeBuffers.hpp"
#include "objInput.hpp"
#include "sprite3D.hpp"


//global variables
GLFWwindow* window;
int windowW;
int windowH;


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
	window = glfwCreateWindow(windowW, windowH, "Sprites and Bounded Movement", NULL, NULL);
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
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	return 1;
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

	//global shader program in shaders.cpp, allows program to be accessed from Sprites
	initCommonShaderProgram();

	// MVP uniform variable. these are used to communicate with shader. MVP in vertexShader
	GLuint mvpMatrixUniform = glGetUniformLocation(getCommonShaderProgram(), "MVP");

	//generate sprites
	const char* monkeyFilePath = "recources/objects/monkey.obj";
	const char* textureFilePath = "recources/textures/checkers.DDS";
	
	//this Sprite will be controllable with arrow keys
	Sprite3d* monkeySprite = (Sprite3d*)(new Sprite3d(monkeyFilePath, textureFilePath));

	//generate world box
	const char* cubeFilePath = "recources/objects/cube.obj";
	const char* cubeTextureFP = "recources/textures/cubeWorldDXT5.DDS";
	Sprite3d* boundsSprite = (Sprite3d*)(new Sprite3d(cubeFilePath, cubeTextureFP));
	(*boundsSprite).modelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(WORLDSIZE));

	std::vector<Sprite3d*> sprites = {boundsSprite, monkeySprite, (Sprite3d*)(new Sprite3d(cubeFilePath, textureFilePath))};
	(*sprites[2]).modelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.5));

	//main loop
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		//clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//use shader
		glUseProgram(getCommonShaderProgram());

		//monkeySprite is the sprite controlled by the user
		computeMVPMatrices(window, *monkeySprite);	

		for (size_t i = 0; i < sprites.size(); i++) {
			glm::mat4 modelMatrix = (*sprites[i]).modelMatrix;
			glm::mat4 MVP = getPMatrix() * getVMatrix() * modelMatrix;
			glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, &MVP[0][0]);	//link MVP matrix to uniform variable for shader
			(*sprites[i]).drawSpriteTriangles();
		}

		//front and back buffers
		glfwSwapBuffers(window);

		//poll for events
		glfwPollEvents();
	}

	//close window, end program
	glDeleteProgram(getCommonShaderProgram());
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}