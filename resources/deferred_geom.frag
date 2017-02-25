#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 NormalRaw;
in mat3 NormalModel;

struct Material {
    vec3 diffuse;
    float specular;
}; 
uniform Material material;
uniform sampler2D _texture;
uniform sampler2D _texture_normal;
uniform sampler2D _texture_displacement; 

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
	float displacement = length(texture(_texture_displacement,TexCoords));
    gPosition = WorldPos;// + displacement * NormalModel * NormalRaw;
    // Also store the per-fragment normals into the gbuffer
	vec3 texNormal = normalize(2*texture(_texture_normal,TexCoords).rgb-1.0);
    gNormal = NormalModel * normalize(NormalRaw + texNormal);
    // And the diffuse per-fragment color
    gAlbedoSpec.rgb = material.diffuse * texture(_texture,TexCoords).rgb;
    // Store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = material.specular;
}  