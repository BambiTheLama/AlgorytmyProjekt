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

    gl_Position = gl_in[0].gl_Position;
    geo.texCoord = data_in[0].texCoord;
    geo.currentPos =  gl_in[0].gl_Position.xyz;
    geo.lightV =  normalize(lightDir);
    geo.cameraV =  camPos;
    geo.pos = gl_in[0].gl_Position.xyz;
    geo.fragPosLight = data_in[0].fragPosLight;
    geo.brightness = data_in[0].brightness;

    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    geo.texCoord = data_in[1].texCoord;
    geo.currentPos =  gl_in[1].gl_Position.xyz;
    geo.lightV =  normalize(lightDir);
    geo.cameraV =  camPos;
    geo.pos = gl_in[1].gl_Position.xyz;
    geo.fragPosLight = data_in[1].fragPosLight;
    geo.brightness = data_in[1].brightness;

    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    geo.texCoord = data_in[2].texCoord;
    geo.currentPos =  gl_in[2].gl_Position.xyz;
    geo.lightV =  normalize(lightDir);
    geo.cameraV =  camPos;
    geo.pos = gl_in[2].gl_Position.xyz;
    geo.fragPosLight = data_in[2].fragPosLight;
    geo.brightness = data_in[2].brightness;

    EmitVertex();

    EndPrimitive();
}



void main()
{

    normalMode();
}