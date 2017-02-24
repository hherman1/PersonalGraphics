#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vert_normal;
layout (location = 2) in vec2 vertex_tex_coords;

uniform mat4 model;
uniform mat3 normal_model;
uniform mat4 camera_view;
uniform mat4 camera_projection;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position,1);
	TexCoord = vertex_tex_coords;
}