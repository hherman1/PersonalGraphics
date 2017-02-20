#pragma once

#include <chrono>
#include "Sphere.h"

#define BALL_RADIUS (20.0f / 1000) // 20 millimeters.

class Ball
{
public:
	Ball();
	~Ball();
	virtual void draw(basicgraphics::GLSLProgram &shader, const glm::mat4 &modelMatrix);
	void update(std::chrono::milliseconds ms);
	void launch();

protected:
	glm::vec3 pos;
	glm::vec3 dir;
	basicgraphics::Sphere _sphere;
};

