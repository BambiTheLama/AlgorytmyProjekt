#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexture;

uniform sampler2D tex0;
uniform mat4 camera;
uniform vec3 pos;
uniform vec4 modelColor;
uniform mat4 model;
out vec2 textPos;
uniform bool debug;

out DATA
{
	vec2 texCoord;
} data_out;

void main()
{
	gl_Position = camera * vec4(vPos + pos, 1.0f);
	textPos = vTexture;
	data_out.texCoord = vTexture;
}