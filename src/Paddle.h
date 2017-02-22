#pragma once
#include "Graphics.h"
#include "GLSLProgram.h"
#include "StandardShader.h"
#include "Importer.h"
#include "PingPong.h"

#define PADDLE_ELEVATION (0.25f + TABLE_TOP)
#define PADDLE_RADIUS 0.1f
#define PADDLE_DEPTH 0.02f
class Paddle
{
public:
	Paddle();
	void move(glm::vec2 diff);
	~Paddle();
	void draw(basicgraphics::GLSLProgram& shader);
	glm::vec3 pos;
	glm::vec3 prev_pos;
	float getAngle();
protected:

	std::shared_ptr<IndexedMeshes> cylinder;
};

