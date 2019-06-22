#version 440 core

in vec2 TexCoords;
out vec4 finalColor;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    vec4 sampled = vec4(1.0,1.0,1.0, texture(text, TexCoords).r);
    finalColor = vec4(vec3(0.0,0.0,0.0), 1.0) * sampled;
    if(finalColor.a < 0.7)
        discard;
}