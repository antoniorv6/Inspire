#version 440 core
layout (location = 0) in vec4 vertex;

out vec2 TextureCoords;

uniform mat4 ModelMat;
uniform mat4 ProjectionMat;

void main()
{
    TextureCoords = vertex.zw;
    gl_Position = ProjectionMat*ModelMat*vec4(vertex.xy, 0.0, 1.0);
}