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