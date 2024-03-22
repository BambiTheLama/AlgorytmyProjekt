#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec2 texCoord;
out vec3 currentPos;
out vec3 lightV;
out vec3 cameraV;
uniform bool debug;
uniform mat4 camera;
uniform mat4 model;
uniform vec3 camPos;
uniform vec3 lightPos;

in DATA
{
	vec2 texCoord;
    vec3 currentPos;
} data_in[];


void normalMode()
{

    // Edges of the triangle
    vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    // Lengths of UV differences
    vec2 deltaUV0 = data_in[1].texCoord - data_in[0].texCoord;
    vec2 deltaUV1 = data_in[2].texCoord - data_in[0].texCoord;

    // one over the determinant
    float invDet = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

    vec3 tangent = vec3(invDet * (deltaUV1.y * edge0 - deltaUV0.y * edge1));
    vec3 bitangent = vec3(invDet * (-deltaUV1.x * edge0 + deltaUV0.x * edge1));

    vec3 T = normalize(vec3(model * vec4(tangent, 0.0f)));
    vec3 B = normalize(vec3(model * vec4(bitangent, 0.0f)));
    vec3 N = normalize(vec3(model * vec4(cross(edge1, edge0), 0.0f)));

    mat3 TBN = mat3(T, B, N);
    // TBN is an orthogonal matrix and so its inverse is equal to its transpose
    TBN = transpose(TBN);


    gl_Position = gl_in[0].gl_Position;
    texCoord = data_in[0].texCoord;
    currentPos = TBN * gl_in[0].gl_Position.xyz;
    lightV = TBN * lightPos;
    cameraV = TBN * camPos;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    texCoord = data_in[1].texCoord;
    currentPos = TBN * gl_in[1].gl_Position.xyz;
    lightV = TBN * lightPos;
    cameraV = TBN * camPos;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    texCoord = data_in[2].texCoord;
    currentPos = TBN * gl_in[2].gl_Position.xyz;
    lightV = TBN * lightPos;
    cameraV = TBN * camPos;
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

    gl_Position =  p2;
    EmitVertex();

    gl_Position =  p3;
    EmitVertex();
     
    gl_Position =  (p1/v + p2 + p3)/2;
    EmitVertex();

    EndPrimitive();

    gl_Position =  p3;
    EmitVertex();

    gl_Position = p1;
    EmitVertex();

    gl_Position =  (p1 + p2/v + p3)/2;
    EmitVertex();

    EndPrimitive();


}

void main()
{

    //if(debug)
    //    debugMode();
    //else
        normalMode();
}