#   OpenGLExperiments
My experiments with OpenGL and my basic starter files for working with graphics. Done in C/C++ with OpenGL, GLM for math and matrices, GLEW for extension handling, and GLFW for handling windows/ user inputs. There are executables in the Debug folder (the shaders folder must be in same directory).

#   Project Descriptions:
# BasicWindow
This project is my simple starter file for opening glfw windows and importing all the libraries. 
# Basic3DModel
This project builds off of BasicWindow and adds in support for shaders. shaders.cpp reads input for GLSL code from files. A cube is generated from a buffer of vectors, and is set inside a VAO with a corresponding color buffer. A simple Model View Projection matrix is set up to show the cube from an outside perspective. My simple vertex shader applies the MVP matrix to each vertex, and my fragment shader assigns the color from the VBO buffer. drawTriangles function draws the triangles from the vertexes in the buffers.
# BasicUserMovement
This project adds user control to Basic3DModel. It adjusts the ModelViewProjection matrix every loop in response to user controls. Allows the user to float and move arount the cube.
Controls: W-fwd A-left S-back D-right SPACE-up LSHIFT-down
