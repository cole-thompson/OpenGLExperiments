# OpenGLExperiments
Cole Thompson<br />
My experiments with OpenGL and my basic starter files for working with graphics. Done in C/C++ and GLSL shader language with OpenGL, GLM for math and matrices, GLEW for extension handling, and GLFW for handling windows and user inputs. There are executables in the Working Projects folder (the recources folder must be in same directory as the .exe). This is a compilation of things I have taught myself from research on the libraries' documentation, tutorials, and articles. The project's goal is to approach graphics from a math oriented, low-level technology perpective.

## Project Descriptions:
###   SpriteMovement
This project built off of my basic movement and 3d rendering code. I modulated common features of an object into a Sprite3d class. Sprite3d's consist of a location, a Model matrix to represent transformations (such as move, rotate, and scale) to be applied next step, a bmp 3D file, a dds texture file, and vertex and uv coordinate buffers. Sprites have a draw method that is called each step from main. Multiple Sprites can exist at once, and there is a vector to handle all of them in the main loop. One of the Sprites can be controlled by user input from the arrow keys to show how the transformation matrixes can be applied. Another large sprite is used as a simple way to display the edges of the "world" based on movement bounds; the user can not move or move objects outside of this area.<br />
Controls: ESC-exit    W-fwd A-left S-back D-right   SPACE-up LSHIFT-down <br />
ARROWKEYS-object movement in horizontal plane
###   ModelTextureMovement
This project started with BasicUserMovement and added functions for loading 3D objects with associated textures from files. Wrote a simple function loadObjFile that reads 3D models from limited .obj files (objInput.cpp), and loads it's vectors, UV coordinates, and normals into buffers. Also wrote a function loadDDSTexture to read textures with mipmaps from .dds files (textureInput.cpp), and apply the colors to vertices using UV coordinates in the shaders. The files used are simple .obj from Blender and and .dds formatted with Compressonator.
###   BasicUserMovement
This project adds user control to Basic3DModel. I adjust the ModelViewProjection matrix every loop in response to user controls, then draw the triangles. The vertex shader recieves the MVP matrix and adjusts each vertex. Allows the user to float and move arount the cube.<br />
Controls: ESC-exit    W-fwd A-left S-back D-right   SPACE-up LSHIFT-down
###   Basic3DModel
This project builds off of BasicWindow and adds in support for shaders. shaders.cpp reads input for GLSL code from files. A cube is generated from a buffer of vectors, and is set inside a VAO with a corresponding color buffer. A simple Model View Projection matrix is set up to show the cube from an outside perspective. My simple vertex shader applies the MVP matrix to each vertex, and my fragment shader assigns the color from the VBO buffer. drawTriangles function draws the triangles from the vertexes in the buffers.
###   BasicWindow
This project is my simple starter file for opening glfw windows and importing all the libraries.
