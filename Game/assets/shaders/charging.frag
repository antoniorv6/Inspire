#version 440 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float _expand;
uniform vec2 _resolution;

const vec2 _center = vec2(1280 / 2, 720 / 2);


void main()
{   
    vec2 uv = gl_FragCoord.xy / _resolution.xy;
    vec2 center = vec2(0.5);
    float d = distance(uv,center);

     vec4 texture = texture2D(screenTexture, TexCoords);
    texture.rgba = mix(texture.rgba, vec4(0.96078, 0.89803, 0.1058, 1.0), d - _expand);
    
    FragColor = texture;
}  