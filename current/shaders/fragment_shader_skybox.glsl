#version 330 core

in vec3 direction;
out vec4 frag_colour;

uniform samplerCube skybox;

void main()
{
    frag_colour = texture(skybox, direction);
}
