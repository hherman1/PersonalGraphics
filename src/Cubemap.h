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
	// reads exactly 6 strings from the array. 
	void loadCubemap(std::string* faces);
};

