//
//  main.cpp
//  
//
//  Created by Bret Jackson on 1/29/17.
//
//


#include "Graphics.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

#include "Engine.h"

//Ping pong assignment
#include "Paddle.h"
#include "Ball.h"
#include "Table.h"
#include "PingPong.h"

using namespace glm;
using namespace std;
using namespace basicgraphics;


int actions[1024];
bool keys[1024];
int w_width = 1920;
int w_height = 1080;

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

struct {
	double yaw = 0;
	double pitch = 0;
	vec3 front = vec3(1,0,0);
} aim ;
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

	GLfloat sensitivity = 0.05f;
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



GLFWwindow* init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(w_width, w_height, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		throw "Failed to create GLFW window";
	}
	glfwMakeContextCurrent(window);
	//glewExperimental = GL_TRUE;
	//if (glewInit() != GLEW_OK)
	//{
	//	throw "Failed to initialize GLEW";
	//}
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);


	glEnable(GL_DEPTH_TEST);


	return window;
}

void wireframe() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char** argv)
{
	
	GLFWwindow* window = init();

	

	//Keep these as shared_ptrs so they arent accidentally deallocated.
	shaderregistry::buildAllShaders();
	shared_ptr<GLSLProgram> depthShader = shaderregistry::loadShader(shaderregistry::DEPTH);
	shared_ptr<GLSLProgram> depthCubemapShader = shaderregistry::loadShader(shaderregistry::DEPTH_CUBEMAP);
	shared_ptr<GLSLProgram> basicShader = shaderregistry::loadShader(shaderregistry::BASIC);
	shared_ptr<GLSLProgram> unlitShader = shaderregistry::loadShader(shaderregistry::UNLIT);

	
	/*Texture gorilla;
	gorilla.loadImage("gorilla.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gorilla.unbind();*/

	shared_ptr<IndexedMeshes> sphere = importer::loadModel("sphere.obj");
	shared_ptr<IndexedMeshes> cylinder = importer::loadModel("cylinder.obj");
	shared_ptr<IndexedMeshes> cone = importer::loadModel("cone.obj");
	//shared_ptr<IndexedMeshes> cube = importer::loadModel("cube.obj");

	
	DepthTexture depthTexture;
	DepthCubemap depthCubemap;

	Clock clock;

	float rotation = 0;

	Spotlight light(vec3(0.5, 1+TABLE_TOP, 0.5), vec3(0, 0, 0));
	light.ambient = vec3(0.2f);
	light.diffuse = vec3(1.f); // Let's darken the light a bit to fit the scene
	light.specular = vec3(1.0f, 1.0f, 1.0f);
	Camera camera(vec3(0, 1+TABLE_TOP, 2.25), vec3(0, TABLE_TOP, 0.25), (float)w_width / (float)w_height);

	Material mat = {
		vec3(1.0f, 0.5f, 0.31f),vec3(1.0f, 0.5f, 0.31f),vec3(0.5f, 0.5f, 0.5f),32.0f
	};

	Texture2D white_texture;
	white_texture.whiteTexture();
	Texture2D wood_texture;
	wood_texture.loadImage("wood2.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	wood_texture.unbind();

	Cubemap skybox;
	string files[6] = { "skybox/right.jpg","skybox/left.jpg","skybox/top.jpg","skybox/bottom.jpg","skybox/back.jpg","skybox/front.jpg" };
	skybox.loadCubemap(files);

	vec3 ball_pos;
	
	Table table;
	Paddle paddle;
	Ball ball;
	bool inbounds = true;
	int volley = 0;
	bool tPressed = false;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float seconds = clock.tick();
		rotation += 180*seconds ;


		//Debug mode only.
		//camera.pos = 3.f * aim.front;
		//camera.pos.y += TABLE_TOP + 1;


		//gorilla.bind();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if (keys[GLFW_KEY_A]) {
            light.position.x -= 1 * seconds;
        }
        if (keys[GLFW_KEY_D]) {
            light.position.x += 1 * seconds;
        }
        if (keys[GLFW_KEY_W]) {
            light.position.z -= 1 * seconds;
        }
        if (keys[GLFW_KEY_S]) {
            light.position.z += 1 * seconds;
        }


		
		vec3 oldPos = paddle.pos;
		paddle.move(vec2(mouseScreenDiff.x,-mouseScreenDiff.y));
		vec3 cameraTarget = 0.1f*(paddle.pos + ball.pos) + 0.9f*vec3(0, TABLE_TOP, 0);
		camera.lookAt += (cameraTarget - camera.lookAt)*seconds; //0.1f*(camera.lookAt - ());
		mouseScreenDiff = vec2(0);
		//paddle.pos = paddle.pos + vec3(0.1*seconds);
		ball.update(seconds);
		if (ping_pong::ballHitPaddle(ball.pos,ball.prev_pos, oldPos, paddle.pos)) {
			//ball.dir = vec3(0);
			float ballDir = sign(ball.pos.z - ball.prev_pos.z);
			float paddleDir = sign(paddle.pos.z - paddle.prev_pos.z);
			vec3 paddleChange = paddle.pos - oldPos;

			if (ballDir == 1) {
				ball.dir.z *= -1;

			}
			ball.dir += (paddleChange/seconds)*vec3(0.6,0,0.3f);
			ball.dir.y += 0.05*(length(paddleChange) / seconds + length(ball.dir));

			ball.pos.z = paddle.pos.z - 0.15;

		}
		//net hit?
		if (ping_pong::ballHitNet(ball.pos,ball.prev_pos)) {
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
		if (ping_pong::ballHitWall(ball.pos,ball.prev_pos)) {
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

		//need to unbind and reset viewport after
		standard_shader::setupDepthShader(*depthShader, depthTexture, light);
		//standard_shader::setupDepthCubemapShader(depthCubemapShader,depthCubemap,light);
		//draw 
		//paddle.draw(depthCubemapShader);
		//ball.draw(depthCubemapShader);
		//table.draw(depthCubemapShader);
		//ping_pong::drawFloor(depthCubemapShader);

		paddle.draw(*depthShader);
		ball.draw(*depthShader);
		table.draw(*depthShader);
		ping_pong::drawFloor(*depthShader);

		//ping_pong::drawRoomWalls(depthShader);


		//depthCubemap.unbind();
		depthTexture.unbind();
		utils::resetViewport();
		//utils::displayTexture2D(depthTexture.texture());
		//utils::displayTexture(depthCubemap.cubemap());

		//Camera cubeCam(vec3(0), aim.front*vec3(1, -1, 1), (1920.f / 1080.f));
		utils::drawSkybox(camera, skybox);
		//glDepthMask(GL_TRUE);

		basicShader->use();


		/*basicShader.setUniform("camera_view", light.view());
		basicShader.setUniform("camera_projection", light.proj());*/
		standard_shader::setCamera(*basicShader, camera);
		standard_shader::setLight(*basicShader, light);
		standard_shader::setMaterial(*basicShader, mat);
		standard_shader::setSpotightMatrices(*basicShader, light);
		standard_shader::setShadowMap(*basicShader, depthTexture.texture());
		standard_shader::setTexture2D(*basicShader, white_texture);
		//utils::displayTexture(white_texture);

		paddle.draw(*basicShader);
		ball.draw(*basicShader);
		table.draw(*basicShader);

		standard_shader::setTexture2D(*basicShader, wood_texture);
		ping_pong::drawFloor(*basicShader);

		standard_shader::setTexture2D(*basicShader, white_texture);
		//ping_pong::drawRoomWalls(*basicShader);


		//utils::displayTexture(depthTexture.texture());
		//Light
		//Should standardize this shader at some point
		unlitShader->use();
		unlitShader->setUniform("projection", camera.proj());
		unlitShader->setUniform("view", camera.view());
		unlitShader->setUniform("object_color", vec3(1, 1, 1));

		{
			shared_ptr<ArrayMesh> cube = utils::getCube();
			cube->bind();
			mat4 model = mat4(1.0);
			model = translate(model, light.position);
			model = scale(model, vec3(1.f/16));
			unlitShader->setUniform("model", model);
			unlitShader->setUniform("object_color", light.specular);
			standard_shader::drawArrayMesh(*cube);
		}


		//utils::displayTexture(gorilla);
		

		glfwSwapBuffers(window);
	}
	utils::clean();
	glfwTerminate();
	return 0;
}
