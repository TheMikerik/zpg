#version 330 core

in vec3 FragPos;
in vec3 Normal;
out vec4 frag_colour;

uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform mat4 viewMatrix;

const float AMBIENT_STRENGTH = 0.3;
const float SPECULAR_STRENGTH = 0.5;
const float SHININESS = 32.0;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 viewPos = -transpose(mat3(viewMatrix)) * vec3(viewMatrix[3]);
    vec3 viewDir = normalize(viewPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), SHININESS);

    vec3 result = (AMBIENT_STRENGTH + diff) * lightColor * objectColor
                + SPECULAR_STRENGTH * spec * lightColor;

    frag_colour = vec4(result, 1.0);
}
