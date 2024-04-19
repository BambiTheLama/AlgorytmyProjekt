#version 330 core

layout (location = 0) in int data;
layout (location = 1) in vec3 pos;

uniform mat4 camera;
uniform mat4 model;
uniform int dir;
uniform float time;

struct getData{
	vec3 pos;
	bool cutY;
	bool cutSides;
	bool animatedUp;
	bool animatedDown;
	int scaleF;
	int scaleS;
} d;

vec3 getPos(vec3 vPos)
{
	if(dir<=1)
	{
		vPos.z *= d.scaleS;
		vPos.y *= d.scaleF;
	}
	else if(dir <= 3)
	{
		vPos.x *= d.scaleS;
		vPos.y *= d.scaleF;
	}
	else if(dir <= 5)
	{
		vPos.x *= d.scaleF;
		vPos.z *= d.scaleS;
	}
	
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
		if(vPos.y<0.5f)
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
	d.cutY = (data >> 22) == 1;				/// 0b000000000100000000000000000000000
	d.cutSides = (data >> 23) == 1;			/// 0b000000001000000000000000000000000
	d.animatedUp = (data >> 24) == 1;		/// 0b000000010000000000000000000000000
	d.animatedDown = (data >> 25) == 1;		/// 0b000000100000000000000000000000000
	d.scaleF = ((data >> 26) & 7) + 1;		/// 0b000111000000000000000000000000000
	d.scaleS = ((data >> 29) & 7) + 1;		/// 0b111000000000000000000000000000000

	vec3 vPos=d.pos + getPos(pos);
	vec3 currentPos = vec3(model * vec4(vPos, 1.0f));
	gl_Position = camera * vec4(currentPos, 1.0f);
}