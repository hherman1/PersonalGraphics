#pragma once
#include "Graphics.h"
#include "Framebuffer.h"
#include <vector>
class DepthCubemap
{
public:
	DepthCubemap();
	~DepthCubemap();
protected:
	GLuint _cubemap;
	Framebuffer _framebuffer;
};

