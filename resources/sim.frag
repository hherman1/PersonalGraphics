#version 330 core

in vec3 vertColor;
out vec4 color;


void main()
{
    color = vec4(vertColor,1);
    //color = vec4(1);
} 