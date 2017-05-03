#pragma once
#include "Texture.h"
#include <string>
class Texture2D : public Texture
{
public:
	Texture2D();
	using Texture::bind;
	void bind();
	using Texture::unbind;
	static void unbind();
	int width();
	int height();
	void setWidth(int w);
	void setHeight(int h);
	void loadImage(std::string filename);
	void whiteTexture();
	~Texture2D();
protected:
	int _width, _height;

};

