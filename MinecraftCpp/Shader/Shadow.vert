#version 330 core

layout (location = 0) in int data;

uniform mat4 camera;
uniform mat4 model;

struct getData{
	vec3 pos;
	vec2 text;
};


void main()
{
	getData d;
	d.pos.x  = int(data)       & 15;	/// 0b00000000000001111
	d.pos.y  = int(data) >> 4  & 255;	/// 0b00000111111110000
	d.pos.z  = int(data) >> 12 & 15;	/// 0b11110000000000000

	vec3 currentPos = vec3(model * vec4(d.pos, 1.0f));
	gl_Position = camera * vec4(currentPos, 1.0f);
}