#version 330 core

out vec4 frag_Color;

in vec3 normal;
in vec3 fragPos;

uniform vec3 viewerPos;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main(void)
{
    vec3 N = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewerDir = normalize(viewerPos - fragPos);
    vec3 halfVec = normalize(lightDir + viewerDir);

    vec3 ambient = 0.1 * lightColor;
    vec3 diffuse = max(dot(N, lightDir), 0.0) * lightColor;
    vec3 specular = 0.5 * pow(max(dot(N, halfVec), 0.0), 32) * lightColor;
    vec3 r = (ambient + diffuse + specular) * objectColor;
    // r = vec3(1.0, 0.0, 0.0);
    frag_Color = vec4(r, 1.0);
}

