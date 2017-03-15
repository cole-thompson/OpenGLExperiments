#version 330 core

// Input vertex poition and color
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

//output fragment color
out vec2 fragmentUV;

uniform mat4 MVP;

void main(){
	fragmentUV = vertexUV;
    gl_Position = MVP*vec4(vertexPosition_modelspace, 1);
}

