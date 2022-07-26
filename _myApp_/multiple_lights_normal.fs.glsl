#version 430 core

in vec3 vsNormal;  
in vec3 vsPos;  
in vec2 vsTexCoord; // UV
in mat3 vsTBN;
in vec3 vsT;
in vec3 vsB;
in vec3 vsN;
vec3 LightDirection_tangentspace;
in vec3 EyeDirection_tangentspace;
in vec3 EyeDirection_cameraspace;

struct Material { 
    sampler2D diffuse; 
    sampler2D specular;
    sampler2D normalMap;

    float shininess; 

	vec3 defaultColor;
	vec3 defaultSpecular;
	int useDiffuseMap;
	int useSpecularMap;
    int useNormalMap;
}; 

struct DirLight { 
    vec3 direction; 

    vec3 ambient; 
    vec3 diffuse; 
    vec3 specular; 
}; 

struct PointLight {
    vec3 position;
    
    float c1;
    float c2;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float c1;
    float c2;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NUM_POINT_LIGHTS 1

uniform Material material;
uniform vec3 viewPos; 
uniform DirLight dirLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform int useNormal;
uniform mat4 view;

out vec4 fragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 matAmbientColor;
vec3 matDiffuseColor;
vec3 matSpecularColor;

vec3 TextureNormal_tangentspace;

void main()
{
	if(useNormal != 0)
	{
		// properties
		vec3 norm = normalize(vsNormal);
        vec3 viewDir = normalize(viewPos - vsPos);
		// vec3 viewDir = normalize(viewPos - vsPos);

		matAmbientColor = material.defaultColor; // MaterialDiffuseColor 
		matDiffuseColor = material.defaultColor; // MaterialAmbientColor 
		matSpecularColor = material.defaultSpecular; // MaterialSpecularColor 

        if(material.useNormalMap != 0) 
        {
            // Local normal, in tangent space. V tex coordinate is inverted because normal map is in TGA (not in DDS) for better quality
            norm = (texture(material.normalMap, vec2(vsTexCoord.x, -vsTexCoord.y)).rgb*2.0 - 1.0);
            // norm = norm.x * vsN + norm.y * vsT + norm.z * vsB;
            norm = normalize(norm);
            
            // vec3 normalMap = vec3(texture(material.normalMap, vsTexCoord).rgb);
            // normalMap = (normalMap * 2.0) - 1.0;

            // norm = vsN + normalMap.x * vsT + normalMap.y * vsB;
            // norm = normalize(norm);
        }

		if(material.useDiffuseMap != 0)
		{
			matAmbientColor = vec3(texture(material.diffuse, vsTexCoord));
			matDiffuseColor = vec3(texture(material.diffuse, vsTexCoord));
		}

		if(material.useSpecularMap != 0)
		{
			matSpecularColor = vec3(texture(material.specular, vsTexCoord));
		}

		// phase 1: directional lighting
		vec3 result = CalcDirLight(dirLight, norm, viewDir);
		// phase 2: point lights
		for(int i = 0; i < NUM_POINT_LIGHTS; i++) 
		{
			result += CalcPointLight(pointLights[i], norm, vsPos, viewDir);    
		}
		// phase 3: spot light
		result += CalcSpotLight(spotLight, norm, vsPos, viewDir);    
    
		fragColor = vec4(result, 1.0);
	}
	else
		fragColor = vec4(material.defaultColor, 1.0);
} 

// directional light
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient = light.ambient * matAmbientColor;
    vec3 diffuse = light.diffuse * diff * matDiffuseColor;
    vec3 specular = light.specular * spec * matSpecularColor;
    
	return (ambient + diffuse + specular);
}

// point light
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);

    // attenuation
    float attenuation = 1.0 / (1.0 + light.c1 * distance + light.c2 * (distance * distance));    

    // combine results
    vec3 ambient = light.ambient * matAmbientColor;
    vec3 diffuse = light.diffuse * diff * matDiffuseColor;
    vec3 specular = light.specular * spec * matSpecularColor;

    return (ambient + diffuse + specular) * attenuation;
}

// spot light
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
	// diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
	// specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
	// Soft edge spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + light.c1 * distance + light.c2 * (distance * distance));
        
	// combine results
    vec3 ambient = light.ambient * matAmbientColor;
    vec3 diffuse = light.diffuse * diff * matDiffuseColor;
    vec3 specular = light.specular * spec * matSpecularColor;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}