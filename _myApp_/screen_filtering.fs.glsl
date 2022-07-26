#version 430 core

out vec4 fragColor;
  
in vec2 vsTexCoord; // FBO Texture

uniform sampler2D screenTexture;

float offsetX = 1.0f / float(textureSize(screenTexture,0).x); // (1/가로) 
float offsetY = 1.0f / float(textureSize(screenTexture,0).y); // (1/세로)
float kernel[9];

vec2 offsets[9] = vec2[]( // 이동량 픽셀위치
    vec2(-offsetX,  offsetY), // top-left
    vec2( 0.0f,    offsetY), // top-center
    vec2( offsetX,  offsetY), // top-right
    vec2(-offsetX,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offsetX,  0.0f),   // center-right
    vec2(-offsetX, -offsetY), // bottom-left
    vec2( 0.0f,   -offsetY), // bottom-center
    vec2( offsetX, -offsetY)  // bottom-right    
);
vec3 sampleTex[9];
vec3 color = vec3(0.0);

// 0 = 아주 단순한 텍스처링 쉐이더 (51:49)
// 1 = 샤픈 필터링
// 2 = 엣지 검출 필터링
// 3 = 블러 필터링
uniform int uMode;
int mode = 0;

void main()
{
    switch (uMode) {
        // 아주 단순한 텍스처링 쉐이더 (51:49)
        case 0:
            fragColor = texture(screenTexture, vsTexCoord);
            break;
        // 샤픈 필터링
        case 1:
            kernel = float[](
                -1.f, -1.f, -1.f,
                -1.f,  9.f, -1.f,
                -1.f, -1.f, -1.f
            );
            
            for(int i = 0; i < 9; i++)
            {
                sampleTex[i] = vec3(texture(screenTexture, 
                                            vsTexCoord.st + offsets[i]
                                            ));
            }

            for(int i = 0; i < 9; i++)
                color += sampleTex[i] * kernel[i];
            
            fragColor = vec4(color, 1.0);
            break;
        // 엣지 검출 필터링
        case 2:
            kernel = float[](
                1.f,    1.f,   1.f,
                1.f,    -8.f,  1.f,
                1.f,    1.f,   1.f
            );
            
            for(int i = 0; i < 9; i++)
            {
                sampleTex[i] = vec3(texture(screenTexture, 
                                            vsTexCoord.st + offsets[i]
                                            ));
            }

            for(int i = 0; i < 9; i++)
                color += sampleTex[i] * kernel[i];
            
            fragColor = vec4(color, 1.0);
            break;
        // 블러 필터링
        case 3:
            kernel = float[](
                1.f,    2.f,   1.f,
                2.f,    4.f,  2.f,
                1.f,    2.f,   1.f
            );

            for(int i = 0; i < 9; i++) {
                kernel[i] = (kernel[i] / 16.f);
            }
            
            for(int i = 0; i < 9; i++)
            {
                sampleTex[i] = vec3(texture(screenTexture, 
                                            vsTexCoord.st + offsets[i]
                                            ));
            }

            for(int i = 0; i < 9; i++)
                color += sampleTex[i] * kernel[i];
            
            fragColor = vec4(color, 1.0);
            break;
    }

}  