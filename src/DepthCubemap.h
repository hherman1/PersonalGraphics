#pragma once
#include "Graphics.h"
#include "Framebuffer.h"
#include <vector>



namespace depthcubemap {
	std::vector<glm::mat4> genTransforms(glm::vec3 pos);
}
class DepthCubemap
{
public:
	//cubemaps are textures
	GLuint cubemap();
	void unbind();
	DepthCubemap();
	void bind();
	~DepthCubemap();
protected:
	GLuint _cubemap;
	Framebuffer _framebuffer;
};

