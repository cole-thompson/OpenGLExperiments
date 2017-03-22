
#ifndef SHAPE2D_HPP
#define SHAPE2D_HPP


#include <vector>

/**
 * Shapes exist in the 2d plane on the screen. Shapes can be checked for clicks.
 * Shapes are rectangles by default, but they can be other shapes (with a vertex buffer, uv buffer, and texture).
 * The click box is determined by the loc, width, and height.
 */

class Shape2d {
private:
	GLFWwindow* window;

	static GLuint shader;
	//mySampler2D uniform variable 
	static GLuint textureUniform;

	const char* ddsFilePath;
	GLuint Texture;

	GLuint vertexArrayObject;
	GLuint objVertexbuffer;
	GLuint objUVbuffer;
	int numVertices;

public:
	glm::vec2 loc;
	float width;
	float height;
	std::vector<glm::vec2> vertex_buffer_data;
	std::vector<glm::vec2> uv_buffer_data;

	Shape2d(GLFWwindow* glfwWindow, const char* ddsFP, float xPos, float yPos, float w, float h);
	~Shape2d();

	void drawSpriteTriangles();
	bool isClicked();
	void setShape(std::vector<glm::vec2> vertex_buffer_data, std::vector<glm::vec2> uv_buffer_data, glm::vec2 pos, float w, float h);
	void setTexture(const char* ddsFP);


	static GLuint* getShader();
	static void initShader();
};


#endif