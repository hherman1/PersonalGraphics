#version 330 core

in vec3 TexCoords; // Direction vector representing a 3D texture coordinate
uniform samplerCube cubemap;  // Cubemap texture sampler
out vec4 color;

void main()
{             
    color = texture(cubemap, TexCoords);
}  