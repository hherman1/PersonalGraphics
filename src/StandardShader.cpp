#include "StandardShader.h"




//void ArrayMesh::draw()
//{
//	draw(GL_TRIANGLES);
//}
//void ArrayMesh::draw(GLenum mode)
//{
//	glDrawArrays(mode, 0, count());
//}
using namespace basicgraphics;
using namespace std;
namespace skybox_shader {
	void setCamera(GLSLProgram& shader, Camera camera) {
		shader.setUniform("camera_projection", camera.proj());
		shader.setUniform("camera_view", mat4(mat3(camera.view()))); // remove translations
		shader.setUniform("camera_view_pos", camera.pos);
	}
}
namespace standard_shader {

	void setCamera(GLSLProgram& shader, Camera camera) {
		shader.setUniform("camera_projection", camera.proj());
		shader.setUniform("camera_view", camera.view());
		shader.setUniform("camera_view_pos", camera.pos);
	}
	void setMaterial(GLSLProgram& shader, Material mat) {
		shader.setUniform("material.ambient", mat.ambient);
		shader.setUniform("material.diffuse", mat.diffuse);
		shader.setUniform("material.specular", length(mat.specular));
		shader.setUniform("material.shininess", mat.shininess);
	}
	void setLight(GLSLProgram& shader, Light light) {
		shader.setUniform("light.position", light.position);
		shader.setUniform("light.direction", light.direction);

		shader.setUniform("light.ambient", light.ambient);
		shader.setUniform("light.diffuse", light.diffuse);
		shader.setUniform("light.specular", light.specular);

		shader.setUniform("light.constant", light.constant);
		shader.setUniform("light.linear", light.linear);
		shader.setUniform("light.quadratic", light.quadratic);

		shader.setUniform("light.farPlane", light.farPlane);

		shader.setUniform("light.cutOff", light.cutOff);

	}
	void setModel(GLSLProgram& shader, mat4 model) {
		shader.setUniform("model", model);
		shader.setUniform("normal_model", utils::normal_model(model));
	}
	void drawModel(GLSLProgram& shader, Model model)
	{
		setMaterial(shader,model.mat);
		drawIndexedMesh(model.mesh);
	}
	void drawArrayMesh(ArrayMesh& mesh) {
		mesh.bind();
		glDrawArrays(GL_TRIANGLES, 0, mesh.count());
		mesh.unbind();
	}
	void drawIndexedMesh(IndexedMesh& mesh) {
		mesh.bind();
		glDrawElements(GL_TRIANGLES, mesh.elements(), GL_UNSIGNED_INT, 0);
		mesh.unbind();
	}
	void drawIndexedGPUReference(IndexedGPUMeshReference m) {
		gl_mesh::bind(m.meshReference);
		glDrawElements(GL_TRIANGLES, m.elements, GL_UNSIGNED_INT, 0);
		gl_mesh::unbind();
	}
	
	void drawIndexedMeshes(vector<IndexedGPUMeshReference> meshes) {
		for (auto i = meshes.begin(); i < meshes.end();i++) {
			drawIndexedGPUReference(*i);
		}
	}

	void setShadowMap(basicgraphics::GLSLProgram & shader, Texture2D & shadowMap) {
		glActiveTexture(GL_TEXTURE3);
		shadowMap.bind();
		shader.setUniform("shadowMap", 3);
	}
	void setShadowCubemap(basicgraphics::GLSLProgram & shader, Cubemap & shadowCube) {
		glActiveTexture(GL_TEXTURE3);
		shadowCube.bind();
		shader.setUniform("shadowCubemap", 3);
	}
	void setTexture2D(basicgraphics::GLSLProgram & shader, Texture2D & texture) {
		glActiveTexture(GL_TEXTURE4);
		texture.bind();
		shader.setUniform("_texture", 4);
	}
	void setTexture2DNormal(basicgraphics::GLSLProgram & shader, Texture2D & texture) {
		glActiveTexture(GL_TEXTURE5);
		texture.bind();
		shader.setUniform("_texture_normal", 5);
	}

}

void standard_shader::setSpotightMatrices(GLSLProgram & shader, Spotlight l)
{
	shader.setUniform("light_view", l.view());
	shader.setUniform("light_projection", l.proj());
}
//need to unbind and reset viewport after
void standard_shader::setupDepthShader(GLSLProgram & shader, DepthTexture & dt, Spotlight l) {
	shader.use();
	dt.bind();
	dt.setViewport();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	standard_shader::setSpotightMatrices(shader, l);
}

//depth cubemap
namespace standard_shader {

	void setPointlightMatrices(GLSLProgram & shader, Light l) {
		vector<mat4> shadowTransforms = depthcubemap::genTransforms(l.position);
		for (int i = 0; i < 6; i++) {
			string name = "shadowMatrices[" + to_string(i) + "]";
			shader.setUniform(name.c_str(), shadowTransforms[i]);
		}
	}
	void setCubemapPointlight(basicgraphics::GLSLProgram  & shader, Light l) {
		setPointlightMatrices(shader, l);
		shader.setUniform("lightPos", l.position);
		shader.setUniform("far_plane", l.farPlane);
	}
	void setupDepthCubemapShader(basicgraphics::GLSLProgram  & shader, DepthCubemap & dcm, Light l) {
		shader.use();
		dcm.bind();
		dcm.setViewport();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		setCubemapPointlight(shader, l);

		//dcm.setViewport();
	}
}

// deferred light
namespace standard_shader {
	void setDeferredGeom(basicgraphics::GLSLProgram & shader, DeferredGeom & dg) {
		glActiveTexture(GL_TEXTURE0);
		dg.position().bind();
		shader.setUniform("deferred.WorldPos", 0);

		glActiveTexture(GL_TEXTURE1);
		dg.normal().bind();
		shader.setUniform("deferred.Normal", 1);

		glActiveTexture(GL_TEXTURE2);
		dg.material().bind();
		shader.setUniform("deferred.Materials", 2);
	}

}