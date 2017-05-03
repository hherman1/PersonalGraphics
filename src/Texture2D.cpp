#include "Texture2D.h"

using namespace std;

Texture2D::Texture2D()
{
	bind();
}
void Texture2D::bind() {
	bind(GL_TEXTURE_2D);
}
void Texture2D::unbind() {
	unbind(GL_TEXTURE_2D);
}
int Texture2D::width() {
	return _width;
}
int Texture2D::height() {
	return _height;
}
void Texture2D::setWidth(int w)
{
	_width = w;
}
void Texture2D::setHeight(int h)
{
	_height = h;
}
// bind texture first!!!
// generates texture and its mipmaps. Supplied texture is forced to be level 0. 
void Texture2D::loadImage(string filename) {
	unsigned char* image = SOIL_load_image(filename.c_str(), &_width, &_height, 0, SOIL_LOAD_RGB);
	if (image == NULL) {
		SOIL_last_result();
		std::runtime_error err(string("Error loading image: ") + string(SOIL_last_result()));
		std::cerr << err.what();
		//throw exception(err.what());
		throw err;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
}
void Texture2D::whiteTexture() {
	//Texture tex;
	char pixel[3] = { static_cast<char>(255),static_cast<char>(255),static_cast<char>(255) };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
}

Texture2D::~Texture2D()
{
}
