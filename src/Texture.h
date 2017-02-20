#pragma once
#include <SOIL.h>
#include <string>
#include "Graphics.h"

class Texture
{
public:
	Texture();
	void bind();
	void unbind();
	int width();
	Texture(Texture&& move) = delete;

	int height();

	GLuint id();

	void loadImage(std::string filename);
	~Texture();
protected:
	int _width, _height;
	GLuint _texture;
};

