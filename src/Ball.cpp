#include "Ball.h"
#include "PingPong.h"

using namespace basicgraphics;
using namespace glm;
using namespace std;

Ball::Ball() :
	_sphere(importer::loadModel("sphere.obj")),
	pos(0, TABLE_TOP + 0.5, 0),
	dir(0, 0, 0)
{
}


Ball::~Ball()
{
}

void Ball::draw(basicgraphics::GLSLProgram & shader)
{
	mat4 model;
	model = mat4(1.f);
	model = translate(model, pos);
	model = scale(model, vec3(BALL_RADIUS));
	standard_shader::setModel(shader, model);
	standard_shader::setMaterial(shader, {
		vec3(0.9f),
		vec3(0.9f),
		vec3(0.01f),
		.25f * 128
	});
	standard_shader::drawIndexedMeshes(_sphere->indexedGPUReferences);
}

bool Ball::shouldBounce() {
	return (pos.y < TABLE_TOP + BALL_RADIUS)
		&& (pos.x < TABLE_RIGHT && pos.x > TABLE_LEFT)
		&& (pos.z < TABLE_FRONT && pos.z > TABLE_BACK);
}
void Ball::update(float seconds)
{
	dir = dir + vec3(0, -GRAVITY*seconds,0);
	prev_pos = pos;
	pos = pos + (seconds*dir) +  dir * (0.5f * seconds * seconds);
	if (shouldBounce()) {
		pos.y = TABLE_TOP + BALL_RADIUS;
		dir *= vec3(0.95f,-0.9f,0.98f);
		//dir.y = dir.y * (-0.9f);
	}
}

void Ball::launch()
{
	pos = vec3(0, TABLE_TOP + BALL_RADIUS + 0.5, -TABLE_LENGTH / 2);
	dir = vec3(0, 0.4, TABLE_LENGTH/2);
}
