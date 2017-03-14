#include "userControls.hpp"
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


glm::mat4 viewMatrix = glm::mat4(1.0);
glm::mat4 projectionMatrix = glm::mat4(1.0);

glm::vec3 userPosition = glm::vec3(0, 0, 5); //on +Z
float horizontalAngle = 3.14f;	//-Z
float verticalAngle = 0.0f;

float userSpeed = 3.0f;
float mouseSpeed = 0.005f;

//Projection Matrix
float FoV = 45.0f;
float aspectRatio = 4.0f / 3.0f;



glm::mat4 getVMatrix() {
	return viewMatrix;
}

glm::mat4 getPMatrix() {
	return projectionMatrix;
}



void computeMVPMatrices(GLFWwindow* window) {
	static double lastTimeStep = glfwGetTime();
	double currentTimeStep = glfwGetTime();
	float timeDiff = float(currentTimeStep - lastTimeStep);
	lastTimeStep = currentTimeStep;

	double xMouse, yMouse;
	glfwGetCursorPos(window, &xMouse, &yMouse);

	int windowW, windowH;
	glfwGetWindowSize(window, &windowW, &windowH);
	glfwSetCursorPos(window, windowW / 2, windowH / 2);

	horizontalAngle += mouseSpeed * float(windowW / 2 - xMouse);
	verticalAngle += mouseSpeed * float(windowH / 2 - yMouse);

	//Current Direction
	glm::vec3 directionDiffVector(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	//Right vector
	glm::vec3 rightVector = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	//Up vector
	glm::vec3 upVector = glm::cross(rightVector, directionDiffVector);

	//Forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		userPosition += directionDiffVector * timeDiff * userSpeed;
	}
	//Backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		userPosition -= directionDiffVector * timeDiff * userSpeed;
	}
	//Right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		userPosition += rightVector * timeDiff * userSpeed;
	}
	//Left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		userPosition -= rightVector * timeDiff * userSpeed;
	}
	//Up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		userPosition += upVector * timeDiff * userSpeed;
	}
	//Down
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		userPosition -= upVector * timeDiff * userSpeed;
	}


	projectionMatrix = glm::perspective(FoV, aspectRatio, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(userPosition, userPosition + directionDiffVector, upVector);
}