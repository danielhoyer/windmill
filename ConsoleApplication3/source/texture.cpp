#include "../include/main.h"

Texture::Texture()
{

}

Texture::~Texture()
{

}

/*
	Load a 24Bit-.bmp-file:
	open file and check if the .bmp-file is valide
	after that read file and write content into the data-array
*/
GLuint Texture::loadTexture(const char* texturePath)
{
	FILE * file;
	fopen_s(&file, texturePath,"rb");

	if (!file)
	{
		printf("Image could not be opened\n");
		return -1;
	}

	if ( fread(header, 1, 54, file)!=54 )
	{ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return -1;
	}

	if ( header[0]!='B' || header[1]!='M' )
	{
		printf("Not a correct BMP file\n");
		return -1;
	}

	//read data
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)
	{
		imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	}
	if (dataPos==0)
	{
		dataPos=54; // The BMP header is done that way
	}

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	GLuint textureID;
	// Create one OpenGL texture
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	return textureID;
}

/*
	returns .bmp-file's width
*/
unsigned int Texture::getWidth()
{
	return this->width;
}

/*
	returns .bmp-file's height
*/
unsigned int Texture::getHeight()
{
	return this->height;
}

/*
	returns .bmp-file's data
*/
unsigned char* Texture::getData()
{
	return this->data;
}