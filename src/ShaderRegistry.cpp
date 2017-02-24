#include "ShaderRegistry.h"
#include <vector>

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
	// not always useful
	void buildShaderFrom(Shader target, vector<string> files) {
		GLSLProgram * shader = new GLSLProgram();
		for (auto i = files.begin(); i < files.end(); i++) {
			shader->compileShader(i->c_str());
		}
		shader->link();
		_internal_shaders[target] = shared_ptr<GLSLProgram>(shader);
	}
	void buildShader(Shader target)
	{
		switch (target)
		{
		case shaderregistry::BASIC:
		{
			buildShaderFrom(shaderregistry::BASIC, vector<string> {"basic.vert","basic.frag"});
			break;
		}
		case shaderregistry::UNLIT: 
		{
			buildShaderFrom(shaderregistry::UNLIT, vector<string> {"unlit.vert", "unlit.frag"});
			break;
		}
		case shaderregistry::DEPTH:
		{
			buildShaderFrom(shaderregistry::DEPTH, vector<string> {"depth.vert", "depth.frag"});
			break;
		}
		case shaderregistry::DEPTH_CUBEMAP:
		{
			buildShaderFrom(shaderregistry::DEPTH_CUBEMAP, vector<string> {"depth_cubemap.vert","depth_cubemap.geom", "depth_cubemap.frag"});
			break;
		}
		case shaderregistry::DEFERRED_GEOM:
		{
			buildShaderFrom(shaderregistry::DEFERRED_GEOM, vector<string> {"deferred_geom.vert", "deferred_geom.frag"});
			break;
		}
		case shaderregistry::DEFERRED_LIGHT:
		{
			buildShaderFrom(shaderregistry::DEFERRED_LIGHT, vector<string> {"deferred_light.vert", "deferred_light.frag"});
			break;
		}
		case shaderregistry::UTIL_TEXTURE2D:
		{
			buildShaderFrom(shaderregistry::UTIL_TEXTURE2D, vector<string> {"textureUtil.vert", "textureUtil.frag"});
			break;
		}
		case shaderregistry::UTIL_CUBEMAP:
		{
			buildShaderFrom(shaderregistry::UTIL_CUBEMAP, vector<string> {"cubemapUtil.vert", "cubemapUtil.frag"});
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
			buildShader(static_cast<Shader> (shaderregistry::BASIC + i));
		}
	}
}