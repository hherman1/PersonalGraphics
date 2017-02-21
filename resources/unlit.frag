#version 330 core


in mat4 lightMat;
in mat4 cameraMat;

in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoord;
out vec4 color;

uniform vec3 object_color;

uniform sampler2D Texture0;

void main()
{
    color = vec4(object_color,1);
} 