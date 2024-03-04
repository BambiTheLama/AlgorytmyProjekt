#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 aNormal;
//layout(location = 2) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 FragPos;
out vec3 Normal;
//out vec2 v_TexCoord;

void main()
{
   //gl_Position = u_MVP * u_Trans * (position + u_Position);

    FragPos = vec3(u_Model * position);
    //Normal = aNormal;

    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    gl_Position =  u_Projection * u_View * u_Model * position;
   
   //v_TexCoord = texCoord;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 FragPos;
in vec3 Normal;
//in vec2 v_TexCoord;

uniform vec3 u_Color;
uniform vec3 u_LightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D u_Texture;

void main()
{
    //vec4 texColor = texture(u_Texture, v_TexCoord);

    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * u_LightColor;

    // diffuse

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos  - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // end diffuse

    // specular

    float specularStrength = 1.5;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = 0.0;
    if (diff > 0.4) {
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    }
    
    vec3 specular = specularStrength * spec * u_LightColor;

    // end specular

    float height = FragPos.y; // Wysokoœæ piksela w przestrzeni ekranu (znormalizowana do 0-1)

    vec3 finalColor = vec3(0.0);
    vec3 waterColor = vec3(0.0, 0.0, 0.8); // Kolor wody - niebieski
    vec3 sandColor = vec3(1.0, 1.0, 0.0); // Kolor piasku - ¿ó³ty
    vec3 grassColor = vec3(0.4, 1.0, 0.4); // Kolor trawy - zielony
    vec3 snowColor = vec3(1.0, 1.0, 1.0); // Kolor œniegu - bia³y

    // Generowanie koloru za pomoc¹ p³ynnego gradientu
    if (height < 0.15) {
        finalColor = waterColor;
    }
    else if (height < 0.50)
        finalColor = mix(sandColor, grassColor, smoothstep(0.35, 0.45, height));
    else if (height < 2.20)
        finalColor = mix(grassColor, snowColor, smoothstep(0.5, 2.20, height));
    else if (height < 2.80)
        finalColor = mix(snowColor, vec3(0.8), smoothstep(2.20, 2.80, height));
    else
        finalColor = vec3(0.8);

    vec3 result = (ambient + diffuse) * finalColor;
    color = vec4(result, 1.0);
}