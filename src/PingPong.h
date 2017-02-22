#pragma once
#include "Graphics.h"
#include <glm/glm/gtx/quaternion.hpp>
#include "Ball.h"
#include "Paddle.h"
#include "GLSLProgram.h"
#include "Mesh.h"
#include <memory>
#include "Utils.h"
#include "StandardShader.h"


#define TABLE_LENGTH 2.74f // meters
#define TABLE_WIDTH 1.525f // meters
#define TABLE_HEIGHT 0.77f // meters
#define TABLE_DEPTH 0.04f // meters
#define TABLE_TOP (TABLE_HEIGHT + TABLE_DEPTH/2)
#define TABLE_FRONT (TABLE_LENGTH/2)
#define TABLE_BACK (-TABLE_LENGTH/2)	
#define TABLE_LEFT (-TABLE_WIDTH/2)
#define TABLE_RIGHT (TABLE_WIDTH/2)

#define NET_DEPTH 0.15f
#define NET_TOP (NET_DEPTH + TABLE_TOP)
#define NET_OVERHANG 0.15f
#define N_VSTRINGS 40
#define N_HSTRINGS 10

#define WALL_HEIGHT (TABLE_LENGTH/3)
#define WALL_TOP (TABLE_TOP + WALL_HEIGHT)

#define FLOOR_TILE_SIZE 2.f
#define N_FLOOR_TILES_X 3
#define N_FLOOR_TILES_Z 3
#define FLOOR_DEPTH 0.01f
#define FLOOR_WIDTH (N_FLOOR_TILES_X * FLOOR_TILE_SIZE)
#define FLOOR_LENGTH (N_FLOOR_TILES_Z * FLOOR_TILE_SIZE)
#define FLOOR_BACK (-FLOOR_LENGTH/2)
#define FLOOR_FRONT (FLOOR_LENGTH/2)
#define FLOOR_LEFT (-FLOOR_WIDTH/2)
#define FLOOR_RIGHT (FLOOR_WIDTH/2)
#define FLOOR_TOP (FLOOR_DEPTH/2)

#define ROOM_WALL_HEIGHT 4.f


//_CANNOT_ use vec2(ball.pos) to construct, we are concerned with 
//the balls x and _z_ coords, not x and y.
namespace ping_pong {
	void drawWall(basicgraphics::GLSLProgram & shader);
	void drawTable(basicgraphics::GLSLProgram & shader);
	void drawFloor(basicgraphics::GLSLProgram & shader);
	void drawRoomWalls(basicgraphics::GLSLProgram & shader);
	void drawNet(basicgraphics::GLSLProgram & shader);
	bool ballHitPaddle(vec3 & ballCurrent, vec3 & ballPrev, vec3 & paddlePrev, vec3 & paddleCurrent);
	bool ballHitWall(vec3 & ballCurrent, vec3 & ballPrev);
	bool ballHitWall(vec3 & ballCurrent, vec3 & ballPrev, vec3 wallMin, vec3 wallMax, vec3 wallNorm);
	bool ballHitNet(vec3 & ballCurrent, vec3 & ballPrev);
}


glm::vec3 closestPoint(glm::vec3 & start, glm::vec3 & end, glm::vec3 & pt);
