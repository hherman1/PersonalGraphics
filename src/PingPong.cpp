#include "PingPong.h"

#define THRESHOLD 0.05f

using namespace glm;
using namespace basicgraphics;
using namespace std;


// does not include texture, set texture before call.
void ping_pong::drawFloor(GLSLProgram & shader) {
	//textured floor
	{
		//square has default size 2 (each side is length 2)
		auto cube = utils::getCube();
		mat4 model(1.f);
		model = rotate(model, radians(90.f), vec3(1, 0, 0));
		model = scale(model, vec3(FLOOR_TILE_SIZE / 2,FLOOR_TILE_SIZE/2, FLOOR_DEPTH / 2));
		standard_shader::setMaterial(shader, {
			vec3(1.f),
			vec3(1.f),//vec3(1.f,0.8f,0),
			vec3(0.7f),
			32
		});
		// render floor before centering so that the back left corner is 0,0
		mat4 center = translate(mat4(1.f),vec3(FLOOR_TILE_SIZE/2,0,FLOOR_TILE_SIZE/2));
		center = translate(center, vec3(-FLOOR_WIDTH / 2, 0, -N_FLOOR_TILES_Z*FLOOR_TILE_SIZE / 2)); // center whole floor
		
		for (int x = 0; x < N_FLOOR_TILES_X; x++) {
			for (int z = 0; z < N_FLOOR_TILES_Z; z++) {
				mat4 translation = translate(center, vec3(x*FLOOR_TILE_SIZE, 0, z*FLOOR_TILE_SIZE)); // tile correctly
				standard_shader::setModel(shader, translation * model);
				standard_shader::drawArrayMesh(*cube);
			}
		}

	}
}
void ping_pong::drawRoomWalls(basicgraphics::GLSLProgram & shader) {
	auto cube = utils::getCube();
	standard_shader::setMaterial(shader, {
		vec3(1.f),
		vec3(1.f),//vec3(1.f,0.8f,0),
		vec3(0.7f),
		32
	});

	//back wall
	mat4 model(1.f);
	model = translate(model, vec3(0, ROOM_WALL_HEIGHT / 2, FLOOR_BACK));
	model = scale(model, vec3(FLOOR_WIDTH / 2, ROOM_WALL_HEIGHT / 2,0.1));
	standard_shader::setModel(shader, model);
	standard_shader::drawArrayMesh(*cube);

	//left wall
	model = mat4(1.f);
	model = translate(model, vec3(FLOOR_LEFT, ROOM_WALL_HEIGHT / 2, 0));
	model = rotate(model, radians(90.f), vec3(0, 1, 0));
	model = scale(model, vec3(FLOOR_LENGTH / 2, ROOM_WALL_HEIGHT / 2, 0.1));
	standard_shader::setModel(shader, model);
	standard_shader::drawArrayMesh(*cube);

	//right wall
	model = mat4(1.f);
	model = translate(model, vec3(FLOOR_RIGHT, ROOM_WALL_HEIGHT / 2, 0));
	model = rotate(model, radians(90.f), vec3(0, 1, 0));
	model = scale(model, vec3(FLOOR_LENGTH / 2, ROOM_WALL_HEIGHT / 2, 0.1));
	standard_shader::setModel(shader, model);
	standard_shader::drawArrayMesh(*cube);
}




//mediocre hit detection
float lineDistance(vec3 & start, vec3 & end, vec3 &pt);

bool ping_pong::ballHitPaddle(vec3 & ballCurrent,vec3 & ballPrev, vec3 & paddlePrev,vec3 & paddleCurrent) {
	//return lineDistance(paddlePrev, paddleCurrent, vec3(ballPos.x,0,ballPos.z)) < BALL_RADIUS + PADDLE_RADIUS;
	bool result = false;
	vec3 ballCurFixed(ballCurrent.x, 0, ballCurrent.z);
	vec3 ballPreFixed(ballPrev.x, 0, ballPrev.z);
	if(utils::bounded(ballCurrent.z,paddleCurrent.z,paddlePrev.z)) {
		result = result || lineDistance(paddlePrev, paddleCurrent, ballCurFixed) < BALL_RADIUS + PADDLE_RADIUS;
	}
	if (utils::bounded(ballPrev.z, paddleCurrent.z, paddlePrev.z)) {
		result = result || lineDistance(paddlePrev, paddleCurrent, ballPreFixed) < BALL_RADIUS + PADDLE_RADIUS;
	}

	if ((ballPrev.z - paddleCurrent.z) * (ballCurrent.z - paddleCurrent.z) < 0) {
		vec3 changeDir = normalize(ballCurFixed - ballPreFixed);
		vec3 compPoint = changeDir* (paddleCurrent.z - ballPreFixed.z) + ballPreFixed;
		result = result || (length(compPoint - paddleCurrent)) < BALL_RADIUS + PADDLE_RADIUS;
	}
	return result;
	//if (length(paddlePrev - paddleCurrent) < THRESHOLD ) {
	//	return lineDistance(ballPreFixed, ballCurFixed, paddleCurrent) < BALL_RADIUS + PADDLE_RADIUS;
	//}

}
bool ping_pong::ballHitWall(vec3 & ballCurrent, vec3 & ballPrev)
{
	return (ping_pong::ballHitWall(ballCurrent, ballPrev, vec3(TABLE_LEFT, 0, TABLE_BACK), vec3(TABLE_RIGHT, TABLE_WALL_TOP, TABLE_BACK),vec3(0,0,1)));

	return (ballCurrent.z < TABLE_BACK && ballPrev.z > TABLE_BACK)
		&& (utils::bounded(vec3(ballCurrent.x, ballCurrent.y, 0),
			vec3(TABLE_LEFT, 0, 0),
			vec3(TABLE_RIGHT, TABLE_WALL_TOP, 0)));
}
bool ping_pong::ballHitWall(vec3 & ballCurrent, vec3 & ballPrev,vec3 wallMin, vec3 wallMax, vec3 wallNorm)
{
	if (dot(ballCurrent - wallMax, wallNorm) * dot(ballPrev - wallMax, wallNorm) < 0) {
		vec3 proj = (ballCurrent - wallNorm*ballCurrent) + (wallNorm * wallMax);//closestPoint(wallMin, wallMax, ballCurrent);
		return (utils::bounded(proj,wallMin,wallMax));
	}
	return false;
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
