#version 330 core

out vec4 FragColor;
uniform vec4 modelColor;
uniform sampler2D tex0;
in vec2 texCoord;
uniform bool debug;


void main()
{
	if (texture(tex0, texCoord).a < 0.1)
		discard;
	FragColor = texture(tex0, texCoord) * modelColor;
}