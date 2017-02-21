#pragma once
#include <memory>
#include "Graphics.h"
#include "Texture.h"
#include "Framebuffer.h"
#include "GLSLProgram.h"

class DepthTexture
{
public:
	DepthTexture();
	~DepthTexture();
	static void compileShader();
	void useDepthShader();
	void bind();
	static void unbind();
	std::shared_ptr<Texture> texture();
	void setShaderTransform(glm::mat4 transform);
protected:
	std::shared_ptr<Framebuffer> _framebuffer = NULL;
	std::shared_ptr<Texture> _texture = NULL;

};

