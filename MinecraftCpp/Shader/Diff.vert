#version 330 core


layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texPos;
layout (location = 2) in uvec2 data;

uniform vec4 modelColor;
uniform mat4 camera;
uniform mat4 model;
uniform mat4 lightProjection;
uniform int dir;
uniform float time;
uniform int chunkX;
uniform int chunkZ;

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

vec2 getTexPos(vec2 tPos)
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
	else if(dir == 4)
	{
		tPos.x *= d.scaleS;
		tPos.y *= d.scaleF;
	}
	else if(dir <= 5)
	{
		tPos.x *= d.scaleF;
		tPos.y *= d.scaleS;
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
			vPos.x -= 0.08;
		else if(dir==1)
			vPos.x += 0.08;
		else if(dir==2)
			vPos.z += 0.08;
		else if(dir==3)
			vPos.z -= 0.08;
	}
	if(d.animatedUp)
	{
		if(vPos.y>0.5f)
		{
			vPos.z += sin(time)/12.0f;
			vPos.x += sin(time)/12.0f;
		}
	}
	if(d.animatedDown)
	{
		if(vPos.y<=0.5f)
		{
			vPos.z -= sin(time)/12.0f;
			vPos.x -= sin(time)/12.0f;
		}
	}
	return vPos;
}

void main()
{
	int tmpData = int(data.x);
	d.pos.x  = (tmpData)        & 31;					/// 0b00000000000000000000000000011111
	d.pos.y  = (tmpData) >> 5   & 255;					/// 0b00000000000000000001111111100000
	d.pos.z  = (tmpData) >> 13  & 31;					/// 0b00000000000000111110000000000000
	d.textID = (tmpData) >> 18  & 63;					/// 0b00000000111111000000000000000000


	tmpData = int(data.y);
	d.cutY = (tmpData & 1) == 1;						/// 0b00000000000000000000000000000001
	d.cutSides = ((tmpData >> 1) & 1) == 1;				/// 0b00000000000000000000000000000010
	d.animatedUp = ((tmpData >> 2) & 1) == 1;			/// 0b00000000000000000000000000000100
	d.animatedDown = ((tmpData >> 3) & 1) == 1;			/// 0b00000000000000000000000000001000
	d.scaleF = ((tmpData >> 21) & 63) + 1;				/// 0b00000111111000000000000000000000
	d.scaleS = ((tmpData >> 27) & 31) + 1;				/// 0b11111000000000000000000000000000

	vec3 currentPos = getPos(pos) + d.pos + vec3(chunkX,0,chunkZ);
	gl_Position = camera * vec4(currentPos, 1.0f);
	data_out.texCoord = getTexPos(texPos);
	data_out.currentPos = currentPos;
	data_out.fragPosLight = lightProjection * vec4(currentPos,1.0f);

	data_out.textID = d.textID;
	if(dir<=1)
		data_out.bright = 0.82;
	else if(dir<=3)
		data_out.bright = 0.68;
	else if(dir<=4)
		data_out.bright = 1.0;
	else if(dir<=5)
		data_out.bright = 0.5;
	else 
		data_out.bright = 1;
}