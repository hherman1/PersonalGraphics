#pragma once
#include "Graphics.h"
#include "Framebuffer.h"
#include "Cubemap.h"
#include <vector>



namespace depthcubemap {
	std::vector<glm::mat4> genTransforms(glm::vec3 pos);
}
class DepthCubemap
{
public:
	Cubemap & cubemap();
	//cubemaps are textures
	void unbind();
	DepthCubemap();
	void bind();
	void setViewport();
	~DepthCubemap();
protected:
	Cubemap _cubemap;
	Framebuffer _framebuffer;
};

