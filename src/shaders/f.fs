#version 330 core
out vec4 FragColor;  

//in vec3 ourColor;
in vec2 TexCoord;
in float zbuf;

uniform sampler2D texture1;

void main()
{
    FragColor = zbuf*vec4(-0.0015f, -0.0015f, -0.0005f, 1.0f)+texture(texture1, TexCoord);
}
