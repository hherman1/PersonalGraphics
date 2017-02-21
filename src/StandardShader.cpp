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
namespace standard_shader {
	
	void setCamera(GLSLProgram& shader, Camera camera) {
		shader.setUniform("camera_projection", camera.proj());
		shader.setUniform("camera_view", camera.view());
		shader.setUniform("camera_view_pos", camera.pos);
	}
	void setMaterial(GLSLProgram& shader, Material mat) {
		shader.setUniform("material.ambient", mat.ambient);
		shader.setUniform("material.diffuse", mat.diffuse);
		shader.setUniform("material.specular", mat.specular);
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
}

