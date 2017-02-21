#include "Utils.h"
using namespace std;
using namespace basicgraphics;
namespace utils {


	// The square should be accessed through getSquare, and will auto-load if not loaded. 
	// It is a 2D square.
	shared_ptr<Mesh> _square = NULL;
	void loadSquare() {
		GLfloat vertices[] = {
			// Positions          // Colors           // Texture Coords
			1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
			1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
			-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
			-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
		};
		GLuint indices[] = {  // Note that we start from 0!
			0, 1, 2,
			2, 3, 0,// First Triangle
		};
		Mesh * square = new Mesh();
		_square.reset(square);
		_square->bind();
		_square->loadVertexData(sizeof(vertices), vertices, GL_STATIC_DRAW);
		_square->loadIndexData(sizeof(indices), indices, GL_STATIC_DRAW);
		_square->setElements(6);
		_square->unbind();
	}
	shared_ptr<Mesh> getSquare() {
		if (_square == NULL) {
			loadSquare();
		}
		return _square;
	}
	//Utils and state needed to display a texture on the square mesh center screen. Has a custom shader.
	// This namespace is internal only, however there is a function right after it which utilizes its machinery.
	namespace display_texture {
		GLSLProgram texShader;
		bool texShader_loaded = false;
		void loadShader() {
			texShader_loaded = true;
			texShader.compileShader("texShader.vert");
			texShader.compileShader("texShader.frag");
			texShader.link();
		}
	}
	// *DEBUGGING UTIL*
	// Compiles shader on first call
	// May also load geometry (of square) on call
	void displayTexture(Texture& tex) {
		using namespace display_texture;
		if (!texShader_loaded) {
			loadShader();
		}
		shared_ptr<Mesh> square = getSquare();

		texShader.use();

		glActiveTexture(GL_TEXTURE0);
		tex.bind();
		texShader.setUniform("Texture0", 0);

		square->bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		square->unbind();
		
	}

	
	shared_ptr<Mesh> _indexed_cube = NULL;
	void loadIndexedCube() {
		struct format {
			vec3 pos;
			vec2 texCoords;
		};
		vec3 defaultNorm(0, 0, 0);
		GLfloat cube_vertices[] = {
			// front
			-1.0, -1.0,  1.0,
			1.0, -1.0,  1.0,
			1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
			// back
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,
			1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0,
		};
		GLfloat cube_texcoords[] = {
			// front
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
			// back
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
		};

		GLuint indices[] = {
			// front
			0, 1, 2,
			2, 3, 0,
			// top
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// bottom
			4, 0, 3,
			3, 7, 4,
			// left
			4, 5, 1,
			1, 0, 4,
			// right
			3, 2, 6,
			6, 7, 3,
		};
		vector<Vertex> Vertices;
		for (int i = 0; i < 8;i++) {
			Vertex v = {
				((vec3*)cube_vertices)[i],
				defaultNorm,
				((vec2*)cube_texcoords)[i]
			};
			Vertices.push_back(v);
		}
		Mesh * cube = new Mesh();
		cube->bind();
		cube->loadVertexData(Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
		cube->loadIndexData(sizeof(indices), indices, GL_STATIC_DRAW);
		cube->setElements(36);
		cube->unbind();
		_indexed_cube.reset(cube);
	}
	shared_ptr<Mesh> getIndexedCube() {
		if (_indexed_cube == NULL)
		{
			loadIndexedCube();
		}
		return _indexed_cube;
	}

	shared_ptr<Mesh> _cube = NULL;
	void loadCube() {
		GLfloat vertices[] = {
			// Positions           // Normals           // Texture Coords
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
		};
		Mesh * cube = new Mesh();
		cube->bind();
		cube->loadVertexData(sizeof(vertices), vertices, GL_STATIC_DRAW);
		cube->setElements(36);
		cube->unbind();
		_cube.reset(cube);
	}
	// use glDrawArrays
	shared_ptr<Mesh> getCube() {
		if (_cube == NULL)
		{
			loadCube();
		}
		return _cube;
	}
	//call before glfwTerminate
	void clean() {
		
	}
	mat3 normal_model(mat4 model) {
		return mat3(transpose(inverse(model)));
	}
	void setMaterial(GLSLProgram& shader, Material mat) {
		shader.setUniform("material.ambient",mat.ambient);
		shader.setUniform("material.diffuse", mat.diffuse);
		shader.setUniform("material.specular", mat.specular);
		shader.setUniform("material.shininess", mat.shininess);
	}
	void setLight(GLSLProgram& shader, Light light) {
		shader.setUniform("light.position", light.position);
		shader.setUniform("light.ambient", light.ambient);
		shader.setUniform("light.diffuse", light.diffuse);
		shader.setUniform("light.specular", light.specular);
	}

}
