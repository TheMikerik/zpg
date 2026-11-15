#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 direction;

void main()
{
    direction = aPos;
    // bez posunutí kamery, aby obloha byla stále stejně daleko
    gl_Position = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(aPos, 1.0);
}
