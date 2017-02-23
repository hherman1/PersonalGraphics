#pragma once
#include <SOIL.h>
#include <string>
#include "Graphics.h"

//No cubemap support. 
class Texture
{
public:
	Texture();
	static void bind(GLenum target, GLuint texname);
	void bind(GLenum target);
	static void unbind(GLenum target);
	Texture(Texture&& move) = delete;
	Texture(const Texture& that) = delete;


	GLuint id();
	~Texture();
protected:
	GLuint _texture;
};

