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
	int blockDir;

	vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

} frag;
uniform vec3 camPos;
uniform vec3 shadowMapLightDir;
uniform vec3 lightColor;
uniform bool isDebug;


vec3 getLightValue()
{
    ///AMBIENT
    float ambient = 0.1;

    ///DIFFUSE
    float diffuse = 0.5;

	vec3 normal = normalize(texture(texN, frag.texCoord).rgb * 2.0f - 1.0f);
	if(frag.blockDir == 0)
		normal = normal * 1.0f;
	else if(frag.blockDir == 1)	
		normal = normal * -1.0f;
	else if(frag.blockDir == 2)
		normal = cross(normal,vec3(0.0f, 1.0f, 0.0f));
	else if(frag.blockDir == 3)
		normal = cross(normal,vec3(0.0f, -1.0f, 0.0f));
	else if(frag.blockDir == 4)
		normal = cross(normal,vec3(1.0f, 0.0f, 0.0f));
	else if(frag.blockDir == 5)
		normal = cross(normal,vec3(-1.0f, 0.0f, 0.0f));

    float diff = max(dot(normal, normalize(frag.lightV)), 0.0);

    vec3 textureColor=texture(tex0, frag.texCoord).rgb;

	///SPECULAR
	float specular = 0.5;

	vec3 viewDir = normalize(frag.cameraV - frag.currentPos);
	vec3 reflectDir = reflect(-lightDir, normal);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);


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
		float bias = max(0.5f * (1.0f - dot(normal, shadowMapLightDir)), 0.005f);
	
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
	shadow = 0.0f;

	return (normal+1.0f)/2.0f;

    vec3 ambientColor = ambient * textureColor;
    vec3 diffuseColor = diff *  textureColor * pow(1.0f - shadow,3);
	vec3 specularColor = specular * spec * lightColor * pow(1.0f - shadow,3); 
    return ambientColor + diffuseColor + diffuseColor;
}

void main()
{


	if (texture(tex0, frag.texCoord).a < 0.1)
		discard;

	FragColor = vec4(getLightValue(),texture(tex0, frag.texCoord).a) * modelColor;
}