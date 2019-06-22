#version 440 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float _deltaTime;
uniform float _amplitude;
uniform float _refraction;
uniform float _width;

const vec2 position    = vec2(1280 / 2, 720 / 2);
const vec2 resolution  = vec2(1280, 720); 


void main()
{
  vec2 uv = TexCoords;
  vec2 texCoord = uv;
  vec2 center = position / resolution;
  float distance = distance(uv, center);

  if ((distance <= (_deltaTime + _width)) && (distance >= (_deltaTime - _width))) 
  {
    float diff = (distance - _deltaTime);
    float powDiff = 1.0 - pow(abs(diff * _amplitude), _refraction);
    float diffTime = diff  * powDiff;
    vec2 diffUV = normalize(uv - center);
    texCoord = uv + (diffUV * diffTime);
  }

  FragColor = texture2D(screenTexture, texCoord);
} 