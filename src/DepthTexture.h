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
	Texture& texture();
	void setShaderTransform(glm::mat4 transform);
protected:
	Framebuffer _framebuffer;
	Texture _texture;

};

