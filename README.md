### OpenGLExperiments
Cole Thompson
My experiments with OpenGL and my basic starter files for working with graphics. Done in C/C++ and GLSL shader language with OpenGL, GLM for math and matrices, GLEW for extension handling, and GLFW for handling windows and user inputs. There are executables in the Working Projects folder (the recources folder must be in same directory as the .exe). This is a compilation of things I have taught myself from research on the libraries' documentation, tutorials, and articles. The project's goal is to approach graphics from a low-level perpective and understand the math and 

## Project Descriptions:
# BasicWindow
This project is my simple starter file for opening glfw windows and importing all the libraries. 
# Basic3DModel
This project builds off of BasicWindow and adds in support for shaders. shaders.cpp reads input for GLSL code from files. A cube is generated from a buffer of vectors, and is set inside a VAO with a corresponding color buffer. A simple Model View Projection matrix is set up to show the cube from an outside perspective. My simple vertex shader applies the MVP matrix to each vertex, and my fragment shader assigns the color from the VBO buffer. drawTriangles function draws the triangles from the vertexes in the buffers.
# BasicUserMovement
This project adds user control to Basic3DModel. I adjust the ModelViewProjection matrix every loop in response to user controls, then draw the triangles. The vertex shader recieves the MVP matrix and adjusts each vertex. Allows the user to float and move arount the cube.
Controls: ESC-exit W-fwd A-left S-back D-right SPACE-up LSHIFT-down
# ModelTextureMovement
This project started with BasicUserMovement and added functions for loading 3D objects with associated textures from files. Wrote a simple function loadObjFile that reads 3D models from limited .obj files (objInput.cpp), and loads it's vectors, UV coordinates, and normals into buffers. Also wrote a function loadDDSTexture to read textures with mipmaps from .dds files (textureInput.cpp), and apply the colors to vertices using UV coordinates in the shaders. The files used are simple .obj from Blender and and .dds formatted with Compressonator.
