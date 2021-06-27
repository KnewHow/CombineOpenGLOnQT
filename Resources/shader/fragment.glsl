#version 330 core

out vec4 frag_Color;

in vec2 texcoords;

uniform sampler2D texture_color1;
uniform sampler2D texture_color2;

void main(void)
{
    frag_Color = mix(texture(texture_color1, texcoords), texture(texture_color2, texcoords), 0.4);
    //frag_Color = texture(texture_color1, texcoords);
    //frag_Color = vec4(1.0, 0.0, 0.0,1.0);
}

