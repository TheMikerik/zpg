#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));

    Normal = mat3(modelMatrix) * normalize(aPos);

    gl_Position = projectionMatrix * viewMatrix * vec4(FragPos, 1.0);
}
