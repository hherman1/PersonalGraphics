#pragma once
#include "Graphics.h"

class Mesh {
public:
	Mesh();
	Mesh(Mesh&& move) = delete;
	void bind();
	void unbind();
	void loadVertexData(GLsizeiptr size, const GLvoid * data, GLenum usage);
	void loadIndexData(GLsizeiptr size, const GLvoid * data, GLenum usage);
	~Mesh();
protected:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

};