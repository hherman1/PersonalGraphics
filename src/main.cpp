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


int w_width = 1920;
int w_height = 1080;



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


	glEnable(GL_DEPTH_TEST);


	return window;
}

void wireframe() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char** argv)
{
	
	GLFWwindow* window = init();
	pingponggame::init(window, w_width, w_height);

	

	//Keep these as shared_ptrs so they arent accidentally deallocated.
	shaderregistry::buildAllShaders();
	shared_ptr<GLSLProgram> depthShader = shaderregistry::loadShader(shaderregistry::DEPTH);
	shared_ptr<GLSLProgram> depthCubemapShader = shaderregistry::loadShader(shaderregistry::DEPTH_CUBEMAP);
	shared_ptr<GLSLProgram> basicShader = shaderregistry::loadShader(shaderregistry::BASIC);
	shared_ptr<GLSLProgram> unlitShader = shaderregistry::loadShader(shaderregistry::UNLIT);

	shared_ptr<IndexedMeshes> sphere = importer::loadModel("sphere.obj");
	shared_ptr<IndexedMeshes> cylinder = importer::loadModel("cylinder.obj");
	shared_ptr<IndexedMeshes> cone = importer::loadModel("cone.obj");
	
	DepthTexture depthTexture;
	DepthCubemap depthCubemap;

	Clock clock;

	float rotation = 0;

	Material mat = {
		vec3(1.0f, 0.5f, 0.31f),vec3(1.0f, 0.5f, 0.31f),vec3(0.5f, 0.5f, 0.5f),32.0f
	};

	Texture2D white_texture;
	white_texture.whiteTexture();
	Texture2D wood_texture;


	Cubemap skybox;
	string files[6] = { "skybox/right.jpg","skybox/left.jpg","skybox/top.jpg","skybox/bottom.jpg","skybox/back.jpg","skybox/front.jpg" };
	skybox.loadCubemap(files);

	vec3 ball_pos;


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float seconds = clock.tick();
		
		pingponggame::update(seconds);

		//gorilla.bind();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

		GLSLProgram &depthCubemapShader = *shaderregistry::loadShader(shaderregistry::DEPTH_CUBEMAP);
		standard_shader::setupDepthCubemapShader(depthCubemapShader,depthCubemap,pingponggame::light());
		//draw 
		pingponggame::drawGeometry(depthCubemapShader);

		depthCubemap.unbind();
		utils::resetViewport();

		//need to unbind and reset viewport after
		standard_shader::setupDepthShader(*depthShader, depthTexture, pingponggame::light());

		//pingponggame::drawGeometry(*depthShader);

		//ping_pong::drawRoomWalls(depthShader);


		//depthCubemap.unbind();
		depthTexture.unbind();
		utils::resetViewport();
		//utils::displayTexture2D(depthTexture.texture());
		//utils::displayTexture(depthCubemap.cubemap());

		//Camera cubeCam(vec3(0), aim.front*vec3(1, -1, 1), (1920.f / 1080.f));
		//utils::drawSkybox(_camera, skybox);

		utils::drawSkybox(pingponggame::camera(), skybox);
		//utils::drawSkybox(pingponggame::camera(), depthCubemap.cubemap());
		//glDepthMask(GL_TRUE);

		basicShader->use();


		/*basicShader.setUniform("camera_view", _light.view());
		basicShader.setUniform("camera_projection", _light.proj());*/
		standard_shader::setCamera(*basicShader, pingponggame::camera());
		standard_shader::setLight(*basicShader, pingponggame::light());
		standard_shader::setMaterial(*basicShader, mat);
		standard_shader::setSpotightMatrices(*basicShader, pingponggame::light());
		standard_shader::setShadowMap(*basicShader, depthTexture.texture());
		standard_shader::setShadowCubemap(*basicShader, depthCubemap.cubemap());
		standard_shader::setTexture2D(*basicShader, white_texture);
		//utils::displayTexture(white_texture);

		pingponggame::drawGeometry(*basicShader);


		standard_shader::setTexture2D(*basicShader, white_texture);
		//ping_pong::drawRoomWalls(*basicShader);


		//utils::displayTexture(depthTexture.texture());
		//Light
		//Should standardize this shader at some point
		unlitShader->use();
		unlitShader->setUniform("projection", pingponggame::camera().proj());
		unlitShader->setUniform("view", pingponggame::camera().view());
		unlitShader->setUniform("object_color", vec3(1, 1, 1));

		{
			shared_ptr<ArrayMesh> cube = utils::getCube();
			cube->bind();
			mat4 model = mat4(1.0);
			model = translate(model, pingponggame::light().position);
			model = scale(model, vec3(1.f/16));
			unlitShader->setUniform("model", model);
			unlitShader->setUniform("object_color", pingponggame::light().specular);
			standard_shader::drawArrayMesh(*cube);
		}


		//utils::displayTexture(gorilla);
		

		glfwSwapBuffers(window);
	}
	utils::clean();
	glfwTerminate();
	return 0;
}
