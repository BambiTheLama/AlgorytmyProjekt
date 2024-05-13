#version 330 core
#extension GL_ARB_bindless_texture: require

out vec4 FragColor;
uniform vec4 modelColor;

layout (bindless_sampler) uniform sampler2D texN[64];
layout (bindless_sampler) uniform sampler2D texH[64];
layout (bindless_sampler) uniform sampler2D tex0[64];

uniform sampler2D waterTex1;
uniform sampler2D waterTex0;
uniform sampler2D waterTexN;
uniform sampler2D texShadow;
uniform sampler2D texRefraction;

uniform vec3 lightDir;
uniform int dir;
uniform bool debug;
uniform bool normalsMode;
uniform float time;
uniform bool hasRefrectTexture;
uniform bool hideBelow;
uniform float hightToHide;
in DATA
{
	vec2 texCoord;
	vec3 currentPos;
	vec4 fragPosLight;
	vec4 fragPosRefraction;
	flat int textID;
	float bright;
	flat bool underWater;
	flat bool isRefract;
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

vec3 getNormalColor()
{
	return texture(texN[frag.textID], frag.texCoord).rgb / 2+1.0f/4.0f;
}

vec3 directLight()
{
	vec3 normalTex = getNormalColor();
	vec3 heightTex = texture(texH[frag.textID], frag.texCoord).rgb;
	vec3 albedoTex = texture(tex0[frag.textID], frag.texCoord).rgb;


	// ambient lighting
	float ambient = 0.30f;

	// diffuse lighting
	vec3 normal = normalize(getNormal(normalize(normalTex * 2.0f - 1.0f)));

	vec3 lightDirection = lightDir;
	float diffuse = min(max(dot(normal, lightDirection), 0.0f),0.5f);

	// specular lighting
	float specularLight = 0.2f;
	vec3 viewDirection = normalize(camPos - frag.currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 4);
	float specular = specAmount * specularLight;

	float shadow = 0.0f;
	vec3 lightCoords = frag.fragPosLight.xyz / frag.fragPosLight.w;
	if(lightCoords.z <= 1.0f)
	{
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		float currentDepth = lightCoords.z;
		float bias = max(0.5f * (1.0f - dot(normal, lightDirection)), 0.005f);
	
		int sampleRadius = 2;
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

	vec3 diffuseColor = albedoTex * diffuse * (1.0f - shadow) * lightColor;
	vec3 specularColor =  heightTex.r * specular * (1.0f - shadow) * albedoTex * lightColor;
	vec3 ambientColor = albedoTex * ambient;

	if(hasRefrectTexture && frag.isRefract)
	{
		vec3 refractCoords = frag.fragPosRefraction.xyz / frag.fragPosRefraction.w;

		if(refractCoords.z <= 1.0f)
		{
			refractCoords = (refractCoords + 1.0f) / 2.0f;
			vec2 textureCors=frag.texCoord+(time/16-int(time/16));
			vec2 distortion = texture(waterTexN, textureCors).xy*2-1;
			distortion*=0.01f;
			vec3 refractColor = texture(texRefraction, vec2(refractCoords.x,-refractCoords.y)+distortion).xyz/2;

			diffuseColor += refractColor;

		}


	}
	else if(frag.underWater)
	{
		float t = (time/16-int(time/16));
		vec3 waterColor=texture(waterTex0, frag.texCoord+t).rgb/3;
		vec3 waterWhiteSpotsColor=texture(waterTex1, frag.texCoord+t).rgb;
		vec3 waterRedSpotsColor = vec3(texture(waterTex1, frag.texCoord+t+0.02).r,0,0);
		vec3 waterGreenSpotsColor = vec3(0,texture(waterTex1, frag.texCoord+t+0.04).g,0);
		vec3 waterBlueSpotsColor = vec3(0,0,texture(waterTex1, frag.texCoord+t+0.06).b);
		ambientColor += waterWhiteSpotsColor+waterRedSpotsColor+waterGreenSpotsColor+waterBlueSpotsColor;
		specularColor*=waterColor;
		diffuseColor*=waterColor;
	}

	if(shadow == 1)
		return ambientColor;
	
	return ambientColor + specularColor + diffuseColor;
}

void main()
{
	if(hideBelow && frag.currentPos.y< hightToHide)
		discard;
	float text = texture(texN[frag.textID], frag.texCoord).a;
	if (text < 0.1)
		discard;
	if(debug)
	{
		vec3 normalTex = (getNormal(getNormalColor())+1)/2;
		FragColor = vec4(normalTex,text);
	}
	else
		FragColor = vec4(min(directLight()*frag.bright,1.0f),text);
}