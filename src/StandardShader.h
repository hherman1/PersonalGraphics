#pragma once
#include "GLSLProgram.h"
#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "Utils.h"

#include <vector>



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
	//Sets projection matrix, view matrix, and view position to camera data.
	void setCamera(basicgraphics::GLSLProgram & shader, Camera cam);
	void setMaterial(basicgraphics::GLSLProgram & shader, Material mat);
	void setLight(basicgraphics::GLSLProgram & shader, Light light);
	//sets the model and normal_model matrices for the standard shader
	void setModel(basicgraphics::GLSLProgram & shader, mat4 model);
	void drawModel(basicgraphics::GLSLProgram& shader,Model model);
	void drawIndexedMesh(IndexedMesh& mesh);
	void drawArrayMesh(ArrayMesh& mesh);
};

