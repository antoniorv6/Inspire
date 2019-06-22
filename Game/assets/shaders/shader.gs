#version 440 core

layout (triangles_adjacency) in;
layout (triangle_strip, max_vertices = 18) out;

in vec2 vTextureCoord[];
in vec3 vaNormal[];
in vec3 vFragPos[];
in int  vHasOutline[];

flat out float IsEdge;

out vec2 TextureCoord;
out vec3 aNormal;
out vec3 FragPos;

float EdgeWidth = 0.004;
float PctExtend = 0.00001;

bool isFrontFacing(vec3 a, vec3 b, vec3 c)
{
    float dotResult = (a.x* b.y - b.x * a.y) + (b.x * c.y - c.x * b.y) + (c.x * a.y - a.x * c.y); 
    
    if (dotResult > 0)
    {
        return true;
    }
    return false;
}

void emitEdgeQuad(vec3 e0, vec3 e1)
{
    vec2 ext = PctExtend * (e1.xy - e0.xy);
    vec2 v = normalize(e1.xy - e0.xy);
    vec2 n = vec2(-v.y, v.x) * EdgeWidth;

    if(e0.x <= 1.0 && e0.x >= -1.0 && e1.x <= 1.0 && e1.x >= -1.0 && e0.y <= 1.0 && e0.y >= -1.0 && e1.y <= 1.0 && e1.y >= -1.0)
    {
        IsEdge = 1.0;
        gl_Position = vec4(e0.xy - ext, e0.z, 1.0);
        EmitVertex();
        gl_Position = vec4(e0.xy - n - ext, e0.z, 1.0);
        EmitVertex();
        gl_Position = vec4(e1.xy + ext, e1.z, 1.0);
        EmitVertex();
        gl_Position = vec4(e1.xy - n + ext, e1.z, 1.0);
        EmitVertex();
        EndPrimitive();
    }
}

void main()
{
    if(vHasOutline[0] == 1)
    {
        vec3 p0 = gl_in[0].gl_Position.xyz / gl_in[0].gl_Position.w;
        vec3 p1 = gl_in[1].gl_Position.xyz / gl_in[1].gl_Position.w;
        vec3 p2 = gl_in[2].gl_Position.xyz / gl_in[2].gl_Position.w;
        vec3 p3 = gl_in[3].gl_Position.xyz / gl_in[3].gl_Position.w;
        vec3 p4 = gl_in[4].gl_Position.xyz / gl_in[4].gl_Position.w;
        vec3 p5 = gl_in[5].gl_Position.xyz / gl_in[5].gl_Position.w;    

        float clip0 = gl_in[0].gl_ClipDistance[0];

        if(isFrontFacing(p0, p2, p4))
        {
            if(!isFrontFacing(p0, p1, p2))
                emitEdgeQuad(p0,p2);
            if(!isFrontFacing(p2, p3, p4))
                emitEdgeQuad(p2, p4);
            if(!isFrontFacing(p4, p5, p0))
                emitEdgeQuad(p4, p0);
        }
    }

    IsEdge = 0.0;
    TextureCoord = vTextureCoord[0];
    aNormal = vaNormal[0];
    FragPos = vFragPos[0];
    gl_Position  = gl_in[0].gl_Position;
    EmitVertex();

    TextureCoord = vTextureCoord[2];
    aNormal = vaNormal[2];
    FragPos = vFragPos[2];
    gl_Position  = gl_in[2].gl_Position;
    EmitVertex();

    TextureCoord = vTextureCoord[4];
    aNormal = vaNormal[4];
    FragPos = vFragPos[4];
    gl_Position  = gl_in[4].gl_Position;
    EmitVertex();
    
    EndPrimitive();

}
