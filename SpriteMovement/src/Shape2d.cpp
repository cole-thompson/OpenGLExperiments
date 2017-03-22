#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include "shaders.hpp"
#include "textureInput.hpp"
#include "objInput.hpp"
#include "Shape2d.hpp"



//Static shaders for normal screen cooridinates and using textures with uv coordinates

GLuint Shape2d::shader;	
//mySampler2D uniform variable 
GLuint Shape2d::textureUniform;	

void Shape2d::initShader() {
	static const char* vertexShader = "recources/shaders/sprite2dVertexShader.vert";
	static const char* fragmentShader = "recources/shaders/textureFragmentShader.frag";
	shader = loadShaders(vertexShader, fragmentShader);

	//mySampler2D uniform variable 
	textureUniform = glGetUniformLocation(shader, "mySampler2D");
}

GLuint* Shape2d::getShader() {
	return &shader;
}


//Texture and Shape

void Shape2d::setTexture(const char* ddsFP) {
	ddsFilePath = ddsFP;
	glUseProgram(shader);
	//create textures from files with textureInput.cpp
	Texture = loadDDSTexture(ddsFilePath);
}

//pos, width, and height determine click box
void Shape2d::setShape(std::vector<glm::vec2> vertexBuffer, std::vector<glm::vec2> uvBuffer, glm::vec2 pos, float w, float h) {
	glfwMakeContextCurrent(window);

	loc = pos;
	height = h;
	width = w;

	numVertices = vertexBuffer.size();

	if (loc[0] > 0 || loc[1] > 0) {
		for (int i = 0; i < numVertices; i++) {
			vertexBuffer[i] += loc;
		}
	}

	vertex_buffer_data = vertexBuffer;
	uv_buffer_data = uvBuffer;

	glUseProgram(shader);
	//create textures from files with textureInput.cpp
	Texture = loadDDSTexture(ddsFilePath);

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	//vertex buffer object openGL VBO setup
	glGenBuffers(1, &objVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, objVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, (numVertices * sizeof(glm::vec2)), &vertex_buffer_data[0], GL_STATIC_DRAW);

	//uv buffer setup
	glGenBuffers(1, &objUVbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, objUVbuffer);
	glBufferData(GL_ARRAY_BUFFER, (numVertices * sizeof(glm::vec2)), &uv_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


//Constructor, Destructor

Shape2d::Shape2d(GLFWwindow* glfwWindow, const char* ddsFP, float xPos, float yPos, float w, float h) {
	window = glfwWindow;

	//rectangle around click box
	glm::vec2 bottom_left = loc + glm::vec2(-w / 2.0, -h / 2.0);
	glm::vec2 top_left = loc + glm::vec2(-w / 2.0, h / 2.0);
	glm::vec2 bottom_right = loc + glm::vec2(w / 2.0, -h / 2.0);
	glm::vec2 top_right = loc + glm::vec2(w / 2.0, h / 2.0);

	std::vector<glm::vec2> uvBuffer, vertexBuffer;
	vertexBuffer.push_back(bottom_left);
	vertexBuffer.push_back(top_left);
	vertexBuffer.push_back(bottom_right);
	vertexBuffer.push_back(top_left);
	vertexBuffer.push_back(bottom_right);
	vertexBuffer.push_back(top_right);

	uvBuffer.push_back(glm::vec2(0.0f, 1.0f));
	uvBuffer.push_back(glm::vec2(0.0f, 0.0f));
	uvBuffer.push_back(glm::vec2(1.0f, 1.0f));
	uvBuffer.push_back(glm::vec2(0.0f, 0.0f));
	uvBuffer.push_back(glm::vec2(1.0f, 1.0f));
	uvBuffer.push_back(glm::vec2(1.0f, 0.0f));

	setTexture(ddsFP);
	setShape(vertexBuffer, uvBuffer, glm::vec2(xPos, yPos), w, h);
}

Shape2d::~Shape2d() {
	glfwMakeContextCurrent(window);
	glBindVertexArray(vertexArrayObject);
	glDeleteBuffers(1, &objVertexbuffer);
	glDeleteBuffers(1, &objUVbuffer);
	glDeleteTextures(1, &textureUniform);
	glDeleteVertexArrays(1, &vertexArrayObject);
}



//draws the triangles in the vertex buffer with the UV's, uses shaders
void Shape2d::drawSpriteTriangles() {
	glfwMakeContextCurrent(window);
	glUseProgram(shader);
	glBindVertexArray(vertexArrayObject);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(textureUniform, 0);

	//attribute buffer 1 for vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, objVertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0
		2,                  // size
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


//determines if the shape has been clicked
bool Shape2d::isClicked() {
	glfwMakeContextCurrent(window);
	glm::vec2 top_right = loc + glm::vec2(width / 2.0, height / 2.0);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		int windowW, windowH;
		glfwGetWindowSize(window, &windowW, &windowH);
		double mouseXpx, mouseYpx;
		glfwGetCursorPos(window, &mouseXpx, &mouseYpx);

		glm::vec2 mouseFl = { ((float)mouseXpx / (float)windowW - 0.5) * 2.0, ((float)mouseYpx / (float)windowH - 0.5) * 2.0 };
		float xDist = top_right[0] - mouseFl[0];
		float yDist = top_right[1] - mouseFl[1];
		
		if ((xDist > width) || (xDist < 0) || (yDist > height) || (yDist < 0)) {
			return false;
		}
		return true;
	}
	return false;
}

