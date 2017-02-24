#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

struct Material {
    vec3 diffuse;
    float specular;
}; 
uniform Material material;
uniform sampler2D _texture;

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    gPosition = WorldPos;
    // Also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // And the diffuse per-fragment color
    gAlbedoSpec.rgb = material.diffuse * texture(_texture,TexCoords).rgb;
    // Store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = material.specular;
}  