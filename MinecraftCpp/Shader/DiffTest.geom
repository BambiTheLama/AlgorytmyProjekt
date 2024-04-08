#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

out GEO_OUT
{
    vec2 texCoord;
    vec3 pos;
    vec3 currentPos;
    vec3 lightV;
    vec3 cameraV;
    vec4 fragPosLight;
    float brightness;


    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} geo;

uniform mat4 camera;
uniform mat4 model;
uniform vec3 camPos;
uniform vec3 lightPos;
uniform vec3 lightDir;

in DATA
{
	vec2 texCoord;
	vec3 currentPos;
	vec4 fragPosLight;
    float brightness;
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
    //TBN = transpose(TBN);


    gl_Position = gl_in[0].gl_Position;
    geo.texCoord = data_in[0].texCoord;
    geo.currentPos =  gl_in[0].gl_Position.xyz;
    geo.lightV =  normalize(lightDir);
    geo.cameraV =  camPos;
    geo.pos = gl_in[0].gl_Position.xyz;
    geo.fragPosLight = data_in[0].fragPosLight;
    geo.brightness = data_in[0].brightness;

    geo.TangentLightPos =  geo.lightV;
    geo.TangentViewPos  = TBN * camPos;
    geo.TangentFragPos  = TBN * data_in[0].currentPos;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    geo.texCoord = data_in[1].texCoord;
    geo.currentPos =  gl_in[1].gl_Position.xyz;
    geo.lightV =  normalize(lightDir);
    geo.cameraV =  camPos;
    geo.pos = gl_in[1].gl_Position.xyz;
    geo.fragPosLight = data_in[1].fragPosLight;
    geo.brightness = data_in[1].brightness;

    geo.TangentLightPos =  geo.lightV;
    geo.TangentViewPos  = TBN * camPos;
    geo.TangentFragPos  = TBN * data_in[1].currentPos;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    geo.texCoord = data_in[2].texCoord;
    geo.currentPos =  gl_in[2].gl_Position.xyz;
    geo.lightV =  normalize(lightDir);
    geo.cameraV =  camPos;
    geo.pos = gl_in[2].gl_Position.xyz;
    geo.fragPosLight = data_in[2].fragPosLight;
    geo.brightness = data_in[2].brightness;

    geo.TangentLightPos = geo.lightV;
    geo.TangentViewPos  = TBN * camPos;
    geo.TangentFragPos  = TBN * data_in[2].currentPos;
    EmitVertex();

    EndPrimitive();
}



void main()
{

    normalMode();
}