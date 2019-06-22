#version 440 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float offsetDenom;  
uniform float mixPct;

float offset = 1.0 / offsetDenom;

vec2 offsets[9] = vec2[]
(
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right  
);

float kernel[9] = float[]
(
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);

void main()
{
    vec4 texture = texture2D(screenTexture, TexCoords);
    vec3 color   = vec3(0.0);
    vec3 sampleTex[9];

    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture2D(screenTexture, TexCoords.st + offsets[i]));
    }

    for(int i = 0; i < 9; i++)
    {
        color += sampleTex[i] * kernel[i];
    }
        
    vec3 negative = vec3(1 - color);
    vec3 rgb      = texture.rgb;
    texture.rgb   = mix(texture.rgb, negative, clamp(mixPct, 0, 1));

    FragColor = texture;
} 