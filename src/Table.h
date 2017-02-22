#pragma once
#include "GLSLProgram.h"
#include "Graphics.h"
#include "StandardShader.h"
#include "Utils.h"
#include <memory>
#include "Mesh.h"

#define TABLE_LENGTH 2.74 // meters
#define TABLE_WIDTH 1.525 // meters
#define TABLE_HEIGHT 0 // meters
#define TABLE_DEPTH 0.04 // meters
#define TABLE_TOP (TABLE_HEIGHT + TABLE_DEPTH/2)
#define TABLE_FRONT (TABLE_LENGTH/2)
#define TABLE_BACK (-TABLE_LENGTH/2)	
#define TABLE_LEFT (-TABLE_WIDTH/2)
#define TABLE_RIGHT (TABLE_WIDTH/2)


class Table
{
public:
	Table();
	~Table();
	virtual void draw(basicgraphics::GLSLProgram &shader);

};

