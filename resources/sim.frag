#version 330 core

in vec3 vertColor;
out vec4 color;

void main()
{
    //dens = vertColor;
    //uv = vertColor;

    color = vec4(vertColor,1);
} 