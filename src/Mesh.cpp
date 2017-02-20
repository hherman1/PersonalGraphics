#include "Mesh.h"

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

Mesh::~Mesh()
{
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);

}
