#include "Utils.h"
using namespace std;
using namespace basicgraphics;
using namespace glm;
namespace utils {

	void resetViewport() {
		glViewport(0, 0, 1920, 1080);
	}

	bool bounded(vec3 vec, vec3 min, vec3 max) {
		return clamp(vec, min, max) == vec;
	}
	bool bounded(float f, float min, float max) {
		return clamp(f, min, max) == f;
	}

	// returns VBO id;

	GLuint makeQuadGrid(int dim) {
		vector<vec3> vertices(dim*dim);
		auto index = vertices.begin();
		for (int i = 0; i < dim; i++) {
			for (int j = 0; j < dim; j++) {
				*index = vec3(j*2.f / (dim-1) - 1, i*2.f / (dim-1) - 1, 0);
				index++;
			}
		}
		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*dim*dim, &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return VBO;
	}

	// returns EBO id;
	Indices makeQuadGridIndices(int dim) {
		vector<GLuint> indices((dim-1)*(dim-1)*6);
		int index = 0;
		for (int i = 0; i < dim-1; i++) {
			for (int j = 0; j < dim-1; j++) {
				indices[index] = i*dim + j;
				indices[index+1] = (i+1)*dim + j+1;
				indices[index+2] = (i+1)*dim + j;
				indices[index+3] = i*dim+j;
				indices[index+4] = i*dim + j+1;
				indices[index+5] = (i+1)*dim + j+1;
				index+=6;
			}
		}
		GLuint EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ARRAY_BUFFER, EBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint)*6*dim*dim, &indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return {
			EBO,
			(dim - 1)*(dim - 1) * 6
		};
	}


	// The square should be accessed through getSquare, and will auto-load if not loaded. 
	// It is a 2D square.

	shared_ptr<IndexedMesh> _square = NULL;
	void loadSquare() {
		GLfloat vertices[] = {
			// Positions          // Norms           // Texture Coords
			1.0f,  1.0f, 0.0f,   0.f, 0.0f, 1.0f,   1.0f, 1.0f,   // Top Right
			1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // Bottom Right
			-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
			-1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f    // Top Left 
		};
		GLuint indices[] = {  // Note that we start from 0!
			0, 1, 2,
			2, 3, 0,// First Triangle
		};
		IndexedMesh * square = new IndexedMesh();
		_square.reset(square);
		_square->bind();
		_square->loadVertexData(sizeof(vertices), vertices, GL_STATIC_DRAW);
		_square->loadIndexData(sizeof(indices), indices, GL_STATIC_DRAW);
		_square->setElements(6);
		_square->unbind();
	}
	shared_ptr<IndexedMesh> getSquare() {
		if (_square == NULL) {
			loadSquare();
		}
		return _square;
	}
	//Utils and state needed to display a texture on the square mesh center screen. Has a custom shader.
	// This namespace is internal only, however there is a function right after it which utilizes its machinery.
	// *DEBUGGING UTIL*
	// Compiles shader on first call
	// May also load geometry (of square) on call
	void displayTexture2D(Texture2D& tex) {
		GLSLProgram & tex2DShader = *shaderregistry::loadShader(shaderregistry::UTIL_TEXTURE2D);
		shared_ptr<Mesh> square = getSquare();

		tex2DShader.use();

		glActiveTexture(GL_TEXTURE0);
		tex.bind();
		tex2DShader.setUniform("Texture0", 0);

		square->bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		square->unbind();
		
	}
	void drawSkybox(Camera cam, Cubemap & cubemap) {
		glDepthMask(GL_FALSE); // draw in background only 
		GLSLProgram & cubemapShader = *shaderregistry::loadShader(shaderregistry::UTIL_CUBEMAP);
		cubemapShader.use();
		auto cube = getCube();

		glActiveTexture(GL_TEXTURE0);
		cubemap.bind();
		cubemapShader.setUniform("cubemap", cubemap.id());

		skybox_shader::setCamera(cubemapShader,cam);
		standard_shader::drawArrayMesh(*cube);
		glDepthMask(GL_TRUE);
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

	shared_ptr<ArrayMesh> _cube = NULL;
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
		_cube.reset(new ArrayMesh());
		_cube->bind();
		_cube->loadVertexData(sizeof(vertices), vertices, GL_STATIC_DRAW);
		_cube->setCount(36);
		_cube->unbind();
	}
	// use glDrawArrays
	shared_ptr<ArrayMesh> getCube() {
		if (_cube == NULL)
		{
			loadCube();
		}
		return _cube;
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
		shader.setUniform("light.direction", light.direction);

		shader.setUniform("light.ambient", light.ambient);
		shader.setUniform("light.diffuse", light.diffuse);
		shader.setUniform("light.specular", light.specular);
		
		shader.setUniform("light.constant", light.constant);
		shader.setUniform("light.linear", light.linear);
		shader.setUniform("light.quadratic", light.quadratic);

		shader.setUniform("light.cutOff", light.cutOff);

	}
	

	Attributeless::Attributeless()
	{
		glGenVertexArrays(1, &_VAO);
	}

	Attributeless::~Attributeless()
	{
		glDeleteVertexArrays(1, &_VAO);
	}

	void Attributeless::bind()
	{
		glBindVertexArray(_VAO);
	}

	void Attributeless::draw(GLenum primitive, int numVertices)
	{
		glBindVertexArray(_VAO);
		glDrawArraysInstanced(primitive, 0, 1, numVertices);

	}

}
