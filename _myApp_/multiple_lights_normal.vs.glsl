#version 430 core

layout (location = 0) in vec3 pos; // vertexPosition_modelspace
layout (location = 1) in vec2 texCoord; // vertexUV
layout (location = 2) in vec3 normal; // vertexNormal_modelspace
layout (location = 3) in vec3 vertexTangent_modelspace;
layout (location = 4) in vec3 vertexBitangent_modelspace;

out vec3 vsPos; // Position_worldspace 
out vec3 vsNormal;
out vec2 vsTexCoord; // UV
out mat3 vsTBN;
out vec3 vsT;
out vec3 vsB;
out vec3 vsN;

out vec3 EyeDirection_cameraspace;
out vec3 EyeDirection_tangentspace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 LightPosition_worldspace;
uniform vec3 viewPos; 

mat4 ModelViewMatrix;
mat3 MV3x3;

void main()
{
    // Position of the vertex, in worldspace : M * position (Position_worldspace)
    vsPos = vec3(model * vec4(pos, 1.0));

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( view * model * vec4(pos,1)).xyz;
	// EyeDirection_cameraspace = viewPos - vertexPosition_cameraspace;
    EyeDirection_cameraspace = viewPos - vsPos;

    // UV of the vertex. No special space for this one.
    vsTexCoord = texCoord;

    // model to camera = ModelView
    ModelViewMatrix = view * model;
    MV3x3 = mat3(ModelViewMatrix);

    vec3 vertexTangent_cameraspace = MV3x3 * vertexTangent_modelspace;
    vec3 vertexBitangent_cameraspace = MV3x3 * vertexBitangent_modelspace;
    vec3 vertexNormal_cameraspace = vec3(model * vec4(normal, 0.0)); 

    // vsT = normalize(vec3(model * vec4(vertexTangent_modelspace, 0.0)));
    // vsB = normalize(vec3(model * vec4(vertexBitangent_modelspace, 0.0)));
    // vsN = normalize(vec3(model * vec4(normal, 0.0))); // 문제있음
    vsT = vertexTangent_cameraspace;
    vsB = vertexBitangent_cameraspace;
    vsN = vertexNormal_cameraspace;


    // vsNormal = mat3(transpose(inverse(model))) * normal;  
    // vsNormal = vertexNormal_cameraspace;
    vsNormal = mat3(transpose(inverse(model))) * normal;  

    
    // You can use dot products instead of building this matrix and transposing it. See References for details.
	mat3 vsTBN = transpose(mat3(
		vertexTangent_cameraspace,
		vertexBitangent_cameraspace,
		vertexNormal_cameraspace	
	));

	EyeDirection_tangentspace =  vsTBN * EyeDirection_cameraspace;

    // Output position of the vertex, in clip space : MVP * position
    gl_Position = projection * view * vec4(vsPos, 1.0);
}