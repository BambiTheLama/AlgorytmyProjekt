#version 330 core

layout (location = 0) in int data;

uniform sampler2D tex0;
uniform vec2 textSize;
uniform vec4 modelColor;
uniform mat4 camera;
uniform mat4 model;

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
	d.pos.x  = int(data)       & 15;	/// 0b0000000000000000000001111
	d.pos.y  = int(data) >> 4  & 255;	/// 0b0000000000000111111110000
	d.pos.z  = int(data) >> 12 & 15;	/// 0b0000000011110000000000000
	d.text.x = int(data) >> 16 & 15;	/// 0b0000111100000000000000000
	d.text.y = int(data) >> 20 & 15;	/// 0b1111000000000000000000000

	vec3 currentPos = vec3(model * vec4(d.pos, 1.0f));
	gl_Position = camera * vec4(currentPos, 1.0f);
	textPos = d.text / textSize;
	data_out.texCoord = d.text/ textSize;
	data_out.currentPos = currentPos;
}