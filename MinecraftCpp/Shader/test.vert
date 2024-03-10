#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexture;

uniform sampler2D tex0;
uniform mat4 camera;
uniform mat4 model;
uniform vec4 modelColor;
out vec2 textPos;

void main()
{
	vec3 point = vec3(model * vec4(vPos, 1.0f));
	gl_Position = camera * vec4(point, 1.0f);
	textPos=vTexture;
}