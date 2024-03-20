#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexture;

uniform sampler2D tex0;
uniform vec2 textSize;
uniform vec4 modelColor;
uniform mat4 camera;
uniform mat4 model;
uniform bool debug;

out vec2 textPos;

out DATA
{
	vec2 texCoord;
	vec3 currentPos;
} data_out;



void main()
{
	vec3 currentPos = vec3(model * vec4(vPos, 1.0f));
	gl_Position = camera * vec4(currentPos, 1.0f);
	textPos = vTexture / textSize;
	data_out.texCoord = textPos;
	data_out.currentPos = currentPos;
}