#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 camera;
uniform vec3 cubePos;

void main()
{
	gl_Position = camera * vec4(pos + cubePos, 1.0f);
}