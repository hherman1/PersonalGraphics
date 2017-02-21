#include "Mesh.h"

using namespace std;
/*
* Is automatically bound when you instantiate it. 
*/
Mesh::Mesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	bind();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Describes the data being sent through layout(0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat))); // U,V coords for textures
	glEnableVertexAttribArray(2);

}
void Mesh::bind() {
	glBindVertexArray(VAO);
}
void Mesh::unbind() {
	glBindVertexArray(0);
}
//Should bind before calling. 
void Mesh::loadVertexData(GLsizeiptr size, const GLvoid * data, GLenum usage) {
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}
void Mesh::loadIndexData(GLsizeiptr size, const GLvoid * data, GLenum usage) {
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}
void Mesh::setElements(int e) {
	_elements = e;
}
void Mesh::setCount(int e)
{
	_count = e;
}
void Mesh::draw()
{
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
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);

}

IndexedMesh::IndexedMesh():
	Mesh()
{
}
//MUST BIND FIRST!
void IndexedMesh::draw() {
	draw(GL_TRIANGLES, GL_UNSIGNED_INT);
}
// if you want to control the call
void IndexedMesh::draw(GLenum mode, GLenum type) {
	glDrawElements(mode, elements(), type, 0);
}

ArrayMesh::ArrayMesh():
	Mesh()
{
}

void ArrayMesh::draw()
{
	draw(GL_TRIANGLES);
}
void ArrayMesh::draw(GLenum mode)
{
	glDrawArrays(mode, 0, count());
}

void Material::setMaterial(basicgraphics::GLSLProgram & shader)
{
	shader.setUniform("material.ambient", ambient);
	shader.setUniform("material.diffuse", diffuse);
	shader.setUniform("material.specular", specular);
	shader.setUniform("material.shininess", shininess);
}
// BROKEN FUNCTION DO NOT USE
//void Material::setMaterial(string name, basicgraphics::GLSLProgram & shader)
//{
//	
//	shader.setUniform((name + ".ambient").c_str(), ambient);
//	shader.setUniform("material.diffuse", diffuse);
//	shader.setUniform("material.specular", specular);
//	shader.setUniform("material.shininess", shininess);
//}
