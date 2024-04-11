#version 330 core

layout (location = 0) in int data;
layout (location = 1) in vec3 pos;
layout (location = 2) in vec2 textPos;

uniform vec4 modelColor;
uniform mat4 camera;
uniform mat4 model;
uniform mat4 lightProjection;
uniform int dir;

struct getData{
	vec3 pos;
	vec2 text;
	int textID;
};

out DATA
{
	vec2 texCoord;
	vec3 currentPos;
	vec4 fragPosLight;
	flat int textID;
	float bright;
} data_out;



void main()
{
	getData d;
	d.pos.x  = data       & 15;			/// 0b000000000000000000000000000001111
	d.pos.y  = data >> 4  & 255;		/// 0b000000000000000000000111111110000
	d.pos.z  = data >> 12 & 15;			/// 0b000000000000000011110000000000000
	d.textID = data >> 16 & 95;			/// 0b000000000011111100000000000000000

	vec3 vPos=d.pos+pos;


	vec3 currentPos = vec3(model * vec4(vPos, 1.0f));
	gl_Position = camera * vec4(currentPos, 1.0f);
	data_out.texCoord = textPos;
	data_out.currentPos = currentPos;
	data_out.fragPosLight = lightProjection * vec4(currentPos,1.0f);

	data_out.textID = d.textID;

	if(dir<=1)
		data_out.bright = 0.82;
	else if(dir<=3)
		data_out.bright = 0.68;
	else if(dir<=4)
		data_out.bright = 1.0;
	else		
		data_out.bright = 0.5;

}