#pragma once
#include "Graphics.h"
#include "GLSLProgram.h"
#include "StandardShader.h"
#include "Importer.h"
#include "Table.h"

#define PADDLE_ELEVATION 0.25f
#define PADDLE_RADIUS 0.1f
class Paddle
{
public:
	Paddle();
	void move(glm::vec2 diff);
	~Paddle();
	void draw(basicgraphics::GLSLProgram& shader);
	glm::vec3 pos;
	float getAngle();
protected:

	std::shared_ptr<IndexedMeshes> cylinder;
};

