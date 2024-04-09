#version 330 core

layout (location = 0) in int data;

uniform sampler2D tex0;
uniform vec4 modelColor;
uniform mat4 camera;
uniform mat4 model;
uniform mat4 lightProjection;

struct getData{
	vec3 pos;
	vec2 text;
	int textID;
	int blockDir;
};

out DATA
{
	vec2 texCoord;
	vec3 currentPos;
	vec4 fragPosLight;
	flat int textID;
	flat int dir;
	float bright;
} data_out;



void main()
{
	getData d;
	d.pos.x  = data       & 15;			/// 0b000000000000000000000000000001111
	d.pos.y  = data >> 4  & 255;		/// 0b000000000000000000000111111110000
	d.pos.z  = data >> 12 & 15;			/// 0b000000000000000011110000000000000
	d.text.x = data >> 16 & 1;			/// 0b000000000000000100000000000000000
	d.text.y = data >> 17 & 1;			/// 0b000000000000001000000000000000000
	d.textID = data >> 18 & 31;			/// 0b000000000111110000000000000000000
	d.blockDir = data >> 23 & 7;		/// 0b000000111000000000000000000000000

		
	vec3 currentPos = vec3(model * vec4(d.pos, 1.0f));
	gl_Position = camera * vec4(currentPos, 1.0f);
	data_out.texCoord = d.text;
	data_out.currentPos = currentPos;
	data_out.fragPosLight = lightProjection * vec4(currentPos,1.0f);

	data_out.dir = d.blockDir;
	data_out.textID = d.textID;

	if(d.blockDir<=1)
		data_out.bright = 0.82;
	else if(d.blockDir<=3)
		data_out.bright = 0.68;
	else if(d.blockDir<=4)
		data_out.bright = 1.0;
	else		
		data_out.bright = 0.5;

}