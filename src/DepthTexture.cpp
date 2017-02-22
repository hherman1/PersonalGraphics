#include "DepthTexture.h"

using namespace basicgraphics;
using namespace glm;
using namespace std;

GLSLProgram depthShader;

//auto unbinds after construction
DepthTexture::DepthTexture():
	_framebuffer(),
	_texture()
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _texture.id(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	Framebuffer::validate();
	Framebuffer::unbind();
}


DepthTexture::~DepthTexture()
{
}

// binds its framebuffer.
// should probably clear after binding to clear previous depth texture.
void DepthTexture::bind()
{
	_framebuffer.bind();
}

void DepthTexture::setViewport()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
}

void DepthTexture::unbind()
{
	Framebuffer::unbind();
	Texture::unbind();
}

Texture& DepthTexture::texture()
{
	return _texture;
}
