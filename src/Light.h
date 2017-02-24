#pragma once
#include "Graphics.h"
class Light
{
public:
	Light(glm::vec3 _pos);
	Light(glm::vec3 _pos, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular);
	~Light();
	glm::vec3 position;
	glm::vec3 direction = glm::vec3(0, -1.f, 0);

	glm::vec3 ambient = glm::vec3(1.f);
	glm::vec3 diffuse = glm::vec3(1.f);
	glm::vec3 specular = glm::vec3(1.f);

	float constant = 1.f;
	float linear = 0.09;//0.001f;
	float quadratic = 0.032;//0.00032;

	float farPlane = 5;

	float cutOff = -1; // Point _light
};

