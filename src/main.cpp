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
#include "Mesh.h"
#include <SOIL.h>
#include "Texture.h"
#include "Utils.h"

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

GLFWwindow* init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
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
	Mesh triangle;
	triangle.bind();
	triangle.loadVertexData(sizeof(vertices), vertices, GL_STATIC_DRAW);
	triangle.loadIndexData(sizeof(indices), indices, GL_STATIC_DRAW);
	triangle.unbind();

	GLSLProgram shader;
	shader.compileShader("basic.vert");
	shader.compileShader("basic.frag");
	shader.link();

	
	//textures
	/*Texture cabinet;
	cabinet.loadImage("container.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	cabinet.unbind();

	Texture earth;
	earth.loadImage("flat-earth.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	earth.unbind();*/
	Texture gorilla;
	gorilla.loadImage("gorilla.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gorilla.unbind();


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*shader.use();
		shader.setUniform("green", (float)(sin(glfwGetTime()) / 2 + 0.5f));

		glActiveTexture(GL_TEXTURE0);
		cabinet.bind();
		shader.setUniform("Texture0", 0);

		glActiveTexture(GL_TEXTURE1);
		earth.bind();
		shader.setUniform("Texture1", 1);

		shared_ptr<Mesh> square = utils::getSquare();
		square->bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		square->unbind();*/
		
		utils::displayTexture(gorilla);
		

		glfwSwapBuffers(window);
	}
	utils::clean();
	glfwTerminate();
	return 0;
}
