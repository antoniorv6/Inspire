#version 440 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D blendingTexture;

uniform float _mirrorAlpha;
uniform float _greenPct;
uniform float _bluePct;

void main()
{
    vec4 screen = texture2D(screenTexture, TexCoords);
    screen.g = screen.g - _greenPct;
    screen.b = screen.b - _bluePct;

    vec4 mirror = texture2D(blendingTexture, TexCoords);
    vec3 rgb = mirror.rgb;

    if(length(rgb) == 0)
    {
        FragColor = screen;
    }
    else
    {
        mirror.a = _mirrorAlpha;
        FragColor = mix(screen, mirror, mirror.a);
    }
} 