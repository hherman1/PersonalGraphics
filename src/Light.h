#pragma once
#include "Graphics.h"
class Light
{
public:
	Light(glm::vec3 _pos);
	Light(glm::vec3 _pos, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular);
	~Light();
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

