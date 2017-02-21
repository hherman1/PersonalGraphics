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

using namespace glm;
using namespace std;
using namespace basicgraphics;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

struct {
	double yaw = 0;
	double pitch = 0;
	vec3 front = vec3(1,0,0);
} aim ;
struct {
	double x = 0;
	double y = 0;
} mouse;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	GLfloat xoffset = xpos - mouse.x;
	GLfloat yoffset = mouse.y - ypos; // Reversed since y-coordinates range from bottom to top
	mouse.x = xpos;
	mouse.y = ypos;

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


int w_width = 1920;
int w_height = 1080;
GLFWwindow* init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(w_width, w_height, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		throw "Failed to create GLFW window";
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		throw "Failed to initialize GLEW";
	}
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


	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 2,
		2, 3, 0,// First Triangle
	};
	/*Vertex * Vertices = (Vertex*) vertices;
	vector<Vertex> VerticesVec(Vertices, Vertices + 4);*/
	//vector<Vertex> Vertices((Vertex*)vertices, (Vertex*)vertices + 4 * sizeof(Vertex));
	ArrayMesh triangle;
	triangle.bind();
	triangle.loadVertexData(sizeof(vertices), vertices, GL_STATIC_DRAW);
	triangle.loadIndexData(sizeof(indices), indices, GL_STATIC_DRAW);
	triangle.unbind();
	

	GLSLProgram unlitShader;
	unlitShader.compileShader("unlit.vert");
	unlitShader.compileShader("unlit.frag");
	unlitShader.link();

	GLSLProgram shader;
	shader.compileShader("basic.vert");
	shader.compileShader("basic.frag");
	shader.link();

	
	Texture gorilla;
	gorilla.loadImage("gorilla.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gorilla.unbind();

	auto proj = perspective(45.0f, (float)w_width / (float)w_height, 0.1f, 100.0f);
	glm::mat4 view;
	// Note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	shared_ptr<IndexedMeshes> sphere = importer::loadModel("sphere.obj");
	
	
	DepthTexture::compileShader();
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
	Spotlight light(vec3(2, 1, 0), vec3(0, 0, 0));
	light.ambient = vec3(0.2f, 0.2f, 0.2f);
	light.diffuse = vec3(0.5f, 0.5f, 0.5f); // Let's darken the light a bit to fit the scene
	light.specular = vec3(1.0f, 1.0f, 1.0f);
	Camera camera(vec3(3, 6, 3), vec3(0, 0, 0), (float)w_width / (float)w_height);

	Material mat = {
		vec3(1.0f, 0.5f, 0.31f),vec3(1.0f, 0.5f, 0.31f),vec3(0.5f, 0.5f, 0.5f),32.0f
	};
	

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float seconds = clock.tick();
		rotation += 180*seconds ;

		camera.pos = 10.f * aim.front;
		//camera.pos = vec3(rotate(mat4(1.0f), radians(rotation), vec3(1, 1, 1)) * vec4(camera.pos,1.f));


		shader.use();

		standard_shader::setCamera(shader, camera);
		standard_shader::setLight(shader, light);
		standard_shader::setMaterial(shader, mat);

		gorilla.bind();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		for (int i = 0; i < 10; i++) {
			mat4 model;
			model = translate(model, cubePositions[i]);
			//model = glm::rotate(model, (float)radians(rotation), glm::vec3(1.0f, 1.0f, 0.0f));
			model = scale(model, vec3(0.5f, 0.5f, 0.5f));

			standard_shader::setModel(shader, model);


			shared_ptr<ArrayMesh> cube = utils::getCube();
			standard_shader::drawArrayMesh(*cube);
		}

		{
			mat4 model;
			model = mat4(1.f);
			standard_shader::setModel(shader, model);
			standard_shader::setMaterial(shader, {
				vec3(0.24725,0.1995,0.0745),
				vec3(0.75164,0.60648,0.22648),
				vec3(0.628281,0.555802,0.366065),
				32.f
			});
			standard_shader::drawIndexedMeshes(sphere->indexedGPUReferences);
		}

		shared_ptr<ArrayMesh> cube = utils::getCube();
		mat4 model;
		model = translate(model, vec3(0, -2, 0));
		model = scale(model, vec3(4.f, 0.25f, 4.f));
		standard_shader::setModel(shader,model);
		standard_shader::drawArrayMesh(*cube);


		unlitShader.use();
		unlitShader.setUniform("projection", camera.proj());
		unlitShader.setUniform("view", camera.view());
		unlitShader.setUniform("object_color", vec3(1, 1, 1));

		{
			shared_ptr<ArrayMesh> cube = utils::getCube();
			cube->bind();
			model = mat4(1.0);
			model = translate(model, light.position);
			model = scale(model, vec3(0.25));
			unlitShader.setUniform("model", model);
			unlitShader.setUniform("object_color", light.specular);
			standard_shader::drawArrayMesh(*cube);
		}


		//utils::displayTexture(gorilla);
		

		glfwSwapBuffers(window);
	}
	utils::clean();
	glfwTerminate();
	return 0;
}
