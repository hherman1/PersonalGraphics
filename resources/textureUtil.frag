#version 330 core

in vec3 vert_color;
in vec2 TexCoord;
out vec4 color;

uniform sampler2D Texture0;

void main()
{
	vec2 TexCoordFixed = vec2(TexCoord.x,1 - TexCoord.y);
    color = texture(Texture0,TexCoordFixed.xy);
} 