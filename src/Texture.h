#pragma once
#include <SOIL.h>
#include <string>
#include "Graphics.h"

//No cubemap support. 
class Texture
{
public:
	Texture();
	static void bind(GLuint texname);
	void bind();
	static void unbind();
	int width();
	Texture(Texture&& move) = delete;
	Texture(const Texture& that) = delete;


	int height();

	GLuint id();

	void loadImage(std::string filename);
	//sets the texture to be a plain white texture. useful for untextured objects. 
	void whiteTexture();
	~Texture();
protected:
	int _width, _height;
	GLuint _texture;
};

