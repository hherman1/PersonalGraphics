#pragma once
#include <memory>
#include "Graphics.h"
#include "Texture.h"
#include "Framebuffer.h"
#include "GLSLProgram.h"
#include "Light.h"

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;


//Steps to render a depth texture:
// call "use" on your depthShader
// set your orthographic and view matrices for your shader
// bind the depth texture
// set the depth textures viewport
// draw your objects
// unbind depth texture.
class DepthTexture
{
public:
	DepthTexture();
	~DepthTexture();
	DepthTexture(DepthTexture&& move) = delete;
	DepthTexture(const DepthTexture& that) = delete;
	void bind();
	void setViewport();
	static void unbind();
	Texture& texture();
protected:
	Framebuffer _framebuffer;
	Texture _texture;

};

