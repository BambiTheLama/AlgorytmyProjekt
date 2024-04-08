#version 330 core

layout (location = 0) in int data;

uniform sampler2D tex0;
uniform vec2 textSize;
uniform vec4 modelColor;
uniform mat4 camera;
uniform mat4 model;
uniform mat4 lightProjection;

struct getData{
	vec3 pos;
	vec2 text;
	int brightness;
};

out DATA
{
	vec2 texCoord;
	vec3 currentPos;
	vec4 fragPosLight;
	float brightness;
} data_out;



void main()
{
	getData d;
	d.pos.x  = data       & 15;			/// 0b000000000000000000000000000001111
	d.pos.y  = data >> 4  & 255;		/// 0b000000000000000000000111111110000
	d.pos.z  = data >> 12 & 15;			/// 0b000000000000000011110000000000000
	d.text.x = data >> 16 & 15;			/// 0b000000000000111100000000000000000
	d.text.y = data >> 20 & 15;			/// 0b000000001111000000000000000000000
	d.brightness = data >> 24 & 7;		/// 0b000001110000000000000000000000000


		
	vec3 currentPos = vec3(model * vec4(d.pos, 1.0f));
	gl_Position = camera * vec4(currentPos, 1.0f);
	data_out.texCoord = d.text/ textSize;
	data_out.currentPos = currentPos;
	data_out.fragPosLight = lightProjection * vec4(currentPos,1.0f);
	if(d.brightness==4)
		data_out.brightness = 1;
	else if(d.brightness == 3 && d.brightness == 2)
		data_out.brightness = 0.9;
	else if(d.brightness == 1 && d.brightness == 0)
		data_out.brightness = 0.8;
	else
		data_out.brightness = 0.6;
}