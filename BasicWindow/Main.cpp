#include <stdio.h>


#include <glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

//global variables
GLFWwindow* window;

//other functions

//when user closes window
void window_close_callback(GLFWwindow *window) {
	//dont let user end program  glfwSetWindowShouldClose(window, GLFW_FALSE);
}

//when user resizes window
void window_size_callback(GLFWwindow* window, int width, int height) {

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
	window = glfwCreateWindow(1024, 768, "OpenGL Practice", NULL, NULL);
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

	//set up callbacks for window
	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	//if decorated window, there is a frame around the window
	int frame_left_size, frame_top_size, framme_right_size, frame_bottom_size;
	glfwGetWindowFrameSize(window, &frame_left_size, &frame_top_size, &framme_right_size, &frame_bottom_size);

	//window settings
	//glfwSetWindowSizeLimits(window, 200, 200, 400, 400);
	//glfwSetWindowAspectRatio(window, 16, 9);

	while (!glfwWindowShouldClose(window)) {
		//clear screen
		glClear(GL_COLOR_BUFFER_BIT);

		//front and back buffers
		glfwSwapBuffers(window);

		//poll for events
		glfwPollEvents();
	}

	//close window, end program
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


