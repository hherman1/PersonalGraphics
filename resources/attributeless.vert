#version 330 core

layout (location = 0) in float density;

uniform int dim;

uniform sampler2D positions;

out vec3 vert_color;

uniform mat4 transform;

void main() {
	float y = float(gl_InstanceID / dim);
	float x = float(gl_InstanceID % dim);

	//vec4 pos = texture(positions,vec2(x/w_width,y/w_height));


	//gl_Position = transform * pos;
	
	gl_Position = vec4(x/dim *2 - 1,2*y/dim-1,-1,1);
	vert_color = vec3(density);
	//gl_Position = vec4(x/w_width,0,-1,1);
}