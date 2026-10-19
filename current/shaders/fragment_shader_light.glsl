#version 330 core

in vec3 FragPos;
in vec3 Normal;
out vec4 frag_colour;

uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 lightColor;

const float AMBIENT_STRENGTH = 0.1;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 result = (AMBIENT_STRENGTH + diff) * lightColor * objectColor;

    frag_colour = vec4(result, 1.0);
}
