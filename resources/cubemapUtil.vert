#version 330 core
layout (location = 0) in vec3 position;
out vec3 TexCoords;

uniform mat4 camera_view;
uniform mat4 camera_projection;


void main()
{
    gl_Position =   camera_projection * camera_view * vec4(position, 1.0);  
    TexCoords = position;
}  