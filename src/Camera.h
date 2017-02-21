#pragma once
#include "Graphics.h"
class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 lookAt, float aspectRatio);
	~Camera();
	glm::mat4 proj();
	glm::vec3 pos;
	glm::vec3 lookAt;
	glm::mat4 persp;
	glm::mat4 view();
};

