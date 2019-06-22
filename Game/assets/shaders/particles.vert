// #version 440 core
// layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

// out vec2 TexCoords;
// out vec4 ParticleColor;

// uniform mat4 projection;
// uniform vec2 offset;
// uniform vec4 color;

// void main()
// {
//     float scale = 10.0f;
//     TexCoords = vertex.zw;
//     ParticleColor = color;
//     gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
// }

#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoords;

out vec2 TextureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TextureCoord = texCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
} 