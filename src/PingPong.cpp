#include "PingPong.h"

#define THRESHOLD 0.05f

using namespace glm;
using namespace basicgraphics;
using namespace std;

Material table_mat = {
	vec3(0.0,0.1,0.06),
	vec3(0.0,0.60980392,0.80980392),
	vec3(0.50196078,0.50196078,0.50196078),
	0.25f * 128.f
};
void ping_pong::drawWall(GLSLProgram & shader) {
	shared_ptr<ArrayMesh> wall = utils::getCube();
	mat4 model;
	model = translate(model, vec3(0, WALL_HEIGHT/2 + TABLE_TOP, TABLE_BACK));
	model = scale(model, vec3(TABLE_WIDTH / 2, WALL_HEIGHT / 2,0.001f));
	standard_shader::setModel(shader, model);
	standard_shader::setMaterial(shader, table_mat);
	standard_shader::drawArrayMesh(*wall);
}

void ping_pong::drawTable(GLSLProgram & shader) {
		shared_ptr<ArrayMesh> cube = utils::getCube();
		mat4 model;
		model = translate(model, vec3(0, 0, 0));
		model = scale(model, vec3(TABLE_WIDTH / 2, TABLE_DEPTH / 2, TABLE_LENGTH / 2));
		standard_shader::setModel(shader, model);
		standard_shader::setMaterial(shader, table_mat);
		standard_shader::drawArrayMesh(*cube);
}

void ping_pong::drawNet(GLSLProgram & shader)
{
	mat4 net = mat4(1.f);
	net = translate(net, vec3(0.0f, TABLE_TOP + NET_DEPTH / 2, 0.0f));
	vec3 diffuse;
	shared_ptr<ArrayMesh> cube;

	for (int i = 0; i < N_VSTRINGS; i++) {
		mat4 net_string = mat4(1.f);

		float dx = (TABLE_WIDTH + 2*NET_OVERHANG) / (N_VSTRINGS-1);
		
		net_string = translate(net_string, vec3(TABLE_LEFT-NET_OVERHANG + i*dx, 0, 0.0f));
		net_string = scale(net_string, vec3(0.00125f, NET_DEPTH/2, 0.00125f));
		standard_shader::setModel(shader, net * net_string);
		if (i == 0 || i == N_VSTRINGS - 1) {
			diffuse = vec3(0.0f, 0.0f, 0.0f);
		}
		else {
			diffuse = vec3(1.0f, 1.0f, 1.0f);
		}

		standard_shader::setMaterial(shader, {
			diffuse,
			diffuse,
			vec3(0.01),
			0.5f * 128
		});
		cube = utils::getCube();
		standard_shader::drawArrayMesh(*cube);
	}
	for (int i = 0; i < N_HSTRINGS; i++) {
		float dy = (float)NET_DEPTH / (float)(N_HSTRINGS-1);
		float y = dy * i - NET_DEPTH / 2;
		mat4 model_make = mat4(1.f);
		model_make = translate(model_make, vec3(0.0f, y, 0.0f));
		model_make = scale(model_make, vec3((TABLE_WIDTH / 2)+NET_OVERHANG, 0.00125f, 0.00125f));
		standard_shader::setModel(shader, net * model_make);

		if (i == 0 || i == N_HSTRINGS - 1) {
			diffuse = vec3(0.0f, 0.0f, 0.0f);
		}
		else {
			diffuse = vec3(1.0f, 1.0f, 1.0f);
		}
		standard_shader::setMaterial(shader, {
			diffuse,
			diffuse,
			vec3(0.01),
			0.5f * 128
		});
		cube = utils::getCube();
		standard_shader::drawArrayMesh(*cube);
	}
}


//mediocre hit detection
float lineDistance(vec3 & start, vec3 & end, vec3 &pt);

bool ping_pong::ballHitPaddle(vec3 & ballCurrent,vec3 & ballPrev, vec3 & paddlePrev,vec3 & paddleCurrent) {
	//return lineDistance(paddlePrev, paddleCurrent, vec3(ballPos.x,0,ballPos.z)) < BALL_RADIUS + PADDLE_RADIUS;
	vec3 ballCurFixed(ballCurrent.x, 0, ballCurrent.z);
	vec3 ballPreFixed(ballPrev.x, 0, ballPrev.z);
	if(utils::bounded(ballCurrent.z,paddleCurrent.z,paddlePrev.z) 
		|| utils::bounded(ballPrev.z, paddleCurrent.z, paddlePrev.z)) {
		return lineDistance(paddlePrev, paddleCurrent, ballCurFixed) < BALL_RADIUS + PADDLE_RADIUS;
	}

	if ((ballPrev.z - paddleCurrent.z) * (ballCurrent.z - paddleCurrent.z) < 0) {
		vec3 changeDir = normalize(ballCurFixed - ballPreFixed);
		vec3 compPoint = changeDir* (paddleCurrent.z - ballPreFixed.z) + ballPreFixed;
		return (length(compPoint - paddleCurrent)) < BALL_RADIUS + PADDLE_RADIUS;
	}
	return false;
	//if (length(paddlePrev - paddleCurrent) < THRESHOLD ) {
	//	return lineDistance(ballPreFixed, ballCurFixed, paddleCurrent) < BALL_RADIUS + PADDLE_RADIUS;
	//}

}
bool ping_pong::ballHitWall(vec3 & ballCurrent, vec3 & ballPrev)
{
	return (ballCurrent.z < TABLE_BACK && ballPrev.z > TABLE_BACK)
		&& (utils::bounded(vec3(ballCurrent.x, ballCurrent.y, 0),
			vec3(TABLE_LEFT, 0, 0),
			vec3(TABLE_RIGHT, WALL_TOP, 0)));
}
bool ping_pong::ballHitNet(vec3 & ballCurrent, vec3 & ballPrev)
{
	return (ballCurrent.z * ballPrev.z <= 0)
		&& (utils::bounded(vec3(ballCurrent.x, ballCurrent.y, 0),
			vec3(TABLE_LEFT - NET_OVERHANG, 0, 0),
			vec3(TABLE_RIGHT + NET_OVERHANG, NET_TOP, 0)));
}
float lineDistance(vec3 & start, vec3 & end, vec3 &pt) {
	return length(pt - closestPoint(start, end, pt));
}
glm::vec3 closestPoint(glm::vec3 & start, glm::vec3 & end, glm::vec3 &pt) {
	// The vector from the end of the capsule to the point in question.
	vec3 v(pt - start);

	// Projection of v onto the line segment scaled by 
	// the length of direction.
	vec3 direction(end - start);
	float t = dot(direction, v);

	// Avoid some square roots.  Derivation:
	//    t/direction.length() <= direction.length()
	//      t <= direction.squaredLength()

	if ((t >= 0) && (t <= glm::length2(direction))) {

		// The point falls within the segment.  Normalize direction,
		// divide t by the length of direction.
		if (length2(direction) > 0) {
			return start + direction * t / glm::length2(direction);
		}
		else {
			return start;
		}

	}
	else {

		// The point does not fall within the segment; see which end is closer.

		// Distance from 0, squared
		float d0Squared = length2(v);

		// Distance from 1, squared
		float d1Squared = length2(v - direction);

		if (d0Squared < d1Squared) {

			// Point 0 is closer
			return start;

		}
		else {

			// Point 1 is closer
			return end;

		}
	}
}
