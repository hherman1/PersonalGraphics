#include "Texture.h"

using namespace std;


Texture::Texture()
{
	glGenTextures(1, &_texture);
	bind();
}
void Texture::bind(GLuint texname)
{
	glBindTexture(GL_TEXTURE_2D, texname);
}
void Texture::bind() {
	Texture::bind(id());
}
void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);

}
int Texture::width() {
	return _width;
}
int Texture::height() {
	return _height;
}
GLuint Texture::id() {
	return _texture;
}
// bind texture first!!!
// generates texture and its mipmaps. Supplied texture is forced to be level 0. 
void Texture::loadImage(string filename) {
	unsigned char* image = SOIL_load_image(filename.c_str(), &_width, &_height, 0, SOIL_LOAD_RGB);
	if (image == NULL) {
		SOIL_last_result();
		std::runtime_error err(string("Error loading image: ") + string(SOIL_last_result()));
		std::cerr << err.what();
		//throw exception(err.what());
        throw err;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height,0,GL_RGB,GL_UNSIGNED_BYTE,image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
}


Texture::~Texture()
{
	glDeleteTextures(1, &_texture);
}
