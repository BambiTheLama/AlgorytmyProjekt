#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texturePos;

out vec2 texPos;

void main()
{
    texPos = texturePos;
    gl_Position = vec4(pos.xy, 0.0, 1.0); 
}  