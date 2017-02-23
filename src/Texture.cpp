#include "Texture.h"

using namespace std;


Texture::Texture()
{
	glGenTextures(1, &_texture);
}
void Texture::bind(GLenum target, GLuint texname)
{
	glBindTexture(target, texname);
}
void Texture::bind(GLenum target) {
	Texture::bind(target,id());
}
void Texture::unbind(GLenum target) {
	glBindTexture(target, 0);
}
GLuint Texture::id() {
	return _texture;
}



Texture::~Texture()
{
	glDeleteTextures(1, &_texture);
}
