#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in float dens;
layout (location = 2) in float temp;
layout (location = 3) in int object;

out vec3 vertColor;

void main() {
	vertColor = float(1-object) * vec3(dens) * vec3(1,0.5 + (1-temp)/2,1-temp) + float(object) * vec3(0.64,0.16,0.16);
	gl_Position = vec4(pos,1);

}