#version 330 core

uniform int vertexCount;
uniform int w_width;
uniform int w_height;

uniform sampler2D positions;

out vec3 vert_color;

uniform mat4 transform;

void main() {
	float y = float(gl_InstanceID/w_width);
	float x = float(gl_InstanceID % w_width);

	vec4 pos = texture(positions,vec2(x/w_width,y/w_height));


	gl_Position = transform * pos;
	//gl_Position = vec4(x/w_width,y/w_height,-1,1);
	vert_color = vec3(pos);
	//gl_Position = vec4(x/w_width,0,-1,1);
}