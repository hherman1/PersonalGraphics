#include "PingPong.h"

using namespace glm;

//mediocre hit detection
bool ballHitPaddle(vec2 ballPos, vec2 paddlePrev,vec2 paddleCurrent) {
	//return lineDistance(paddlePrev, paddleCurrent, ballPos) < BALL_RADIUS + PADDLE_RADIUS;
	return true;
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
		return start + direction * t / glm::length2(direction);

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