#pragma once
#include <algorithm>
#include "Graphics.h"
#include "Utils.h"

namespace simulator {
	int getSize();
	int getDim();
	float * getDens();
	void inputDens(glm::vec2 coords);
	void inputVel(vec2 coords, vec2 vel);
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

};


