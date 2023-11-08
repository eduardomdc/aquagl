#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // vertex normal
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out float zbuf;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //ourColor = aColor; // set ourColor to the input color we got from the vertex data
    TexCoord = aTexCoord;
    zbuf = gl_Position[2];
    normal = aNormal;
    fragPos = vec3(model*vec4(aPos, 1.0));
}       
