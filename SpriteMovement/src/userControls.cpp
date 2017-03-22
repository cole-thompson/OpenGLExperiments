#include "userControls.hpp"
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Sprite3d.hpp"
#include "Shape2d.hpp"

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

double lastPauseTime = 0;
double buttonRepeatSpeed = 0.2; //seconds


glm::mat4 getVMatrix() {
	return viewMatrix;
}

glm::mat4 getPMatrix() {
	return projectionMatrix;
}

float absValue(float x) {
	if (x < 0) {
		return x * -1;
	}
	return x;
}

//checks that a potential coordinate is within the max bounds of the world
bool isWithinBounds(glm::vec3 tempPosition) {
	return (absValue(tempPosition[0]) < WORLDSIZE) && (absValue(tempPosition[1]) < WORLDSIZE) && (absValue(tempPosition[2]) < WORLDSIZE);
}

void pause(GLFWwindow* window) {
	lastPauseTime = glfwGetTime();
	double currentTime = glfwGetTime();
	Shape2d* resume = new Shape2d(window, "recources/textures/resume.DDS", 0.0, 0.0, 0.5, 0.5);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	while (!glfwWindowShouldClose(window) && (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) && !(*resume).isClicked() &&
		!(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS  && currentTime - lastPauseTime > buttonRepeatSpeed)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		(*resume).drawSpriteTriangles();

		glfwSwapBuffers(window);
		glfwPollEvents();
		currentTime = glfwGetTime();
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	int windowW, windowH;
	glfwGetWindowSize(window, &windowW, &windowH);
	glfwSetCursorPos(window, windowW / 2, windowH / 2);
	lastPauseTime = glfwGetTime();
}


void computeMVPMatrices(GLFWwindow* window, Sprite3d & controlledSprite) {
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
	
	//pause
	if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) && glfwGetTime() - lastPauseTime > buttonRepeatSpeed) {
		pause(window);
	}

	glm::vec3 tempPosition = userPosition;
	//Forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		tempPosition += directionDiffVector * timeDiff * userSpeed;
	}
	//Backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		tempPosition -= directionDiffVector * timeDiff * userSpeed;
	}
	//Right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		tempPosition += rightVector * timeDiff * userSpeed;
	}
	//Left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		tempPosition -= rightVector * timeDiff * userSpeed;
	}
	//Up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		tempPosition += upVector * timeDiff * userSpeed;
	}
	//Down
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		tempPosition -= upVector * timeDiff * userSpeed;
	}
	if (isWithinBounds(tempPosition)) {
		userPosition = tempPosition;
	}

	//object
	tempPosition = controlledSprite.loc;
	glm::mat4 tempMatrix = controlledSprite.modelMatrix;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		//controlledSprite x--
		float a = (float)timeDiff * controlledSprite.getSpeed();
		tempMatrix = glm::translate(tempMatrix, glm::vec3(-a, 0.0f, 0.0f));
		tempPosition[0] -= a;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		//x++
		float a = (float)timeDiff * controlledSprite.getSpeed();
		tempMatrix = glm::translate(tempMatrix, glm::vec3(a, 0.0f, 0.0f));
		tempPosition[0] += a;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		//z--
		float a = (float)timeDiff * controlledSprite.getSpeed();
		tempMatrix = glm::translate(tempMatrix, glm::vec3(0.0f, 0.0f, -a));
		tempPosition[2] -= a;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		//z++
		float a = (float)timeDiff * controlledSprite.getSpeed();
		tempMatrix = glm::translate(tempMatrix, glm::vec3(0.0f, 0.0f, a));
		tempPosition[2] += a;
	}
	if (isWithinBounds(tempPosition)) {
		controlledSprite.loc = tempPosition;
		controlledSprite.modelMatrix = tempMatrix;
	}

	projectionMatrix = glm::perspective(FoV, aspectRatio, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(userPosition, userPosition + directionDiffVector, upVector);
}