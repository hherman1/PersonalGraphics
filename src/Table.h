#pragma once
#include "GLSLProgram.h"
#include "Graphics.h"
#include "StandardShader.h"
#include "Utils.h"
#include <memory>
#include "Mesh.h"




class Table
{
public:
	Table();
	~Table();
	virtual void draw(basicgraphics::GLSLProgram &shader);

};

