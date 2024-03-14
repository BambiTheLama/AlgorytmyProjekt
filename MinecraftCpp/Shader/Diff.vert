#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexture;

uniform sampler2D tex0;
uniform vec2 textSize;
uniform mat4 camera;
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
	gl_Position = camera * model * vec4(vPos, 1.0f);
	textPos = vTexture/textSize;
	data_out.texCoord = vTexture;
}