#   OpenGLExperiments
My experiments with OpenGL and my basic starter files for working with graphics. Done in C with OpenGL, GLM for math and matrices, GLEW for extension handling, and GLFW for handling windows/ user inputs.

#   Project Descriptions:
# BasicWindow
This project is my simple starter file for opening glfw windows and importing all the libraries. 
# Basic3DModel
This project builds off of BasicWindow and adds in support for shaders. shaders.cpp reads input for GLSL code from files. A cube is generated from a buffer of vectors, and is set inside a VAO with a corresponding color buffer. A simple Model View Projection matrix is set up to show the cube from an outside perspective. My simple vertex shader applies the MVP matrix to each vertex, and my fragment shader assigns the color from the VBO buffer. drawTriangles function draws the triangles from the vertexes in the buffers.