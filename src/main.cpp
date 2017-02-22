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

#include "GLSLProgram.h"
#include "StandardShader.h"
#include "Mesh.h"
#include <SOIL.h>
#include "Texture.h"
#include "Utils.h"
#include "Clock.h"
#include "Framebuffer.h"
#include "DepthTexture.h"
#include "Spotlight.h"
#include "Camera.h"
#include "Importer.h"

//Ping pong assignment
#include "Paddle.h"
#include "Ball.h"
#include "Table.h"
#include "PingPong.h"

using namespace glm;
using namespace std;
using namespace basicgraphics;


bool keys[1024];
int w_width = 1920;
int w_height = 1080;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
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

	GLSLProgram unlitShader;
	unlitShader.compileShader("unlit.vert");
	unlitShader.compileShader("unlit.frag");
	unlitShader.link();

	GLSLProgram shader;
	shader.compileShader("basic.vert");
	shader.compileShader("basic.frag");
	shader.link();

	GLSLProgram depthShader;
	depthShader.compileShader("depth.vert");
	depthShader.compileShader("depth.frag");
	depthShader.link();

	
//	Texture gorilla;
//	gorilla.loadImage("gorilla.jpg");
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	gorilla.unbind();

	shared_ptr<IndexedMeshes> sphere = importer::loadModel("sphere.obj");
	shared_ptr<IndexedMeshes> cylinder = importer::loadModel("cylinder.obj");
	shared_ptr<IndexedMeshes> cone = importer::loadModel("cone.obj");
	//shared_ptr<IndexedMeshes> cube = importer::loadModel("cube.obj");

	
	DepthTexture depthTexture;

	Clock clock;

	float rotation = 0;

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};	
	Spotlight light(vec3(-1, 1, 0), vec3(0, 0, 0));
	light.ambient = vec3(0.2f, 0.2f, 0.2f);
	light.diffuse = vec3(1.f); // Let's darken the light a bit to fit the scene
	light.specular = vec3(1.0f, 1.0f, 1.0f);
	Camera camera(vec3(0, 1, 2), vec3(0, 0, 0.25), (float)w_width / (float)w_height);

	Material mat = {
		vec3(1.0f, 0.5f, 0.31f),vec3(1.0f, 0.5f, 0.31f),vec3(0.5f, 0.5f, 0.5f),32.0f
	};

	vec3 ball_pos;
	
	Table table;
	Paddle paddle;
	Ball ball;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float seconds = clock.tick();
		rotation += 180*seconds ;

		//Debug mode only.
		//camera.pos = 3.f * aim.front;


		shader.use();

		standard_shader::setCamera(shader, camera);
		standard_shader::setLight(shader, light);
		standard_shader::setMaterial(shader, mat);

		//gorilla.bind();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        {
            mat4 net = mat4(1.f);
            net = translate(net, vec3(0.0f, 0.0f,  0.0f));
            vec3 diffuse;
            shared_ptr<ArrayMesh> cube;
        
            for(float i = -(TABLE_WIDTH/2) + 0.1; i < (TABLE_WIDTH/2) - 0.1 ; i = i + 0.02){

                mat4 net_string = mat4(1.f);
                net_string = translate(net_string, vec3(i, 0.18f, 0.0f));
                net_string = scale(net_string, vec3(0.00125f, 0.05f, 0.00125f));
                standard_shader::setModel(shader, net * net_string);
                
                if(i < -(TABLE_WIDTH/2) + 0.1 || i > (TABLE_WIDTH/2) - 0.12){
                    diffuse = vec3(0.0f, 0.0f, 0.0f);
                } else {
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
            
            for(float x = 0.13f; x < 0.23; x = x + 0.02){
                
                mat4 model_make = mat4(1.f);
                model_make = translate(model_make, vec3(0.0f, x, 0.0f));
                model_make = scale(model_make, vec3((TABLE_WIDTH/2)-0.1, 0.00125f, 0.00125f));
                standard_shader::setModel(shader, net * model_make);
                if(x < 0.14f || x > 0.22){
                    diffuse = vec3(0.0f, 0.0f, 0.0f);
                } else {
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

		
		vec3 oldPos = paddle.pos;
		paddle.move(vec2(mouseScreenDiff.x,-mouseScreenDiff.y));
		mouseScreenDiff = vec2(0);
		//paddle.pos = paddle.pos + vec3(0.1*seconds);
		ball.update(seconds);
		if (ballHitPaddle(ball.pos,ball.prev_pos, oldPos, paddle.pos)) {
			//ball.dir = vec3(0);
			float ballDir = sign(ball.pos.z - ball.prev_pos.z);
			float paddleDir = sign(paddle.pos.z - paddle.prev_pos.z);
			vec3 paddleChange = paddle.pos - oldPos;
			//if (paddleDir != 0) {
			//	ball.dir = reflect(ball.dir, normalize(paddleChange));
			//	ball.dir += paddleChange;
			//	ball.pos.z = paddle.pos.z + paddleDir* 0.1;
			//	//ball.dir.z *= ballDir * paddleDir;
			//}
			//else {
			//	ball.dir.z *= -1;
			//	ball.pos.z = paddle.pos.z + sign(ball.dir.z)* 0.2;
			//}
			if (ballDir == 1) {
				ball.dir *= -1;

			}
			ball.dir += (paddleChange/seconds)*0.03f;
			//ball.dir.y +=length(paddleChange)/seconds;

			ball.pos.z = paddle.pos.z - 0.15;

		}
		if (keys[GLFW_KEY_SPACE])
			ball.launch();

		//need to unbind and reset viewport after
		standard_shader::setupDepthShader(depthShader, depthTexture, light);

		//draw 
		paddle.draw(depthShader);
		ball.draw(depthShader);
		table.draw(depthShader);

		depthTexture.unbind();
		utils::resetViewport();
		//utils::displayTexture(depthTexture.texture());

		shader.use();
		standard_shader::setSpotightMatrices(shader, light);
		glActiveTexture(GL_TEXTURE0);
		depthTexture.texture().bind();
		shader.setUniform("shadowMap", 0);
		paddle.draw(shader);
		ball.draw(shader);
		table.draw(shader);

		//utils::displayTexture(depthTexture.texture());
		//Light
		/*unlitShader.use();
		unlitShader.setUniform("projection", camera.proj());
		unlitShader.setUniform("view", camera.view());
		unlitShader.setUniform("object_color", vec3(1, 1, 1));

		{
			shared_ptr<ArrayMesh> cube = utils::getCube();
			cube->bind();
			mat4 model = mat4(1.0);
			model = translate(model, light.position);
			model = scale(model, vec3(1.f/16));
			unlitShader.setUniform("model", model);
			unlitShader.setUniform("object_color", light.specular);
			standard_shader::drawArrayMesh(*cube);
		}*/


		//utils::displayTexture(gorilla);
		

		glfwSwapBuffers(window);
	}
	utils::clean();
	glfwTerminate();
	return 0;
}
