#version 330 core
out vec4 FragColor;  

//in vec3 ourColor;
in vec2 TexCoord;
in float zbuf;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = zbuf*vec4(0.0f, 0.0f, 0.002f, 1.0f)+mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
