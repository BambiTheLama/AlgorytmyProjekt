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
out mat4 out_Model;
//out vec2 v_TexCoord;

void main()
{
   //gl_Position = u_MVP * u_Trans * (position + u_Position);

    FragPos = vec3(u_Model * position);
    out_Model = u_Model;
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
in mat4 out_Model;
//in vec2 v_TexCoord;

uniform vec3 u_Color;
uniform vec3 u_LightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D u_Texture;

void main()
{
    //vec4 texColor = texture(u_Texture, v_TexCoord);
    //color = u_Color;

    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * u_LightColor;

    // diffuse

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos /* - FragPos*/);

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

    vec3 finalColor = vec3(0.0, 0.0, 1.0);

    vec3 result = (ambient + diffuse) * finalColor;
    color = vec4(result, 0.5);
}