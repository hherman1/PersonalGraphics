#include "Camera.h"


using namespace glm;

Camera::Camera(vec3 _pos, vec3 _lookAt, float aspectRatio):
	persp(perspective(45.0f, aspectRatio, 0.1f, 100.0f)),
	lookAt(_lookAt),
	pos(_pos)
{
}


Camera::~Camera()
{
}

mat4 Camera::proj() {
	return persp;
}

mat4 Camera::view() {
	return glm::lookAt(pos, lookAt, vec3(0, 1, 0));
}
