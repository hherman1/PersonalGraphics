#pragma once
#include "GLSLProgram.h"
#include <memory>

namespace shaderregistry {

	enum Shader {
		DEPTH,


		COUNT
	};
	//fetches shader and will compile if it is not already compiled
	std::shared_ptr<basicgraphics::GLSLProgram> loadShader(Shader target);
	//load shader but will not compile the shader
	std::shared_ptr<basicgraphics::GLSLProgram> fetchShader(Shader target);
	void buildShader(Shader target);
	void buildAllShaders();
}
