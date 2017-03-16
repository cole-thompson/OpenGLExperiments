#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <glew.h>
#include <glfw3.h>

#include "textureInput.hpp"

//texture compression types
#define FOURCC_DXT1 0x31545844 //ASCII for types of DDS files
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

GLuint loadDDSTexture(const char * imagepath) {
	char header[124];
	FILE *ddsFile = fopen(imagepath, "rb");
	if (ddsFile == NULL) {
		fprintf(stderr, "DDS Texture File Not Found: %s\n", imagepath);
		return 0;
	}

	//check for dds: first 4 chars of a DDS file is part of the DWORD, which is "DDS " then a number
	//also check to make sure that the file is not shorter than the header
	char ddsCheck[4];
	if ((fread(&ddsCheck, 4, 1, ddsFile) != 1) || strncmp(ddsCheck, "DDS ", 4) != 0 || (fread(&header, 124, 1, ddsFile) != 1)) {
		fclose(ddsFile);
		return 0;
	}
	
	uint32_t height = *(uint32_t*)&(header[8]);		//takes the next 32 bits starting at the index, stores as int type
	uint32_t width = *(uint32_t*)&(header[12]);
	uint32_t linearSize = *(uint32_t*)&(header[16]);
	uint32_t mipMapCount = *(uint32_t*)&(header[24]);
	uint32_t fourCC = *(uint32_t*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;

	//mipmaps add 1/3 amount of bits in original texture
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = new unsigned char[bufsize * sizeof(unsigned char)];
	fread(buffer, 1, bufsize, ddsFile);
	
	fclose(ddsFile);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC) {
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			free(buffer);
			return 0;
	}

	GLuint textureGL;
	glGenTextures(1, &textureGL);
	glBindTexture(GL_TEXTURE_2D, textureGL);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//these compression algroithms convert 4x4 blocks of pixels into bit vectors either 64b or 128b based on type
	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int currentBufferIndex = 0;

	//load compressed mipmaps
	for (unsigned int mipMapNum = 0; (mipMapNum < mipMapCount) && (width || height); mipMapNum++) {
		//(x+3)/4 is used so that it always rounds up
		//convert 4x4 blocks of pixels into bit vectors either 64b or 128b
		unsigned int compressedSize = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		//buffer is starting index
		glCompressedTexImage2D(GL_TEXTURE_2D, mipMapNum, format, width, height, 0, compressedSize, buffer + currentBufferIndex);
		currentBufferIndex += compressedSize;
		width /= 2;
		height /= 2;

		// non-power-of-two textures
		if (width < 1) width = 1;
		if (height < 1) height = 1;
	}

	delete buffer;

	return textureGL;
}