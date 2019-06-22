#version 440 core

in vec2 TextureCoords;
out vec4 FragColor;

uniform sampler2D textureIMG;

void main()
{
    FragColor = texture(textureIMG, TextureCoords);

    //if(FragColor.r < 0.05 && FragColor.g < 0.05 && FragColor.b < 0.05)
    //{
    //    discard;
    //}
}