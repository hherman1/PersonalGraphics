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
    color = mix(texture(Texture0,TexCoord.xy),texture(Texture1,TexCoord.xy),TexCoord.x);
} 