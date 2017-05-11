/*
	Load bmp-files from:
	http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
	and adjusted:
*/

/*
	loadTexture to load 24-Bit .bmp-files
	after loading a bmp-file the bitmap-width, bitmap-height
	and bitmap-data can be returned
*/
class Texture
{
public:
	Texture ();
	~Texture();
	GLuint loadTexture(const char* texturePath);
	unsigned int getWidth();
	unsigned int getHeight();
	unsigned char* getData();

private:
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;	
};
