#pragma once
#include "Graphics.h"
class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();
	Framebuffer(Framebuffer&& move) = delete;
	Framebuffer(const Framebuffer& that) = delete;

	static void validate();
	GLuint id();
	void bind();
	static void unbind();

protected:
	GLuint _framebuffer;
};

