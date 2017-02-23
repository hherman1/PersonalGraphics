#pragma once
#include "Texture.h"
class Cubemap :
	public Texture
{
public:
	Cubemap();
	~Cubemap();
	using Texture::bind;
	void bind();
	//bind first
	void loadCubemap(std::string* faces);
};

