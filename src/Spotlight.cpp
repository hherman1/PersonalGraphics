#include "Spotlight.h"

using namespace glm;



Spotlight::Spotlight(glm::vec3 _pos, glm::vec3 _lookAt):
	Light(_pos),
	orth(ortho(-10.f, 10.f, -10.f, 10.f, 0.1f, 10.f)),
	lookAt(_lookAt)
{
}

Spotlight::~Spotlight()
{
}

glm::mat4 Spotlight::proj_view()
{
	//glm::ortho()
	return orth * glm::lookAt(position, lookAt, glm::vec3(0, 1, 0));
}
