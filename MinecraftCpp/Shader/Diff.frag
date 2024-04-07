#version 330 core

out vec4 FragColor;
uniform vec4 modelColor;
uniform sampler2D tex0;
uniform sampler2D texH;
uniform sampler2D texN;
uniform sampler2D texShadow;
uniform vec3 lightDir;

in GEO_OUT
{
    vec2 texCoord;
    vec3 pos;
    vec3 currentPos;
    vec3 lightV;
    vec3 cameraV;
    vec4 fragPosLight;
    float brightness;

	vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

} frag;
uniform vec3 camPos;

uniform vec3 lightColor;


vec4 directLight()
{
	// ambient lighting
	float ambient = 0.40f;

	// diffuse lighting
	vec3 normal = normalize((texture(texN, frag.texCoord).xyz * 2.0f - 1.0f));
	//vec3 normal = vec3(1);
	vec3 lightDirection = frag.TangentLightPos;
	float diffuse = min(max(dot(normal, lightDirection), 0.0f),0.3f);

	// specular lighting
	float specularLight = 0.30f;
	vec3 viewDirection = normalize(frag.TangentViewPos - frag.TangentFragPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
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
		float bias = max(0.05f * (1.0f - dot(normal, lightDirection)), 0.0001f);
	
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

	vec4 diffuseColor = texture(tex0, frag.texCoord) * diffuse * (1.0f - shadow) * vec4(lightColor,1.0f);
	vec4 specularColor = texture(texH, frag.texCoord).r * specular * (1.0f-shadow) * vec4(lightColor,1.0f);
	vec4 ambientColor = texture(tex0, frag.texCoord) *  ambient;

	if(shadow==1)
		return texture(tex0, frag.texCoord) *  ambient;
	return ambientColor + specularColor + diffuseColor;
}

void main()
{

	if (texture(tex0, frag.texCoord).a < 0.1)
		discard;

	FragColor = vec4(vec3(directLight()*frag.brightness),texture(tex0, frag.texCoord).a) * modelColor;
}