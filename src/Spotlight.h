#pragma once
#include "Graphics.h"
#include "Light.h"
class Spotlight : public Light
{
public:
	Spotlight(glm::vec3 _pos, glm::vec3 _lookAt);
	~Spotlight();
	glm::mat4 proj();
	glm::mat4 view();
	glm::vec3 lookAt;
	glm::mat4 orth;
};

