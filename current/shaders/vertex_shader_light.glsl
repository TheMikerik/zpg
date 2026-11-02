#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aAttr;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform int normalFromAttr;      // 1 = normala je v aAttr, 0 = normala je normalize(aPos) (koule se stredem v pocatku)
uniform int wrongNormalMatrix;  

out vec3 FragPos;
out vec3 Normal;
out vec3 vertexColor;

void main()
{
    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));

    vec3 normal = normalFromAttr == 1 ? aAttr : normalize(aPos);
    if (wrongNormalMatrix == 1) {
        Normal = mat3(modelMatrix) * normal;
    } else {
        Normal = mat3(transpose(inverse(modelMatrix))) * normal;
    }

    vertexColor = aAttr;

    gl_Position = projectionMatrix * viewMatrix * vec4(FragPos, 1.0);
}
