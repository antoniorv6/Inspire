#version 440 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float _Samples     = 6;
// How strong the blur is.
uniform float _EffectAmount;
// Radius of the Blur.
const float _Radius      = 2;
// Blur center, on screen space coordinates. 
const vec2 _blurCenter   = vec2(0.5, 0.5);
// To calculate the center of screen. 
const vec2 _resolution   = vec2(1280 / 720);

void main()
{
    vec4 color        = vec4(0,0,0,0);
    // Position of the current fragment. 
    vec2 fragPosition = (TexCoords.xy / _resolution.xy);
    // Direction vector from blurCenter to fragPosition. 
    vec2 dist         = fragPosition - _blurCenter;

    for(int i = 0; i < _Samples; i++)
    {
        /**
            How much the fragment would be offset.

        */
        float offset = 1 - _EffectAmount * (i  / _Samples) * (clamp(length(dist) / _Radius, 0.0, 1.0));
        color += texture(screenTexture, dist * offset + _blurCenter);
    }
    // To hget the average color.
    color /= _Samples;
    FragColor = color;
} 