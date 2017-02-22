#include "Spotlight.h"

using namespace glm;



Spotlight::Spotlight(glm::vec3 _pos, glm::vec3 _lookAt):
	Light(_pos),
	orth(ortho(-2.f, 2.f, -2.f, 2.f, 0.f, 3.f)),
	lookAt(_lookAt)
{
}

Spotlight::~Spotlight()
{
}

glm::mat4 Spotlight::proj() {
	return orth;
	//return perspective(45.f, 1920.f / 1080.f, 0.5f, 6.f);
}
glm::mat4 Spotlight::view() {
	if (normalize(lookAt - position) == -vec3(0, 1, 0)) {
		return glm::lookAt(position, lookAt, glm::vec3(0, 0, 1));
	}
	else {
		return glm::lookAt(position, lookAt, glm::vec3(0, 1, 0));
	}
}