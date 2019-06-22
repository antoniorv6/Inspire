#version 440 core
out vec4 FragColor;
in vec2 TextureCoord;

uniform sampler2D tex;
uniform vec4 _color;

void main()
{
    vec4 texture = texture2D(tex, TextureCoord);
    texture.r = texture.r + _color.x;
    texture.g = texture.g + _color.y;
    texture.b = texture.b + _color.z;
    texture.a = texture.a + _color.w;
    FragColor = texture;
}