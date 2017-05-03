#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in float dens;
layout (location = 2) in float temp;
layout (location = 3) in int object;
layout (location = 4) in int generators;


uniform int showGenerators;

out vec3 vertColor;

void main() {
	vertColor = vec3(0,0,0.2);
	vertColor += object* vec3(1,1,0);
	vertColor += showGenerators * generators * vec3(1,1,1);
    vec3 fireColor = vec3(temp)*vec3(1, 0.7 - (1-temp/7)/2.1,0);
    vec3 fireCore = vec3(temp) * vec3(1.0,1.0,1.0);
    vec3 smokeColor = vec3(0.2 * dens);
    vertColor += (1-temp)*smokeColor +  (0.1*temp*fireCore) + (0.8*temp*fireColor);
	gl_Position = vec4(pos,1);

}

