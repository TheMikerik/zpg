#version 330 core

in vec3 FragPos;
in vec3 Normal;
out vec4 frag_colour;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    vec3 result = lightColor * objectColor;

    frag_colour = vec4(result, 1.0);
}
