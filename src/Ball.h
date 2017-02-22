#pragma once

#include "Graphics.h"
#include "GLSLProgram.h"
#include "Importer.h"
#include "StandardShader.h"
#include "PingPong.h"


#define GRAVITY (9.8f/3)
#define BALL_RADIUS (30.0f / 1000) // 30 millimeters.

class Ball
{
public:
	Ball();
	~Ball();
	virtual void draw(basicgraphics::GLSLProgram &shader);
	void update(float seconds);
	void launch();
	glm::vec3 pos;
	glm::vec3 prev_pos;
	glm::vec3 dir;


protected:
	bool shouldBounceTable();
	bool shouldBounceFloor();

	std::shared_ptr<IndexedMeshes> _sphere;
};

