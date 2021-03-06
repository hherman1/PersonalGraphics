#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 vertex_tex_coords;


out vec3 vert_color;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
	vert_color = color;
	TexCoord = vertex_tex_coords;
}