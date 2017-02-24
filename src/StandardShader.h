#pragma once
#include "GLSLProgram.h"
#include "Mesh.h"
#include "Light.h"
#include "Spotlight.h"
#include "Camera.h"
#include "Utils.h"
#include "Spotlight.h"
#include "DepthTexture.h"
#include "DepthCubemap.h"
#include "DeferredGeom.h"

#include <vector>


namespace skybox_shader {
	void setCamera(basicgraphics::GLSLProgram & shader, Camera camera);
}
namespace standard_shader {
	struct Scene {
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
		Camera camera;
		Light single_light;
	};
	struct Model {
		IndexedMesh& mesh;
		Material& mat;
	};
	//Sets projection matrix, view matrix, and view position to _camera data.
	void setCamera(basicgraphics::GLSLProgram & shader, Camera cam);
	void setMaterial(basicgraphics::GLSLProgram & shader, Material mat);
	void setLight(basicgraphics::GLSLProgram & shader, Light light);
	//sets the model and normal_model matrices for the standard shader
	void setModel(basicgraphics::GLSLProgram & shader, mat4 model);
	void drawModel(basicgraphics::GLSLProgram& shader,Model model);
	void drawIndexedMesh(IndexedMesh& mesh);
	void drawIndexedGPUReference(IndexedGPUMeshReference m);
	void drawIndexedMeshes(std::vector<IndexedGPUMeshReference> meshes);
	void setShadowMap(basicgraphics::GLSLProgram & shader, Texture2D & shadowMap);
	void setShadowCubemap(basicgraphics::GLSLProgram & shader, Cubemap & shadowCube);
	void setTexture2D(basicgraphics::GLSLProgram & shader, Texture2D & texture);
	void drawArrayMesh(ArrayMesh& mesh);
	void setSpotightMatrices(basicgraphics::GLSLProgram & shader, Spotlight l);
	//need to unbind and reset viewport after
	void setupDepthShader(basicgraphics::GLSLProgram & shader, DepthTexture & dt, Spotlight l);
	// uses _light.farPlane, and others. calls use() on shader
	void setupDepthCubemapShader(basicgraphics::GLSLProgram & shader, DepthCubemap & dcm, Light l);
	// uses TEXTURE2,3,4 for its content
	void setDeferredGeom(basicgraphics::GLSLProgram & shader, DeferredGeom & dg);
};

