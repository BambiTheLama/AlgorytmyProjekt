#version 330 core

layout (location = 0) in int data;

uniform sampler2D tex0;
uniform vec2 textSize;
uniform vec4 modelColor;
uniform mat4 camera;
uniform mat4 model;
uniform bool debug;

out vec2 textPos;

struct getData{
	vec3 pos;
	vec2 text;
};

out DATA
{
	vec2 texCoord;
	vec3 currentPos;
} data_out;



void main()
{
	getData d;
	d.pos.x  = data       & 31;	/// 0b00000000000000000011111
	d.pos.y  = data >> 5  & 31;	/// 0b00000000000001111100000
	d.pos.z  = data >> 10 & 31;	/// 0b00000000111110000000000
	d.text.x = data >> 15 & 15;	/// 0b00001111000000000000000
	d.text.y = data >> 19 & 15;	/// 0b11110000000000000000000

	vec3 currentPos = vec3(model * vec4(d.pos, 1.0f));
	gl_Position = camera * vec4(currentPos, 1.0f);
	textPos = d.text / textSize;
	data_out.texCoord = d.text;
	data_out.currentPos = currentPos;
}