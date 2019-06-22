#version 440

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normals;

out vec4 Col;
out vec2 vTextureCoord;
out vec3 vaNormal;
out vec3 vFragPos;
out int  vHasOutline;

uniform mat4 model;
uniform mat4 MVP;
uniform int HasOutline;

void main()
{
    gl_Position = MVP * vec4(pos, 1.0);
    vFragPos = vec3(model * vec4(pos, 1.0));
    vTextureCoord = texCoords;
    vHasOutline = HasOutline;
    vaNormal = mat3(transpose(inverse(model))) * normals;  
}