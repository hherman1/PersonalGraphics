#pragma once
#include "Graphics.h"
#include "Mesh.h"
#include "Texture.h"
#include "GLSLProgram.h"
#include "Light.h"
#include <memory>
namespace utils {
	void displayTexture(Texture& tex);
	std::shared_ptr<Mesh> getSquare();
	std::shared_ptr<Mesh> getCube();
	void clean();
	mat3 normal_model(mat4 model);
	void setMaterial(basicgraphics::GLSLProgram & shader, Material mat);
	void setLight(basicgraphics::GLSLProgram & shader, Light light);
}
