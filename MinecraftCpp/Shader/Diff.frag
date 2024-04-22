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

in DATA
{
	vec2 texCoord;
	vec3 currentPos;
	vec4 fragPosLight;
	flat int textID;
	float bright;
	bool isText2;
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
	if(frag.isText2)
	{
		normalText = texture(texN2[frag.textID], frag.texCoord).rgb;
		heightText = texture(texH2[frag.textID], frag.texCoord).rgb;
		albedoText = texture(tex02[frag.textID], frag.texCoord).rgb;
	}

	// ambient lighting
	float ambient = 0.50f;

	// diffuse lighting
	vec3 normal = getNormal(normalize(normalText * 2.0f - 1.0f));

	//vec3 normal = vec3(1);
	vec3 lightDirection = lightDir;
	float diffuse = min(max(dot(normal, lightDirection), 0.0f),0.3f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - frag.currentPos);
	vec3 reflectionDirection = reflect(lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;


	// Shadow value
	float shadow = 0.0f;
	// Sets lightCoords to cull space
	vec3 lightCoords = frag.fragPosLight.xyz / frag.fragPosLight.w;
	if(lightCoords.z <= 1.0f)
	{
		// Get from [-1, 1] range to [0, 1] range just like the shadow map
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		float currentDepth = lightCoords.z;
		// Prevents shadow acne
		float bias = max(0.5f * (1.0f - dot(normal, lightDirection)), 0.01f);
	
		// Smoothens out the shadows
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
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);
	
	}
	//shadow=0.0f;
	//return vec3(texture(texN[frag.textID], frag.texCoord));
	//return (normal+1.0f)/2.0f;
	vec3 diffuseColor = albedoText * diffuse * (1.0f - shadow) * lightColor;
	vec3 specularColor = heightText.r * specular * (1.0f - shadow) * lightColor*0.0001f;
	vec3 ambientColor = albedoText * ambient;
	if(shadow == 1)
		return albedoText *  ambient;
	return ambientColor + specularColor + diffuseColor;
}

void main()
{
	if(!debug)
	{
		if (texture(tex0[frag.textID], frag.texCoord).a < 0.1)
		discard;

		FragColor = vec4(directLight()*frag.bright,texture(tex0[frag.textID], frag.texCoord).a) * modelColor;
	}
	else
	{
		FragColor = vec4(1.0f);
	}


}