#version 430 core							
			
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoord; // x,y in NDC

out vec2 vsTexCoord;

void main()
{
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0); 
    vsTexCoord = texCoord;
}  