#include "DepthTexture.h"

using namespace basicgraphics;
using namespace glm;
using namespace std;

GLSLProgram depthShader;

DepthTexture::DepthTexture():
	_framebuffer(),
	_texture()
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _texture.id(), 0);
	glDrawBuffer(GL_NONE);
	Framebuffer::validate();
	Framebuffer::unbind();
}


DepthTexture::~DepthTexture()
{
}

void DepthTexture::compileShader()
{
	depthShader.compileShader("depth.vert");
	depthShader.compileShader("depth.frag");
	depthShader.link();
}
// Make sure compileShader(); is called at some point before calling this. 
void DepthTexture::useDepthShader()
{
	depthShader.use();
}

// binds a framebuffer and a texture. Must change active texture before binding extra textures.
// should probably clear after binding to clear previous depth texture.
void DepthTexture::bind()
{
	_framebuffer.bind();
	_texture.bind();
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

void DepthTexture::setShaderTransform(glm::mat4 transform)
{
	depthShader.setUniform("transform", transform);
}
