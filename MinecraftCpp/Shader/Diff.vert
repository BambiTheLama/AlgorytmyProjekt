#version 330 core

layout (location = 0) in int data;
layout (location = 1) in vec3 pos;
layout (location = 2) in vec2 textPos;

uniform vec4 modelColor;
uniform mat4 camera;
uniform mat4 model;
uniform mat4 lightProjection;
uniform int dir;
uniform float time;

struct getData{
	vec3 pos;
	int textID;
	bool cutY;
	bool cutSides;
	bool animatedUp;
	bool animatedDown;
	int scaleF;
	int scaleS;
} d;

out DATA
{
	vec2 texCoord;
	vec3 currentPos;
	vec4 fragPosLight;
	flat int textID;
	float bright;
} data_out;

vec2 getTextPos(vec2 tPos)
{
	if(dir<=1)
	{
		tPos.x *= d.scaleS;
		tPos.y *= d.scaleF;
	}
	else if(dir <= 3)
	{
		tPos.x *= d.scaleS;
		tPos.y *= d.scaleF;
	}
	else if(dir <= 5)
	{
		tPos.x *= d.scaleS;
		tPos.y *= d.scaleF;
	}

	return tPos;
}

vec3 scalePos(vec3 toRet)
{
	if(dir<=1)
	{
		toRet.z *= d.scaleS;
		toRet.y *= d.scaleF;
	}
	else if(dir <= 3)
	{
		toRet.x *= d.scaleS;
		toRet.y *= d.scaleF;
	}
	else if(dir <= 5)
	{
		toRet.x *= d.scaleF;
		toRet.z *= d.scaleS;
	}
	else
	{
		toRet.y *= d.scaleF;
	}
	return toRet;
}

vec3 getPos(vec3 vPos)
{
	vPos = scalePos(vPos);
	
	if(d.cutY)
	{
		if(vPos.y >= 0.5)
			vPos.y -= 0.1f;
	}
	if(d.cutSides)
	{
		if(dir==0)
			vPos.x-=0.08;
		else if(dir==1)
			vPos.x+=0.08;
		else if(dir==2)
			vPos.z+=0.08;
		else if(dir==3)
			vPos.z-=0.08;

	}
	if(d.animatedUp)
	{
		if(vPos.y>0.5f)
		{
			vPos.z+=sin(time)/12.0f;
			vPos.x+=sin(time)/12.0f;
		}

	}
	if(d.animatedDown)
	{
		if(vPos.y<=0.5f)
		{
			vPos.z-=sin(time)/12.0f;
			vPos.x-=sin(time)/12.0f;
		}

	}
	return vPos;
}

void main()
{
	d.pos.x  = data       & 15;				/// 0b000000000000000000000000000001111
	d.pos.y  = data >> 4  & 255;			/// 0b000000000000000000000111111110000
	d.pos.z  = data >> 12 & 15;				/// 0b000000000000000011110000000000000
	d.textID = data >> 16 & 63;				/// 0b000000000011111100000000000000000
	d.cutY = (data >> 22) == 1;				/// 0b000000000100000000000000000000000
	d.cutSides = (data >> 23) == 1;			/// 0b000000001000000000000000000000000
	d.animatedUp = (data >> 24) == 1;		/// 0b000000010000000000000000000000000
	d.animatedDown = (data >> 25) == 1;		/// 0b000000100000000000000000000000000
	d.scaleF = ((data >> 26) & 7) + 1;		/// 0b000111000000000000000000000000000
	d.scaleS = ((data >> 29) & 7) + 1;		/// 0b111000000000000000000000000000000

	vec3 currentPos = vec3(model * vec4(getPos(pos)+d.pos, 1.0f));
	gl_Position = camera * vec4(currentPos, 1.0f);
	data_out.texCoord = getTextPos(textPos);
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
	data_out.bright = 1.0;

}