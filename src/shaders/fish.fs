#version 330 core
out vec4 FragColor;  

//in vec3 ourColor;
in vec2 TexCoord;
in float zbuf;
in vec3 normal;
in vec3 fragPos;

uniform vec3 objectColor;
uniform vec3 ambientLightColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightDir;

uniform sampler2D texture1;
uniform sampler2D causticTex;

void main()
{
    //FragColor = vec4(ambientLightColor * objectColor, 1.0)*texture(texture1, TexCoord);//zbuf*vec4(-0.0015f, -0.0015f, -0.0005f, 1.0f)
    //FragColor = vec4(normal.z, 0, 0, 1.0);//FragColor*clamp(dot(vec3(-2,0,0),normal),0.7,2);
    vec3 lightDir = normalize(lightDir);
    vec3 up = normalize(vec3(0.9, 0.9, 0.1));
    vec3 up_causticscoord = normalize(up - lightDir*(dot(up, lightDir)));
    vec3 right_causticscoord = cross(up_causticscoord, lightDir);
    vec2 causticTexCoord = vec2(dot(fragPos, up_causticscoord), dot(fragPos, right_causticscoord));
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 final = (ambientLightColor+diffuse) * objectColor;
    float caustic_scale = 0.1;
    final = vec3(clamp(final.r-zbuf*0.001f,0.0f,1.0f),clamp(final.g-zbuf*0.0005f,0.0f,1.0f),clamp(final.b-zbuf*0.0003f,0.0f,1.0f)); //fog
    FragColor = vec4(final, 1.0)*texture(texture1, TexCoord)+0.4*vec4(diffuse,1.0)*texture(causticTex, caustic_scale*causticTexCoord);
}
