#version 440 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D blendingTexture;

uniform float _shieldAlpha;
uniform float _redPct;
uniform float _bluePct;

void main()
{
    vec4 screen = texture2D(screenTexture, TexCoords);
    screen.r = screen.r - _redPct;
    screen.b = screen.b - _bluePct;

    vec4 shield = texture2D(blendingTexture, TexCoords);
    vec3 rgb = shield.rgb;

    if(length(rgb) == 0)
    {
        FragColor = screen;
    }
    else
    {
        shield.a = _shieldAlpha;
        FragColor = mix(screen, shield, shield.a);
    }
} 