#pragma once
#include "Graphics.h"
#include "Mesh.h"
#include "Texture.h"
#include "GLSLProgram.h"
#include <memory>
namespace utils {
	void displayTexture(Texture& tex);
	Mesh* getSquare();
	void clean();
}
