#version 440 core

uniform sampler2D glColorMap;

in vec2  TextCoord;
out vec4 FragColor;

void main()
{
    FragColor = texture2D(glColorMap, TextCoord);

    if(FragColor.a < 0.7)
    {
        discard;
    }
}