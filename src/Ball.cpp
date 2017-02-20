#include "Ball.h"
#include "Table.h"

using namespace basicgraphics;
using namespace glm;

Ball::Ball() :
	_sphere(vec3(0, 0, 0), BALL_RADIUS, vec4(1, 1, 1, 1)),
	pos(0, TABLE_TOP + 0.5, 0),
	dir(0, 0, 0)
{
}


Ball::~Ball()
{
}

void Ball::draw(basicgraphics::GLSLProgram & shader, const glm::mat4 & modelMatrix)
{
	glm::mat4 model = modelMatrix * translate(mat4(1.0f), pos);
	_sphere.draw(shader, model);
}

void Ball::update(std::chrono::milliseconds ms)
{
	float seconds = ms.count() / 1000.0f;
	dir = dir + vec3(0, -9.8*seconds,0);
	pos = pos + (seconds*dir) +  dir * (0.5f * seconds * seconds);
	if (pos.y < TABLE_TOP + BALL_RADIUS) {
		pos.y = TABLE_TOP + BALL_RADIUS;
		dir.y = dir.y * (-0.9f);
	}
}

void Ball::launch()
{
	pos = vec3(0, TABLE_TOP + BALL_RADIUS + 0.5, -TABLE_LENGTH / 2);
	dir = vec3(0, 0, TABLE_LENGTH/4);
}
