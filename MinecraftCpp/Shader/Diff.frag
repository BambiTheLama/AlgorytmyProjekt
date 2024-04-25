#version 330 core

out vec4 FragColor;
uniform vec4 modelColor;


uniform sampler2D texN[32];
uniform sampler2D texH[32];
uniform sampler2D tex0[32];
uniform sampler2D texN2[32];
uniform sampler2D texH2[32];
uniform sampler2D tex02[32];

uniform sampler2D texShadow;
uniform vec3 lightDir;
uniform int dir;
uniform bool debug;
uniform bool normalsMode;

in DATA
{
	vec2 texCoord;
	vec3 currentPos;
	vec4 fragPosLight;
	flat int textID;
	float bright;
	flat int isText2;
} frag;

uniform vec3 camPos;
uniform vec3 lightColor;

vec3 getNormal(vec3 normal)
{
	if(dir == 3)
		return normal;
	else if(dir == 2)	
		return normal * -1.0f;
	else if(dir == 1)
		return cross(normal,vec3(0.0f, 1.0f, 0.0f));
	else if(dir == 0)
		return cross(normal,vec3(0.0f, -1.0f, 0.0f));
	else if(dir == 4)
		return cross(normal,vec3(1.0f, 0.0f, 0.0f));
	else if(dir == 5)
		return cross(normal,vec3(-1.0f, 0.0f, 0.0f));
	else if(dir == 6)
		return cross(normal,vec3(0.0f, -1.0f, -1.0f));
	else if(dir == 7)
		return cross(normal,vec3(0.0f, 1.0f, 1.0f));
	else if(dir == 8)
		return cross(normal,vec3(0.0f, 1.0f, -1.0f));
	else if(dir == 9)
		return cross(normal,vec3(0.0f, -1.0f, 1.0f));
	return normal;
}

vec3 directLight()
{
	vec3 normalText = texture(texN[frag.textID], frag.texCoord).rgb;
	vec3 heightText = texture(texH[frag.textID], frag.texCoord).rgb;
	vec3 albedoText = texture(tex0[frag.textID], frag.texCoord).rgb;
	if(frag.isText2 == 1)
	{
		normalText = texture(texN2[frag.textID], frag.texCoord).rgb;
		heightText = texture(texH2[frag.textID], frag.texCoord).rgb;
		albedoText = texture(tex02[frag.textID], frag.texCoord).rgb;
	}

	// ambient lighting
	float ambient = 0.50f;

	// diffuse lighting
	vec3 normal = getNormal(normalize(normalText * 2.0f - 1.0f));

	vec3 lightDirection = lightDir;
	float diffuse = min(max(dot(normal, lightDirection), 0.0f),0.3f);

	// specular lighting
	float specularLight = 0.0075f;
	vec3 viewDirection = normalize(camPos - frag.currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;


	float shadow = 0.0f;
	vec3 lightCoords = frag.fragPosLight.xyz / frag.fragPosLight.w;
	if(lightCoords.z <= 1.0f)
	{
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		float currentDepth = lightCoords.z;
		float bias = max(0.5f * (1.0f - dot(normal, lightDirection)), 0.01f);
	
		int sampleRadius = 3;
		vec2 pixelSize = 1.0 / textureSize(texShadow, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(texShadow, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
		shadow /= pow((sampleRadius * 2 + 1), 2);
	
	}


	vec3 diffuseColor = albedoText * diffuse * (1.0f - shadow) * lightColor;
	vec3 specularColor = heightText.r * specular * (1.0f - shadow) * lightColor;
	vec3 ambientColor = albedoText * ambient;



	if(shadow == 1)
		return albedoText *  ambient;
	return ambientColor + specularColor + diffuseColor;
}

void main()
{
	if(!debug)
	{
		float text = texture(tex0[frag.textID], frag.texCoord).a;
		if(frag.isText2 == 1)
		{
			text = texture(tex02[frag.textID], frag.texCoord).a;
		}
		if (text < 0.1)
			discard;


		FragColor = vec4(directLight()*frag.bright,text) * modelColor;
	}
	else
	{
		FragColor = vec4(1.0f);
	}

}