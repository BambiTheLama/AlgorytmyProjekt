#version 330 core
out vec4 FragColor;

in vec2 texPos;

uniform sampler2D text0;
uniform vec3 backGroundColor;
float offsetX=1.0f/1600.0f;
float offsetY=1.0f/900.0f;


void main()
{
    //FragColor = vec4(texture(text0, texPos).brga);
    vec4 colorV=vec4(0);

    float kernel[9] = float[](
//       1.0f/16.0f,2.0f/16.0f,1.0f/16.0f,
//       2.0f/16.0f,2.0f/16.0f,2.0f/16.0f,
//       1.0f/16.0f,2.0f/16.0f,1.0f/16.0f
//         0,-1,0,
//         -1,4,-1,
//         0,-1,0
           -1,-1,1,
           -1,1,1,
           -1,1,1
    );

    for(int x=0;x<3;x++)
    {
        for(int y=0;y<3;y++)
        {
            colorV+=texture(text0, vec2(texPos.x-offsetX+offsetX*x,texPos.y-offsetY+offsetY*y))*kernel[y*3+x];
        }
    }



    FragColor=vec4(colorV.rgb,1.0f);
    FragColor=texture(text0, texPos);
    vec3 dif=FragColor.rgb-backGroundColor.rgb;
    if(dif.x >= -0.1 && dif.x <= 0.1 && dif.y >= -0.1 && dif.y <= 0.1 && dif.z >= -0.1 && dif.z <= 0.1)
        FragColor.a=0.0f;

} 