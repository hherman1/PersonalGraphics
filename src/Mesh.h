#pragma once
#include "Graphics.h"
#include "GLSLProgram.h"
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
	void setMaterial(basicgraphics::GLSLProgram& shader);
	//void setMaterial(string name, basicgraphics::GLSLProgram & shader);
};

class Mesh {
public:
	explicit Mesh();
	~Mesh();



	Mesh(Mesh&& move) = delete;
	Mesh(const Mesh& that) = delete;
	Mesh& operator=(const Mesh&) = delete;

	int elements();
	int count();
	void bind();
	void unbind();
	void loadVertexData(GLsizeiptr size, const GLvoid * data, GLenum usage);
	void loadIndexData(GLsizeiptr size, const GLvoid * data, GLenum usage);
	void setElements(int e);
	void setCount(int e);
	virtual void draw();
protected:
	int _elements;
	int _count;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

};
class IndexedMesh : public Mesh {
public:
	explicit IndexedMesh();
	IndexedMesh(IndexedMesh&& move) = delete;
	IndexedMesh(const IndexedMesh& that) = delete;
	IndexedMesh& operator=(const IndexedMesh&) = delete;

	int count() = delete;
	int setCount(int e) = delete;
	virtual void draw() override;
	void draw(GLenum mode, GLenum type);
};
class ArrayMesh : public Mesh {
public:
	explicit ArrayMesh();
	ArrayMesh(ArrayMesh&& move) = delete;
	ArrayMesh(const ArrayMesh& that) = delete;
	ArrayMesh& operator=(const ArrayMesh&) = delete;

	int elements() = delete;
	int setElements(int e) = delete;
	virtual void draw() override;
	void draw(GLenum mode);
};