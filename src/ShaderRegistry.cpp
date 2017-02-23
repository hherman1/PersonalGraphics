#include "ShaderRegistry.h"

using namespace std;
using namespace basicgraphics;
namespace shaderregistry {
	shared_ptr<GLSLProgram> _internal_shaders[Shader::COUNT];

	std::shared_ptr<basicgraphics::GLSLProgram> loadShader(Shader target)
	{
		if (!_internal_shaders[target]) {
			buildShader(target);
		}
		return _internal_shaders[target];
	}

	std::shared_ptr<basicgraphics::GLSLProgram> fetchShader(Shader target)
	{
		if (_internal_shaders[target]) {
			return _internal_shaders[target];
		}
		else {
			throw std::runtime_error("Fetching uninitialized shader.");
		}
	}
	void buildShader(Shader target)
	{
		switch (target)
		{
		case shaderregistry::DEPTH:
		{
			GLSLProgram * depthShader = new GLSLProgram();
			depthShader->compileShader("depth.vert");
			depthShader->compileShader("depth.frag");
			depthShader->link();
			_internal_shaders[shaderregistry::DEPTH] = shared_ptr<GLSLProgram>(depthShader);
			break;
		}
		case shaderregistry::COUNT:
			break;
		default:
			break;
		}
	}
	void buildAllShaders()
	{
		for (int i = 0; i < Shader::COUNT; i++) {
			buildShader(shaderregistry::DEPTH + i);
		}
	}
}