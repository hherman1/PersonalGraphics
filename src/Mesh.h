#pragma once
#include "Graphics.h"
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

class Mesh {
public:
	explicit Mesh();
	~Mesh();



	Mesh(Mesh&& move) = delete;
	Mesh(const Mesh& that) = delete;

	int elements();
	int count();
	void bind();
	void unbind();
	void loadVertexData(GLsizeiptr size, const GLvoid * data, GLenum usage);
	void loadIndexData(GLsizeiptr size, const GLvoid * data, GLenum usage);
	void setElements(int e);
	void setCount(int e);
protected:
	int _elements;
	int _count;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

};