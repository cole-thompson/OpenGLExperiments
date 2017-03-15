#include <stdio.h>
#include <stdlib.h>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "shaders.hpp"

//global variables
GLFWwindow* window;
int windowW;
int windowH;


//when user resizes window
void window_size_callback(GLFWwindow* window, int width, int height) {
	windowH = height;
	windowW = width;
}

//cube: six sides -> 12 triangles to create
static const GLfloat cube_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

//my random colors
static const GLfloat cube_color_buffer_data[] = {
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	0.0f,	1.0f,
	0.0f,	0.0f,	1.0f,
	0.0f,	0.0f,	1.0f,
	0.0f,	0.0f,	1.0f,
	0.0f,	0.0f,	1.0f,
	0.0f,	0.0f,	1.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	0.0f,	1.0f,
	0.0f,	0.0f,	1.0f,
	0.0f,	0.0f,	1.0f,
	0.0f,	0.0f,	1.0f,
	0.0f,	0.0f,	1.0f,
	0.0f,	0.0f,	1.0f
};

//draws the triangles in the vertex buffer with the color in the buffer, uses shaders
void drawTriangles(GLuint* vertexbuffer, GLuint* colorbuffer, int numTriangles) {
	//attribute buffer 1 for vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
	glVertexAttribPointer(0, 3,	GL_FLOAT, GL_FALSE, 0, (void*)0);

	//attribute buffer 2 for colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, *colorbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 3 * numTriangles);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

glm::mat4 getCubeModelMatrix() {
	//translation*rotation*scale matrices for model (cube)
	return glm::mat4(1.0);
}

//MAIN
int main() {
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
	window = glfwCreateWindow(windowW, windowH, "Basic Cube", NULL, NULL);
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

	//background
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	//No triangles who's normals don't face the camera
	glEnable(GL_CULL_FACE);


	//vertex array object openGL VAO
	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	//create shaders from files with shaders.cpp
	GLuint shaderProgram = LoadShaders("recources/shaders/mvpVertexShaderColor.vert", "recources/shaders/simpleFragmentShader.frag");

	// MVP uniform variable. these are used to communicate with shader. MVP in vertexShader
	GLuint MVPMatrixID = glGetUniformLocation(shaderProgram, "MVP");


	//vertex buffer object openGL VBO setup
	GLuint cubeVertexbuffer;
	glGenBuffers(1, &cubeVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_buffer_data), cube_vertex_buffer_data, GL_STATIC_DRAW);

	//color buffer setup
	GLuint cubeColorbuffer;
	glGenBuffers(1, &cubeColorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeColorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color_buffer_data), cube_color_buffer_data, GL_STATIC_DRAW);

	glm::mat4 projectionMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	//View matrix at (4, 3, 3), looking at origin
	glm::mat4 viewMat = glm::lookAt(glm::vec3(4, 3, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	while (!glfwWindowShouldClose(window)) {
		//clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//use shader
		glUseProgram(shaderProgram);

		//apply model transforms to cube
		glm::mat4 modelMat = getCubeModelMatrix();
		glm::mat4 MVP = projectionMat * viewMat * modelMat;
		glUniformMatrix4fv(MVPMatrixID, 1, GL_FALSE, &MVP[0][0]);	//link MVP matrix to uniform variable for shader

		drawTriangles(&cubeVertexbuffer, &cubeColorbuffer, 12);

		//front and back buffers
		glfwSwapBuffers(window);

		//poll for events
		glfwPollEvents();
	}

	glDeleteBuffers(1, &cubeVertexbuffer);
	glDeleteBuffers(1, &cubeColorbuffer);
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &vertexArray);

	//close window, end program
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

