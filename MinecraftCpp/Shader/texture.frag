#version 330 core
out vec4 FragColor;

in vec2 texPos;

uniform sampler2D tex0;
uniform vec3 backgroundColor;
float offsetX = 1.0f / 1600.0f;
float offsetY = 1.0f / 900.0f;


void main()
{
    vec4 colorV = vec4(0);

    FragColor = texture(tex0, texPos);
    vec3 dif = FragColor.rgb-backgroundColor.rgb;
    if(dif.x >= -0.1 && dif.x <= 0.1 && dif.y >= -0.1 && dif.y <= 0.1 && dif.z >= -0.1 && dif.z <= 0.1)
        FragColor.a = 0.0f;
} 