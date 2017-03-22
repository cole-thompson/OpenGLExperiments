#version 330 core

// Input vertex poition and color
layout(location = 0) in vec2 vertexPositionScreenspace;
layout(location = 1) in vec2 vertexUV;

//output fragment color
out vec2 fragmentUV;

uniform mat4 MVP;

void main(){
	fragmentUV = vertexUV;
    gl_Position = vec4(vertexPositionScreenspace, 0, 1);
}

