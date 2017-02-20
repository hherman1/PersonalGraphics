#version 330 core


in mat4 lightMat;
in mat4 cameraMat;

in vec3 vert_color;
in vec2 TexCoord;
out vec4 color;

uniform float green;
uniform sampler2D Texture0;
uniform sampler2D Texture1;

void main()
{
	vec2 TexCoordFixed = vec2(TexCoord.x,1 - TexCoord.y);
    color = mix(texture(Texture0,TexCoordFixed.xy),texture(Texture1,TexCoordFixed.xy),TexCoord.x);
} 