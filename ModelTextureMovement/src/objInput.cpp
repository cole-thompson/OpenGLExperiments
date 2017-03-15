#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <string>

#include "objInput.hpp"

//very limited, only for triangular faces, uvs must be included
bool loadObjFile(const char* objPath, std::vector<glm::vec3>* verticesOutput, std::vector<glm::vec2>* uvsOutput, std::vector<glm::vec3>* normalsOutput) {
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > tempVertices;
	std::vector< glm::vec2 > tempUvs;
	std::vector< glm::vec3 > tempNormals;

	FILE *objFile = fopen(objPath, "r");
	if (objFile == NULL) {
		fprintf(stderr, "OBJ Texture File Not Found: %s\n", objPath);
		return 0;
	}

	std::string firstWordOfLine;
	int res = fscanf(objFile, "%s", &firstWordOfLine[0]);
	firstWordOfLine += "  ";	//in case first word is empty string, or only v, avoids out of bounds
	while (res != EOF) {
		if ((firstWordOfLine[0] == 'v') && (firstWordOfLine[1] == 't')) {
			glm::vec2 uv;
			fscanf(objFile, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y;	//DDS inverted
			tempUvs.push_back(uv);
		}
		else if ((firstWordOfLine[0] == 'v') && (firstWordOfLine[1] == 'n')) {
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tempNormals.push_back(normal);
		}
		else if ((firstWordOfLine[0] == 'v')) {
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tempVertices.push_back(vertex);
		}
		else if ((firstWordOfLine[0] == 'f')) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				fprintf(stderr, "File can't be read by objInput.cpp\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			char clearLine[1000];
			fgets(clearLine, 1000, objFile);
		}
		res = fscanf(objFile, "%s", &firstWordOfLine[0]);
		firstWordOfLine += " ";
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		glm::vec3 vertex = tempVertices[vertexIndex - 1];
		glm::vec2 uv = tempUvs[uvIndex - 1];
		glm::vec3 normal = tempNormals[normalIndex - 1];

		(*verticesOutput).push_back(vertex);
		(*uvsOutput).push_back(uv);
		(*normalsOutput).push_back(normal);
	}
	return true;
}