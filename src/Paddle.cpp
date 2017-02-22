#include "Paddle.h"

using namespace std;
using namespace basicgraphics;
using namespace glm;

Paddle::Paddle() :
	pos(-0.5, 0,0.25)
{
	cylinder = importer::loadModel("cylinder.obj");
}
void Paddle::move(vec2 diff) {
	pos += vec3(diff.x,0,diff.y);
	pos = glm::clamp(pos, vec3(TABLE_LEFT,0, 0), vec3(TABLE_RIGHT,0, TABLE_FRONT));
}
float Paddle::getAngle() {
	//have paddle rotate
	vec3 lookAt(0, 0, TABLE_BACK / 2);
	vec3 face = normalize(lookAt - pos);
	float angle = acos(dot(face, vec3(0, 1, 0)));
	if (cross(face, vec3(0, 1, 0)).z < 0) {
		angle = -angle;
	}
	return angle;
}
void Paddle::draw(GLSLProgram& shader) {
	//Ping Pong Paddle
	{

		float angle = getAngle();
		mat4 model = mat4(1.f);
		model = translate(model, vec3(pos.x,PADDLE_ELEVATION,pos.z));
		model = rotate(model, angle, vec3(0, 1, 0));


		//paddle head
		mat4 model_paddle = mat4(1.f);
		model_paddle = translate(model_paddle, vec3(0, 0, 0));
		model_paddle = scale(model_paddle, vec3(PADDLE_RADIUS, PADDLE_RADIUS, 0.02));
		model_paddle = rotate(model_paddle, radians(90.f), vec3(1, 0, 0));
		standard_shader::setModel(shader, model * model_paddle);
		vec3 diffuse = vec3(1.f, 0.5, 0.5);
		standard_shader::setMaterial(shader, {
			diffuse,
			diffuse,
			vec3(0.01),
			0.5f * 128
		});
		standard_shader::drawIndexedMeshes(cylinder->indexedGPUReferences);

		//paddle handle

		vec4 handle_target(pos.x, 0, 0, 1);
		mat4 model_handle = mat4(1.f);
		model_handle = rotate(model_handle, radians(180.f) + angle, vec3(0, 0, 1));
		model_handle = translate(model_handle, vec3(0, -0.15, 0));
		model_handle = scale(model_handle, vec3(0.02, 0.15, 0.02));
		standard_shader::setModel(shader, model * model_handle);

		diffuse = vec3(1.f, 0.8f, 0);
		standard_shader::setMaterial(shader, {
			diffuse,
			diffuse,
			vec3(0.f),
			0.5f * 128
		});
		standard_shader::drawIndexedMeshes(cylinder->indexedGPUReferences);
	}
}

Paddle::~Paddle()
{
}
