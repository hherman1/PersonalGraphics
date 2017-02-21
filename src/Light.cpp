#include "Light.h"



Light::Light(glm::vec3 _pos) :
	position(_pos)
{
}

Light::Light(glm::vec3 _pos,glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular):
	position(_pos),
	ambient(_ambient),
	diffuse(_diffuse),
	specular(_specular)
{
}


Light::~Light()
{
}
