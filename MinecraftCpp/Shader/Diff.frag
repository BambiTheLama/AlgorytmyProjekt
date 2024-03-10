#version 330 core

out vec4 FragColor;
uniform vec4 modelColor;
uniform sampler2D tex0;
in vec2 texCoord;
uniform bool debug;


void main()
{
    if(debug)
		FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	else
		FragColor = texture(tex0, texCoord) * modelColor;

}