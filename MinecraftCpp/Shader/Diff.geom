#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 Normal;
out vec3 color;
out vec2 texCoord;
uniform bool debug;

in DATA
{
	vec2 texCoord;
} data_in[];


void normalMode()
{
    gl_Position = gl_in[0].gl_Position;
    texCoord = data_in[0].texCoord;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    texCoord = data_in[1].texCoord;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    texCoord = data_in[2].texCoord;
    EmitVertex();

    EndPrimitive();
}

void debugMode()
{
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;
    vec4 p3 = gl_in[2].gl_Position;
    float v = 36.0f;
    gl_Position =  p1;
    EmitVertex();

    gl_Position =  p2;
    EmitVertex();

    gl_Position =  (p1 + p2 + p3/v)/2;
    EmitVertex();

    EndPrimitive();

    gl_Position =  p3;
    EmitVertex();

    gl_Position =  p2;
    EmitVertex();
     
    gl_Position =  (p1/v + p2 + p3)/2;
    EmitVertex();

    EndPrimitive();

    gl_Position =  p1;
    EmitVertex();

    gl_Position = p3;
    EmitVertex();

    gl_Position =  (p1 + p2/v + p3)/2;
    EmitVertex();

    EndPrimitive();


}

void main()
{

    if(debug)
        debugMode();
    else
        normalMode();
}