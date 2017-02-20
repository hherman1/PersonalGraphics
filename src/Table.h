#pragma once
#include "Box.h"
#include "GLSLProgram.h"

#define TABLE_LENGTH 2.74 // meters
#define TABLE_WIDTH 1.525 // meters
#define TABLE_HEIGHT 0.76 // meters
#define TABLE_DEPTH 0.05 // meters
#define TABLE_TOP (TABLE_HEIGHT + TABLE_DEPTH)
#define TABLE_FRONT (TABLE_LENGTH/2)
#define TABLE_BACK (-TABLE_LENGTH/2)	

class Table
{
public:
	Table();
	~Table();
	basicgraphics::Box box;
	virtual void draw(basicgraphics::GLSLProgram &shader, const glm::mat4 &modelMatrix);

};

