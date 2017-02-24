#version 330 core

in vec2 TexCoord;
out vec4 color;

struct Material {
    vec3 diffuse;
    float specular;
}; 
  
struct Light {
    vec3 position;
	vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;

	float farPlane;

	float cutOff;
};

uniform Light light;  

uniform vec3 camera_view_pos;
//uniform sampler2D shadowMap;
//uniform sampler2D _texture;
uniform samplerCube shadowCubemap;


struct Deferred {
	sampler2D WorldPos;
	sampler2D Normal;
	sampler2D Materials;
};


uniform Deferred deferred;

vec3 calcLight(vec3 WorldPos, vec3 Normal, Material material) {
    vec3 ambient = material.diffuse * light.ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - WorldPos); 
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff*material.diffuse);

	vec3 viewDir = normalize(camera_view_pos - WorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 32 should be shininess
	vec3 specular = light.specular * (spec*material.specular); 
	
	float distance    = length(light.position - WorldPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance)); 

	vec2 TexCoordFixed = vec2(TexCoord.x,1 - TexCoord.y);

	float theta = dot(lightDir, normalize(-light.direction));
	if(theta > light.cutOff) 
	{    
		return (diffuse + ambient  + specular) * attenuation;// * texture(Texture0,TexCoord);
	}
	else {
	  return light.ambient;// * texture(Texture0,TexCoord);
	}
}

float calcShadows(vec3 WorldPos) {
	float shadow = 1.0;
	float bias = 0.005;
	//vec3 shadowTexPos = ShadowPos.xyz/ShadowPos.w;
	//shadowTexPos = shadowTexPos*0.5 + 0.5;
	//float closestDepth = texture(shadowMap,shadowTexPos.xy).r;

	vec3 lightToWorld = WorldPos - light.position;
	float closestDepth = texture(shadowCubemap,lightToWorld).r;
	closestDepth *= light.farPlane;

	if( length(lightToWorld) - bias > closestDepth) {
		shadow = 0.1;
	}
	if( closestDepth == light.farPlane) {
		shadow = 1;
	}
	//if(shadowTexPos.z > 1) {
	//	shadow = 1;
	//}
	return shadow;
}

void main()
{
	vec3 WorldPos = texture(deferred.WorldPos,TexCoord).rgb;
	vec3 Normal = texture(deferred.Normal,TexCoord).rgb;
	Material mat;
	mat.diffuse = texture(deferred.Materials,TexCoord).rgb;
	mat.specular = texture(deferred.Materials,TexCoord).a;

	if(Normal == vec3(0,0,0)) {
		discard;
	}

	float shadow = calcShadows(WorldPos);
	color = shadow * vec4(calcLight(WorldPos,Normal,mat),1);
	//color = shadow * vec4(calcLight(),1) * texture(_texture,TexCoord);
} 