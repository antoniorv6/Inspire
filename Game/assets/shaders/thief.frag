#version 440 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform vec3 _average; 

// = vec3(0.2126, 0.7152, 0.0722);

void main()
{
    FragColor = texture(screenTexture, TexCoords);
    float average = _average.x * FragColor.r + _average.y * FragColor.g +  _average.z * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);
} 