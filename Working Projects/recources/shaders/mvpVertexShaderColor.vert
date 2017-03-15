#version 330 core

// Input vertex poition and color
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

//output fragment color
out vec3 fragmentColor;

uniform mat4 MVP;

void main(){
	fragmentColor = vertexColor;
    gl_Position = MVP*vec4(vertexPosition_modelspace, 1);
}

