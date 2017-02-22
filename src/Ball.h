#pragma once

#include "Graphics.h"
#include "GLSLProgram.h"
#include "Importer.h"
#include "StandardShader.h"

#define BALL_RADIUS (30.0f / 1000) // 30 millimeters.

class Ball
{
public:
	Ball();
	~Ball();
	virtual void draw(basicgraphics::GLSLProgram &shader);
	void update(float seconds);
	void launch();

protected:
	bool shouldBounce();

	glm::vec3 pos;
	glm::vec3 dir;
	std::shared_ptr<IndexedMeshes> _sphere;
};

