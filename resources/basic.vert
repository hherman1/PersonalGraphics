#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vert_normal;
layout (location = 2) in vec2 vertex_tex_coords;

uniform mat4 model;
uniform mat3 normal_model;
uniform mat4 camera_view;
uniform mat4 camera_projection;

//shadow mapping only
uniform mat4 light_view;
uniform mat4 light_projection;

out vec3 vert_color;
out vec3 Normal;
out vec2 TexCoord;
out vec3 WorldPos;
out vec4 ShadowPos;

void main()
{
	WorldPos = vec3(model * vec4(position,1));
    gl_Position = camera_projection * camera_view * model * vec4(position,1);
	Normal = normal_model * vert_normal;
	TexCoord = vertex_tex_coords;
	ShadowPos = light_projection * light_view * model * vec4(position,1);

}