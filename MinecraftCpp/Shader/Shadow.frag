#version 330 core

out vec4 fragColor;

void main()
{
	fragColor = vec4(gl_FragCoord.zzz,1.0f);
}