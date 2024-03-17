#version 330 core

out vec4 FragColor;
uniform vec4 modelColor;
uniform sampler2D tex0;
uniform sampler2D texH;
uniform sampler2D texN;
in vec2 texCoord;
in vec3 camPos;
in vec3 currentPos;
uniform bool debug;
uniform vec3 lightColor;

vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(texture(texN, texCoord).xyz * 2.0f - 1.0f);
	//vec3 normal=vec3(1);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(tex0, texCoord) * (diffuse + ambient) + texture(texH, texCoord).r * specular) * vec4(lightColor,1.0f);
}

void main()
{
	if(debug)
	{
		FragColor = vec4(1.0f);
		return;
	}
	if (texture(tex0, texCoord).a < 0.1)
		discard;

	FragColor = direcLight() * modelColor;
	//FragColor = texture(tex0, texCoord) * modelColor;
}