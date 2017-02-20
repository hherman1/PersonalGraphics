#include "VertexArray.h"


using namespace glm;

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &_id);
}
void VertexArray::bind() {
	glBindVertexArray(_id);
}


VertexArray::~VertexArray()
{
}
