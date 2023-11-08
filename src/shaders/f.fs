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

uniform sampler2D texture1;

void main()
{
    //FragColor = vec4(ambientLightColor * objectColor, 1.0)*texture(texture1, TexCoord);//zbuf*vec4(-0.0015f, -0.0015f, -0.0005f, 1.0f)
    //FragColor = vec4(normal.z, 0, 0, 1.0);//FragColor*clamp(dot(vec3(-2,0,0),normal),0.7,2);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 final = (ambientLightColor+diffuse) * objectColor;
    FragColor = vec4(final, 1.0)*texture(texture1, TexCoord);
}
