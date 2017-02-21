#include "Mesh.h"

using namespace std;
/*
* Is automatically bound when you instantiate it. 
*/

namespace gl_mesh {
	GPUMeshReference makeMesh() {
		GPUMeshReference mesh;
		glGenVertexArrays(1, &mesh.VAO);
		glGenBuffers(1, &mesh.VBO);
		glGenBuffers(1, &mesh.EBO);

		glBindVertexArray(mesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);

		//Describes the data being sent through layout(0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // colors
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat))); // U,V coords for textures
		glEnableVertexAttribArray(2);
		return mesh;
	}
	void deleteMesh(GPUMeshReference meshReference) {
		glDeleteBuffers(1, &meshReference.VBO);
		glDeleteBuffers(1, &meshReference.EBO);
		glDeleteVertexArrays(1, &meshReference.VAO);
	}

	void bind(GPUMeshReference meshReference) {
		glBindVertexArray(meshReference.VAO);
	}
	void unbind() {
		glBindVertexArray(0);
	}
	void loadVertexData(GLsizeiptr size, const GLvoid * data, GLenum usage) {
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	}
	void loadIndexData(GLsizeiptr size, const GLvoid * data, GLenum usage) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
	}

}


Mesh::Mesh()
{
	mesh_reference = gl_mesh::makeMesh();
}
void Mesh::bind() {
	gl_mesh::bind(mesh_reference);
}
void Mesh::unbind() {
	gl_mesh::unbind();
}
//Should bind before calling. 
void Mesh::loadVertexData(GLsizeiptr size, const GLvoid * data, GLenum usage) {
	gl_mesh::loadVertexData(size,data,usage);
}
void Mesh::loadIndexData(GLsizeiptr size, const GLvoid * data, GLenum usage) {
	gl_mesh::loadIndexData(size, data, usage);
}
void Mesh::setElements(int e) {
	_elements = e;
}
void Mesh::setCount(int e)
{
	_count = e;
}
int Mesh::elements() {
	return _elements;
}

int Mesh::count()
{
	return _count;
}

Mesh::~Mesh()
{
	gl_mesh::deleteMesh(mesh_reference);
}

IndexedMesh::IndexedMesh():
	Mesh()
{
}

ArrayMesh::ArrayMesh():
	Mesh()
{
}

IndexedMeshes::IndexedMeshes()
{
}

IndexedMeshes::~IndexedMeshes()
{
	for (auto i = indexedGPUReferences.begin();i < indexedGPUReferences.end(); i++) {
		gl_mesh::deleteMesh((*i).meshReference);
	}
}
