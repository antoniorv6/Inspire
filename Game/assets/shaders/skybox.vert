#version 440 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    //We don't need any texture coordinates, because any position vector is also
    //a direction vector to sample the cube's texels
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    //An optimization to pass the Detph Testing
    gl_Position = pos.xyww;
}  