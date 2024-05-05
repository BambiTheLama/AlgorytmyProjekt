#version 330 core

layout (location = 0) in vec3 vPos;

out vec3 texPos;
uniform mat4 camera;
uniform vec3 cameraPos;

void main()
{
    vec4 pos = camera * vec4(vPos+cameraPos, 1.0f);

    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    texPos = vec3(vPos.x, vPos.y, -vPos.z);
}   