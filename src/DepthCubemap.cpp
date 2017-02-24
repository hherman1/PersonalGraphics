#include "DepthCubemap.h"

using namespace std;
using namespace glm;

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

namespace depthcubemap {
	mat4 genPerspective() {
		GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
		GLfloat _near(0.1f);
		GLfloat _far(25.0f);
		return perspective(glm::radians(90.0f), aspect, _near, _far);
	}
	// proj * view
	vector<mat4> genTransforms(vec3 pos) {
		mat4 shadowProj = genPerspective();
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(pos, pos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(pos, pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(pos, pos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(pos, pos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(pos, pos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(pos, pos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
		return shadowTransforms;
	}
}

Cubemap& DepthCubemap::cubemap() {
	return _cubemap;
}
void DepthCubemap::unbind() {
	_framebuffer.unbind();
}

DepthCubemap::DepthCubemap():
	_framebuffer(),
	_cubemap()
{
	_framebuffer.bind();
	_cubemap.bind();
	for (GLuint i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _cubemap.id(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	_framebuffer.unbind();

}
void DepthCubemap::bind() {
	_framebuffer.bind();
}
void DepthCubemap::setViewport() {
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
}


DepthCubemap::~DepthCubemap()
{
}
