#version 330 core
out vec4 FragColor;

in vec2 texPos;

uniform sampler2D text0;

void main()
{
    FragColor = vec4(texture(text0, texPos).brga);
} 