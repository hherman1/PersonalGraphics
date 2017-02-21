#include "Framebuffer.h"



Framebuffer::Framebuffer()
{
	_framebuffer = 0;
	glGenFramebuffers(1, &_framebuffer);
	bind();
}


Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &_framebuffer);
}

void Framebuffer::validate() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Framebuffer not complete");
}

GLuint Framebuffer::id()
{
	return _framebuffer;
}

void Framebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
}

void Framebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
