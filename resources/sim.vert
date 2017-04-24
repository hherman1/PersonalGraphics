#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in float dens;

out vec3 vertColor;

void main() {
	vertColor = vec3(dens);
	gl_Position = vec4(pos,1);

}