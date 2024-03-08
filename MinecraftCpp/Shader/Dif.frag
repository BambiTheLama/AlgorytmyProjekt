#version 330 core

out vec4 FragColor;
uniform vec4 modelColor;
uniform sampler2D tex0;
uniform bool isTexture;
in vec4 color;
in vec2 textPos;

void main()
{
	if(isTexture)
		FragColor = texture(tex0, textPos) * modelColor;
	else
		FragColor = vec4(modelColor);
}