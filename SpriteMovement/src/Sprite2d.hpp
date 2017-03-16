
#ifndef SPRITE2D_HPP
#define SPRITE2D_HPP

class Sprite2d {
private:
	const char* ddsFilePath;

	GLuint textShader;
	GLuint vertexArrayObject;
	GLuint objVertexbuffer;
	GLuint objUVbuffer;
	int numVertices;

	GLuint Texture;
	GLuint textureUniform;


public:
	//glm::mat4 modelMatrix;
	//glm::vec3 loc;

	Sprite2d(const char* ddsFilePath);
	~Sprite2d();
	void drawSpriteTriangles();

};


#endif