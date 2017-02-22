#pragma once
#include "Graphics.h"
#include <glm/glm/gtx/quaternion.hpp>
#include "Ball.h"
#include "Paddle.h"

//_CANNOT_ use vec2(ball.pos) to construct, we are concerned with 
//the balls x and _z_ coords, not x and y.
bool ballHitPaddle(vec3 & ballPos, vec3 & paddlePrev, vec3 & paddleCurrent);


glm::vec3 closestPoint(glm::vec3 & start, glm::vec3 & end, glm::vec3 & pt);
