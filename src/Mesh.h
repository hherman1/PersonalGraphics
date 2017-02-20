#pragma once
#include "Graphics.h"
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh {
public:
	explicit Mesh();
	~Mesh();



	Mesh(Mesh&& move) = delete;
	Mesh(const Mesh& that) = delete;

	void bind();
	void unbind();
	void loadVertexData(GLsizeiptr size, const GLvoid * data, GLenum usage);
	void loadIndexData(GLsizeiptr size, const GLvoid * data, GLenum usage);
protected:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

};