#version 330 core
layout (location = 0) in vec2 vPos;
layout (location = 1) in vec2 vTexturePos;

uniform vec2 pos;

out vec2 texPos;

void main()
{
    texPos = vTexturePos;
    gl_Position = vec4(vPos.xy+pos.xy, 0.0, 1.0); 
}  