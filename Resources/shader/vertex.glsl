#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 trans;

out vec2 texcoords;

void main(void)
{
    texcoords = aTexCoord;
    gl_Position = trans * vec4(aPos, 1.0);
}
