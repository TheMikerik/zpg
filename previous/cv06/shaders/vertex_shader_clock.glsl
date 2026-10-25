#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float hourAngle;
uniform float minuteAngle;
uniform float secondAngle;

out vec3 vertexColor;

mat4 rotateClockwise(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return mat4( c,  -s,   0.0, 0.0,
                 s,   c,   0.0, 0.0,
                 0.0, 0.0, 1.0, 0.0,
                 0.0, 0.0, 0.0, 1.0);
}

void main()
{
    mat4 handRotation = mat4(1.0);
    if (distance(aColor, vec3(0.0, 0.0, 1.0)) < 0.1) {
        handRotation = rotateClockwise(hourAngle);
    } else if (distance(aColor, vec3(0.0, 1.0, 0.0)) < 0.1) {
        handRotation = rotateClockwise(minuteAngle);
    } else if (distance(aColor, vec3(1.0, 0.0, 0.0)) < 0.1) {
        handRotation = rotateClockwise(secondAngle);
    }

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * handRotation * vec4(aPos, 1.0);
    vertexColor = aColor;
}
