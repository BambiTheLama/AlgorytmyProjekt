#version 330 core

out vec4 FragColor;
uniform vec4 modelColor;
in vec4 color;

void main()
{
	FragColor=vec4(color*modelColor);
}