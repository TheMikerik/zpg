#version 330 core

in vec3 vertexColor;
in vec3 normal;
out vec4 frag_colour;

void main()
{
    float shade = 0.3 + 0.7 * abs(normal.y);
    frag_colour = vec4(shade * vertexColor, 1.0);
}
