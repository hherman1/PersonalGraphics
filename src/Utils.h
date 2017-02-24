#pragma once
#include "Graphics.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "GLSLProgram.h"
#include "Light.h"
#include "ShaderRegistry.h"
#include "Cubemap.h"
#include "StandardShader.h"
#include <memory>
namespace utils {
	void displayTexture2D(Texture2D& tex);
	void drawSkybox(Camera cam, Cubemap & cubemap);
	void resetViewport();
	bool bounded(glm::vec3 vec, glm::vec3 min, glm::vec3 max);
	bool bounded(float f, float min, float max);
	std::shared_ptr<IndexedMesh> getSquare();
	std::shared_ptr<ArrayMesh> getCube();
	mat3 normal_model(mat4 model);
	void setMaterial(basicgraphics::GLSLProgram & shader, Material mat);
	void setLight(basicgraphics::GLSLProgram & shader, Light light);
}
