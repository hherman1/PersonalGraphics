#pragma once
#include "Texture2D.h"
#include "Framebuffer.h"
class DeferredGeom
{
public:
	DeferredGeom(int width, int height);
	~DeferredGeom();
	DeferredGeom(DeferredGeom&& move) = delete;
	DeferredGeom(const DeferredGeom& that) = delete;

	void bind();
	void unbind();
	Texture2D & material();
	Texture2D & normal();
	Texture2D & position();
protected:
	int _width, _height;
	Framebuffer _buf;
	Texture2D _material;
	Texture2D _normal;
	Texture2D _position;
	GLuint _depthbuffer;
};

