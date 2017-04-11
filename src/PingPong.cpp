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

	if ((ballPrev.z - paddleCurrent.z+BALL_RADIUS) * (ballCurrent.z - paddleCurrent.z+BALL_RADIUS) < 0) {
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

namespace pingponggame {
	int actions[1024];
	bool keys[1024];

	int w_width, w_height;

	shared_ptr<Texture2D> cobble;
	shared_ptr<Texture2D> cobble_normal;
	shared_ptr<Texture2D> cobble_displacement;

	shared_ptr<Texture2D> wood_texture;
	shared_ptr<Texture2D> white_texture;

	shared_ptr<Paddle> _paddle;
	shared_ptr<Ball> _ball;
	shared_ptr<Table> _table;

	bool inbounds = true;
	int volley = 0;
	bool tPressed = false;

	Spotlight _light(vec3(0),vec3(0));
	Camera _camera(vec3(0),vec3(0),0);

	Spotlight light() {
		return _light;
	}
	Camera camera() {
		return _camera;
	}

	struct {
		double yaw = 0;
		double pitch = 0;
		vec3 front = vec3(1, 0, 0);
	} aim;
	vec2 mouse;
	vec2 mouseScreen;
	vec2 mouseScreenDiff;
	void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		GLfloat xoffset = xpos - mouse.x;
		GLfloat yoffset = mouse.y - ypos; // Reversed since y-coordinates range from bottom to top
		mouse.x = xpos;
		mouse.y = ypos;

		mouseScreen.x = xpos / w_width;
		mouseScreen.y = ypos / w_height;

		mouseScreenDiff.x = xoffset / w_width;
		mouseScreenDiff.y = yoffset / w_height;

		GLfloat sensitivity = 0.3f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		aim.yaw += xoffset;
		aim.pitch -= yoffset;
		if (aim.pitch > 89.0f)
			aim.pitch = 89.0f;
		if (aim.pitch < -89.0f)
			aim.pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(aim.pitch)) * cos(glm::radians(aim.yaw));
		front.y = sin(glm::radians(aim.pitch));
		front.z = cos(glm::radians(aim.pitch)) * sin(glm::radians(aim.yaw));
		aim.front = glm::normalize(front);
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		// When a user presses the escape key, we set the WindowShouldClose property to true, 
		// closing the application
		actions[key] = action;
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}
	void init(GLFWwindow* window, int width, int height) {
		w_width = width;
		w_height = height;

		glfwSetKeyCallback(window, key_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);
		wood_texture.reset(new Texture2D());
		//wood_texture = Texture2D();
		wood_texture->bind();
		wood_texture->loadImage("wood2.jpg");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		wood_texture->unbind();

		cobble.reset(new Texture2D());
		cobble->bind();
		cobble->loadImage("cobble_albedo.png");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		cobble->unbind();

		cobble_normal.reset(new Texture2D());
		cobble_normal->bind();
		cobble_normal->loadImage("cobble_normal.png");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		cobble_normal->unbind();

		cobble_displacement.reset(new Texture2D());
		cobble_displacement->bind();
		cobble_displacement->loadImage("cobble_displacement.png");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		cobble_displacement->unbind();

		white_texture.reset(new Texture2D());
		white_texture->bind();
		white_texture->whiteTexture();

		_light = Spotlight(vec3(0.5, 1.5 + TABLE_TOP, 0.5), vec3(0, 0, 0));
		_light.ambient = vec3(0.2f);
		_light.diffuse = vec3(1.f); // Let's darken the _light a bit to fit the scene
		_light.specular = vec3(1.0f, 1.0f, 1.0f);
		_camera = Camera(vec3(0, 1 + TABLE_TOP, 2.25), vec3(0, TABLE_TOP, 0.25), (float)w_width / (float)w_height);
	
		_paddle.reset(new Paddle());
		_table.reset(new Table());
		_ball.reset(new Ball());
	}
	void update(float seconds) {
		Paddle & paddle = *_paddle;
		Table & table = *_table;
		Ball & ball = *_ball;
		//Debug mode only.
		//_camera.pos = 3.f * aim.front;
		//_camera.lookAt = _camera.pos + aim.front * vec3(1,-1,1);
		//_camera.pos = vec3(0);
		//_camera.lookAt = aim.front * vec3(1, -1, 1);
		//_camera.pos.y += TABLE_TOP + 1;
		vec3 cameraTarget = 0.1f*(paddle.pos + ball.pos) + 0.9f*vec3(0, TABLE_TOP, 0);
		_camera.lookAt += (cameraTarget - _camera.lookAt)*seconds; //0.1f*(_camera.lookAt - ());

		vec3 oldPos = paddle.pos;
		paddle.move(vec2(mouseScreenDiff.x, -mouseScreenDiff.y));
		mouseScreenDiff = vec2(0);

		ball.update(seconds);
		if (ping_pong::ballHitPaddle(ball.pos, ball.prev_pos, oldPos, paddle.pos)) {
			//ball.dir = vec3(0);
			float ballDir = sign(ball.pos.z - ball.prev_pos.z);
			float paddleDir = sign(paddle.pos.z - paddle.prev_pos.z);
			vec3 paddleChange = paddle.pos - oldPos;

			if (ballDir == 1) {
				ball.dir.z *= -1;

			}
			ball.dir += (paddleChange / seconds)*vec3(0.6, 0, 0.3f);
			ball.dir.y += 0.05*(length(paddleChange) / seconds + length(ball.dir));

			ball.pos.z = paddle.pos.z - 0.15;

		}
		//net hit?
		if (ping_pong::ballHitNet(ball.pos, ball.prev_pos)) {
			ball.dir.z *= -1;
			ball.pos = ball.prev_pos;
			//ball.pos.z = sign(ball.pos.z) * 0.03 + sign(ball.pos.z) * BALL_RADIUS;
			ball.dir *= 0.1;
			if (inbounds) {
				cout << endl << "You hit the net after " << volley << " volleys!" << endl;
				volley = 0;
				inbounds = false;
			}
		}
		//wall hit?
		if (ping_pong::ballHitWall(ball.pos, ball.prev_pos)) {
			ball.dir.z *= -0.9;
			ball.pos.z = TABLE_BACK + BALL_RADIUS;
			volley++;
			cout << "Volley " << volley << ". ";
		}
#define ROOM_WALL_DAMPENER 0.5f
		//back room wall hit?
		if (ping_pong::ballHitWall(ball.pos, ball.prev_pos,
			vec3(FLOOR_LEFT, 0, FLOOR_BACK),
			vec3(FLOOR_RIGHT, ROOM_WALL_HEIGHT, FLOOR_BACK),
			vec3(0, 0, 1))) {
			ball.dir.z *= -ROOM_WALL_DAMPENER;
			ball.pos.z = FLOOR_BACK + BALL_RADIUS;
		}
		//"front room wall" hit?
		if (ping_pong::ballHitWall(ball.pos, ball.prev_pos,
			vec3(FLOOR_LEFT, 0, FLOOR_FRONT),
			vec3(FLOOR_RIGHT, ROOM_WALL_HEIGHT, FLOOR_FRONT),
			vec3(0, 0, 1))) {
			ball.dir.z *= -ROOM_WALL_DAMPENER;
			ball.pos.z = FLOOR_FRONT - BALL_RADIUS;
		}
		// left room wall?
		//bool test = utils::bounded(vec3(-3, 0.5, 1), vec3(-3, 0, 3), vec3(-3, 4, -3));
		if (ping_pong::ballHitWall(ball.pos, ball.prev_pos,
			vec3(FLOOR_LEFT, 0, FLOOR_BACK),
			vec3(FLOOR_LEFT, ROOM_WALL_HEIGHT, FLOOR_FRONT),
			vec3(1, 0, 0))) {
			ball.dir.x *= -ROOM_WALL_DAMPENER;
			ball.pos.x = FLOOR_LEFT + BALL_RADIUS;
		}
		//right room wall?
		if (ping_pong::ballHitWall(ball.pos, ball.prev_pos,
			vec3(FLOOR_RIGHT, 0, FLOOR_BACK),
			vec3(FLOOR_RIGHT, ROOM_WALL_HEIGHT, FLOOR_FRONT),
			vec3(1, 0, 0))) {
			ball.dir.x *= -ROOM_WALL_DAMPENER;
			ball.pos.x = FLOOR_RIGHT - BALL_RADIUS;
		}
		//other messages
		{
			if (ball.pos.y < TABLE_TOP && inbounds) {
				cout << endl << "The ball is out of bounds! You had " << volley << " volleys!" << endl;
				volley = 0;
				inbounds = false;
			}

		}

		if (keys[GLFW_KEY_A]) {
			_light.position.x -= 1 * seconds;
		}
		if (keys[GLFW_KEY_D]) {
			_light.position.x += 1 * seconds;
		}
		if (keys[GLFW_KEY_W]) {
			_light.position.z -= 1 * seconds;
		}
		if (keys[GLFW_KEY_S]) {
			_light.position.z += 1 * seconds;
		}
		if (keys[GLFW_KEY_Q]) {
			_light.position.y -= 1 * seconds;
		}
		if (keys[GLFW_KEY_E]) {
			_light.position.y += 1 * seconds;
		}

		if (keys[GLFW_KEY_SPACE]) {
			ball.launch();
			inbounds = true;
		}
		if (keys[GLFW_KEY_T] && !tPressed) {
			table.trump();
			tPressed = true;
		}
		if (!keys[GLFW_KEY_T]) {
			tPressed = false;
		}
	}

	void drawGeometry(basicgraphics::GLSLProgram & shader)
	{
		standard_shader::setTexture2D(shader, *white_texture);
		standard_shader::setTexture2DNormal(shader, *white_texture);
		standard_shader::setTexture2DDisplacement(shader, *white_texture);

		_paddle->draw(shader);
		_ball->draw(shader);
		_table->draw(shader);

		standard_shader::setTexture2D(shader, *cobble);
		standard_shader::setTexture2DNormal(shader, *cobble_normal);
		standard_shader::setTexture2DDisplacement(shader, *cobble_displacement);
		ping_pong::drawFloor(shader);
	}

}



