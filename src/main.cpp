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

#include "Fire.h"

using namespace glm;
using namespace std;
using namespace basicgraphics;


int w_width = 1920;
int w_height = 1080;

struct {
	vec2 pressCoords;
	bool down;
	bool shiftDown;
	vec2 lastCoords;
	vec2 currentCoords;
	vec2 releaseCoords;

	vec2 screenCoords() {
		return vec2(currentCoords.x / w_width, currentCoords.y / w_height);
	}
	vec2 diff() {
		return currentCoords - lastCoords;
	}
} mouse;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	mouse.lastCoords = mouse.currentCoords;
	mouse.currentCoords = vec2(xpos, ypos);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_RELEASE) {
		mouse.down = false;
		mouse.releaseCoords = mouse.currentCoords;
	}
	else {
		mouse.down = true;
		if (mods & GLFW_MOD_SHIFT) {
			mouse.shiftDown = true;
		}
		else {
			mouse.shiftDown = false;
		}
		mouse.pressCoords = mouse.currentCoords;
	}
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


	glEnable(GL_DEPTH_TEST);


	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	return window;
}

void wireframe() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char** argv)
{
	
	GLFWwindow* window = init();
	//pingponggame::init(window, w_width, w_height);

	double time = glfwGetTime();
	cout << "Building shaders. ";
	shaderregistry::buildAllShaders();
	cout << "Finished in " << glfwGetTime() - time << " seconds." << endl;
	//Keep these as shared_ptrs so they arent accidentally deallocated.
	shared_ptr<GLSLProgram> depthShader = shaderregistry::loadShader(shaderregistry::DEPTH);
	shared_ptr<GLSLProgram> depthCubemapShader = shaderregistry::loadShader(shaderregistry::DEPTH_CUBEMAP);
	shared_ptr<GLSLProgram> basicShader = shaderregistry::loadShader(shaderregistry::BASIC);
	shared_ptr<GLSLProgram> unlitShader = shaderregistry::loadShader(shaderregistry::UNLIT);
	shared_ptr<GLSLProgram> deferredGeomShader = shaderregistry::loadShader(shaderregistry::DEFERRED_GEOM);
	shared_ptr<GLSLProgram> deferredLightShader = shaderregistry::loadShader(shaderregistry::DEFERRED_LIGHT);
	
	DepthTexture depthTexture;
	DepthCubemap depthCubemap;

	DeferredGeom deferredGeom(w_width,w_height);

	Clock clock;

	float rotation = 0;

	Material mat = {
		vec3(1.0f, 0.5f, 0.31f),vec3(1.0f, 0.5f, 0.31f),vec3(0.5f, 0.5f, 0.5f),32.0f
	};

	Texture2D white_texture;
	white_texture.whiteTexture();
	Texture2D wood_texture;


	//Cubemap skybox;
	//string files[6] = { "skybox/right.jpg","skybox/left.jpg","skybox/top.jpg","skybox/bottom.jpg","skybox/back.jpg","skybox/front.jpg" };

	//time = glfwGetTime();
	//cout << "Loading skybox. ";
	//skybox.loadCubemap(files);
	//cout << "Finished in " << glfwGetTime() - time << " seconds." << endl;

	vec3 ball_pos;

	//test bed


	//utils::Attributeless attributeless;

	std::shared_ptr<basicgraphics::GLSLProgram> simulationShader = shaderregistry::makeShader({ "sim.vert","sim.frag" });
	FireRender fire;



	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float seconds = clock.tick();
		
		//pingponggame::update(seconds);


		//gorilla.bind();

		glClearColor(0.f,0.f,1.f,0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		if (mouse.down && mouse.shiftDown) {
			simulator::inputVel(mouse.screenCoords(), mouse.diff());
		} else if (mouse.down ) {
			simulator::inputDens(mouse.screenCoords());
		}
		simulator::simulate(seconds);
		fire.update();

		simulationShader->use();
		fire.draw();
		//glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, 0);


		//glPointSize(10);

		//float test[] = { 1,1,1,1,0,0,1,1,1,1 };

		//glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * simulator::getSize(), simulator::getDens());
		////glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(test), test);

		//attributelessShader->use();
		//attributelessShader->setUniform("dim", simulator::getDim());
		//attributeless.draw(GL_POINTS, simulator::getSize());
		////glDrawElements()

		//simulator::simulate(seconds);



		//// draw shadow cubemap
		//GLSLProgram &depthCubemapShader = *shaderregistry::loadShader(shaderregistry::DEPTH_CUBEMAP);
		//standard_shader::setupDepthCubemapShader(depthCubemapShader,depthCubemap,pingponggame::light());
		////draw 
		//pingponggame::drawGeometry(depthCubemapShader);

		//depthCubemap.unbind();
		//utils::resetViewport();

		////draw skybox
		//utils::drawSkybox(pingponggame::camera(), skybox);

		//// draw geometry textures
		//deferredGeomShader->use();
		//deferredGeom.bind();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//standard_shader::setCamera(*deferredGeomShader, pingponggame::camera());
		//standard_shader::setMaterial(*deferredGeomShader, mat);

		//pingponggame::drawGeometry(*deferredGeomShader);
		//deferredGeom.unbind();

		//utils::displayTexture2D(deferredGeom.position());

		//Camera testCam(vec3(0, 2 + TABLE_TOP, 2.25), vec3(0, TABLE_TOP, 0.25), (float)w_width / (float)w_height);

		//attributelessShader->setUniform("vertexCount", 50);
		//attributelessShader->setUniform("w_width", w_width);
		//attributelessShader->setUniform("w_height", w_height);
		//glActiveTexture(GL_TEXTURE0);
		//deferredGeom.position().bind();
		//attributelessShader->setUniform("positions",0);

		//attributelessShader->setUniform("transform", testCam.proj() * testCam.view());
		//attributelessShader->setUniform("transform", pingponggame::camera().proj() * pingponggame::camera().view());

		//glPointSize(1);

		//// render to screen		
		//shared_ptr<IndexedMesh> square = utils::getSquare();
		//deferredLightShader->use();

		//standard_shader::setDeferredGeom(*deferredLightShader, deferredGeom);
		//standard_shader::setCamera(*deferredLightShader, pingponggame::camera());
		//standard_shader::setLight(*deferredLightShader, pingponggame::light());
		//standard_shader::setShadowCubemap(*deferredLightShader, depthCubemap.cubemap());

		//standard_shader::drawIndexedMesh(*square);

		//
		//// copy depth values to re-enable forward rendering
		//deferredGeom.bind();
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Write to default framebuffer
		//glBlitFramebuffer(
		//	0, 0, w_width, w_height, 0, 0, w_width, w_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST
		//);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		////utils::displayTexture2D(deferredGeom.position());

		////utils::displayTexture2D(deferredGeom.normal());

		////basicShader->use();


		/////*basicShader.setUniform("camera_view", _light.view());
		////basicShader.setUniform("camera_projection", _light.proj());*/
		////standard_shader::setCamera(*basicShader, pingponggame::camera());
		////standard_shader::setLight(*basicShader, pingponggame::light());
		////standard_shader::setMaterial(*basicShader, mat);
		////standard_shader::setSpotightMatrices(*basicShader, pingponggame::light());
		////standard_shader::setShadowMap(*basicShader, depthTexture.texture());
		////standard_shader::setShadowCubemap(*basicShader, depthCubemap.cubemap());
		////standard_shader::setTexture2D(*basicShader, white_texture);
		//////utils::displayTexture(white_texture);

		////pingponggame::drawGeometry(*basicShader);


		////standard_shader::setTexture2D(*basicShader, white_texture);
		//////ping_pong::drawRoomWalls(*basicShader);


		////utils::displayTexture(depthTexture.texture());
		////Light
		////Should standardize this shader at some point
		//unlitShader->use();
		//unlitShader->setUniform("projection", pingponggame::camera().proj());
		//unlitShader->setUniform("view", pingponggame::camera().view());
		//unlitShader->setUniform("object_color", vec3(1, 1, 1));

		//{
		//	shared_ptr<ArrayMesh> cube = utils::getCube();
		//	cube->bind();
		//	mat4 model = mat4(1.0);
		//	model = translate(model, pingponggame::light().position);
		//	model = scale(model, vec3(1.f/16));
		//	unlitShader->setUniform("model", model);
		//	unlitShader->setUniform("object_color", pingponggame::light().specular);
		//	standard_shader::drawArrayMesh(*cube);
		//}


		//utils::displayTexture(gorilla);
		

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
