#include "Utils.h"
using namespace std;
using namespace basicgraphics;
namespace utils {


	// The square should be accessed through getSquare, and will auto-load if not loaded. 
	// It is a 2D square.
	Mesh * _square = NULL;
	bool square_loaded = false;
	void loadSquare() {
		square_loaded = true;
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
		_square = new Mesh();
		_square->bind();
		_square->loadVertexData(sizeof(vertices), vertices, GL_STATIC_DRAW);
		_square->loadIndexData(sizeof(indices), indices, GL_STATIC_DRAW);
		_square->unbind();
	}
	Mesh* getSquare() {
		if (!square_loaded) {
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
		Mesh * square = getSquare();

		texShader.use();

		glActiveTexture(GL_TEXTURE0);
		tex.bind();
		texShader.setUniform("Texture0", 0);

		square->bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		square->unbind();
		
	}
	


	//call before glfwTerminate
	void clean() {
		
	}
}
