#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 leafColor;

const int LAST_TRUNK_VERTEX = 36443;
const vec3 TRUNK_COLOR = vec3(0.40, 0.26, 0.13);

out vec3 vertexColor;
out vec3 normal;

void main()
{
    vertexColor = gl_VertexID > LAST_TRUNK_VERTEX ? leafColor : TRUNK_COLOR;
    normal = normalize(mat3(modelMatrix) * aNormal);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
}
