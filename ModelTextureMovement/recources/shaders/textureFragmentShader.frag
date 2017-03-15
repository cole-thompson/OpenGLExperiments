#version 330 core

//Input fragment color
in vec2 fragmentUV;

// Ouput
out vec3 color;

uniform sampler2D mySampler2D;
void main() {
	color = texture(mySampler2D, fragmentUV).rgb;
}