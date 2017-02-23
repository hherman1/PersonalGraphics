#pragma once
#include "GLSLProgram.h"
#include "Graphics.h"
#include "StandardShader.h"
#include "Utils.h"
#include <memory>
#include "Mesh.h"
#include "Texture2D.h"

#define TABLE_LENGTH 2.74f // meters
#define TABLE_WIDTH 1.525f // meters
#define TABLE_HEIGHT 0.77f // meters
#define TABLE_DEPTH 0.04f // meters
#define TABLE_TOP (TABLE_HEIGHT + TABLE_DEPTH/2)
#define TABLE_FRONT (TABLE_LENGTH/2)
#define TABLE_BACK (-TABLE_LENGTH/2)	
#define TABLE_LEFT (-TABLE_WIDTH/2)
#define TABLE_RIGHT (TABLE_WIDTH/2)

#define TABLE_WALL_HEIGHT (TABLE_LENGTH/3)
#define TABLE_WALL_TOP (TABLE_TOP + TABLE_WALL_HEIGHT)

#define NET_DEPTH 0.15f
#define NET_TOP (NET_DEPTH + TABLE_TOP)
#define NET_OVERHANG 0.15f
#define N_VSTRINGS 40
#define N_HSTRINGS 10

#define TABLE_LEG_SIZE 0.03

class Table
{
public:
	Table();
	~Table();
	virtual void draw(basicgraphics::GLSLProgram &shader);
	void trump();
protected:
	bool _trump = false;
	void drawWall(basicgraphics::GLSLProgram & shader);
	void drawLegs(basicgraphics::GLSLProgram & shader);

	void drawBoard(basicgraphics::GLSLProgram & shader);

	void drawNet(basicgraphics::GLSLProgram & shader);
	Texture2D trump_texture;
	Texture2D white_texture;

	Material table_mat = {
		vec3(0.0,0.1,0.06),
		vec3(0.0,0.60980392,0.80980392),
		vec3(0.50196078,0.50196078,0.50196078),
		0.25f * 128.f
	};
};

