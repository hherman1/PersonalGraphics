#pragma once
#include "GLSLProgram.h"
#include <memory>
#include <vector>

namespace shaderregistry {

	enum Shader {
		BASIC,
		UNLIT,
		DEPTH,
		DEPTH_CUBEMAP,
		DEFERRED_GEOM,
		DEFERRED_LIGHT,
		UTIL_TEXTURE2D,
		UTIL_CUBEMAP,
		COUNT
	};
	//fetches shader and will compile if it is not already compiled
	std::shared_ptr<basicgraphics::GLSLProgram> loadShader(Shader target);
	//load shader but will not compile the shader
	std::shared_ptr<basicgraphics::GLSLProgram> fetchShader(Shader target);
	std::shared_ptr<basicgraphics::GLSLProgram> makeShader(std::vector<string> files);
	void buildShader(Shader target);
	void buildAllShaders();
}
