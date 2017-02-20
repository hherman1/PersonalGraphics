#version 330 core

in vec3 vert_color;
out vec4 color;

uniform float green;

void main()
{
    color = vec4(vert_color.rgg, 1.0f);
} 