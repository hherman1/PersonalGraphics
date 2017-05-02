#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in float dens;
layout (location = 2) in float temp;
layout (location = 3) in int object;
layout (location = 4) in int generators;

out vec3 vertColor;

void main() {
	vertColor = vec3(0,0,0);
	vertColor += object* vec3(0,1,0);
	vertColor += generators * vec3(1,1,0);
	vertColor += vec3(dens) * vec3(1,0.85 - (1-temp)/1.2,0);
	gl_Position = vec4(pos,1);

}

