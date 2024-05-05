#version 330 core

out vec4 fragColor;

in vec3 texPos;

uniform samplerCube tex0;

void main()
{    
    fragColor = texture(tex0, texPos);
    //fragColor = vec4(1,0,0,1);
}