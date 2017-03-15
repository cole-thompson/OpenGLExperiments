#version 330 core

//Input fragment color
in vec3 fragmentColor;

// Ouput
out vec3 color;

void main() {
	color = fragmentColor;
}