#pragma once
#include "Graphics.h"
#include "GLSLProgram.h"
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Indices {
	GLuint EBO;
	int elements;
};

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};
struct GPUMeshReference {
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};
struct IndexedGPUMeshReference {
	int elements;
	GPUMeshReference meshReference;
};

namespace gl_mesh {
	void unbind();
	void loadVertexData(GLsizeiptr size, const GLvoid * data, GLenum usage);
	void loadIndexData(GLsizeiptr size, const GLvoid * data, GLenum usage);
	GPUMeshReference makeMesh();
	void deleteMesh(GPUMeshReference meshReference);
	void bind(GPUMeshReference mesh);
}

class IndexedMeshes {
public:
	std::vector<IndexedGPUMeshReference> indexedGPUReferences;

	IndexedMeshes();
	IndexedMeshes(IndexedMeshes&& move) = delete;
	IndexedMeshes(const IndexedMeshes& that) = delete;
	IndexedMeshes& operator=(const IndexedMeshes&) = delete;

	~IndexedMeshes();
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
protected:
	int _elements;
	int _count;
	GPUMeshReference mesh_reference;

};

class IndexedMesh : public Mesh {
public:
	explicit IndexedMesh();
	IndexedMesh(IndexedMesh&& move) = delete;
	IndexedMesh(const IndexedMesh& that) = delete;
	IndexedMesh& operator=(const IndexedMesh&) = delete;

	int count() = delete;
	int setCount(int e) = delete;
};
class ArrayMesh : public Mesh {
public:
	explicit ArrayMesh();
	ArrayMesh(ArrayMesh&& move) = delete;
	ArrayMesh(const ArrayMesh& that) = delete;
	ArrayMesh& operator=(const ArrayMesh&) = delete;

	int elements() = delete;
	int setElements(int e) = delete;
};