#pragma once
#include <algorithm>
#include <vector>
#include "Graphics.h"
#include "Utils.h"

namespace simulator {
	int getSize();
	int getDim();
	float * getDens();
	float * getTemp();
	int * getGens();
	void inputDens(glm::vec2 coords);
	void inputTemp(vec2 coords);
	void inputVel(vec2 coords, vec2 vel);
	void inputObjs(vec2 coords);
	void inputGens(vec2 coords);
	void simulate(float dt);

}

class FireRender {
public:
	FireRender();
	FireRender(FireRender&& move) = delete;
	FireRender(const FireRender& that) = delete;
	FireRender& operator=(const FireRender&) = delete;
	void update();
	~FireRender();

	void draw();

private:
	int elements;
	GLuint EBO;
	GLuint VAO;
	GLuint VBO;

	GLuint DBO; // Density buffer
	GLuint RBO; // React coordinate buffer
	GLuint OBO; // object buffer
	GLuint GBO; // generator buffer

};


