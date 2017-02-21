#version 330 core


in mat4 lightMat;
in mat4 cameraMat;

in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoord;
out vec4 color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;  

uniform vec3 view_pos;
uniform sampler2D Texture0;

void main()
{
    vec3 ambient = material.ambient * light.ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - WorldPos); 
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff*material.diffuse);

	vec3 viewDir = normalize(view_pos - WorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec*material.specular);  

	vec2 TexCoordFixed = vec2(TexCoord.x,1 - TexCoord.y);
	vec3 result = diffuse + ambient  + specular;
    color = vec4(result,1);
} 