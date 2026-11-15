#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 vertexColor;
in vec2 uv;
out vec4 frag_colour;

uniform vec3 objectColor;
uniform int useVertexColor;     
uniform int useTexture;
uniform sampler2D textureUnitID;

uniform int lightCount;
uniform vec3 lightPos;
uniform vec3 attenuation;       

uniform int shadingModel;       
uniform float shininess;
uniform int specularOnBackSide; 
uniform mat4 viewMatrix;

const vec3 LIGHT_COLOR = vec3(1.0, 1.0, 1.0);
const float AMBIENT_STRENGTH = 0.3;
const float SPECULAR_STRENGTH = 0.5;
const float SHININESS = 32.0;

void main()
{
    vec3 baseColor = useVertexColor == 1 ? vertexColor : objectColor;
    if (useTexture == 1) {
        baseColor = texture(textureUnitID, uv).rgb;
    }

    if (lightCount < 1) {
        frag_colour = vec4(baseColor, 1.0);
        return;
    }

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float dist = length(lightPos - FragPos);
    float att = 1.0 / (attenuation.x + attenuation.y * dist + attenuation.z * dist * dist);

    float diff = max(dot(norm, lightDir), 0.0);

    float spec = 0.0;
    if (shadingModel != 0 && (diff > 0.0 || specularOnBackSide == 1)) {
        vec3 viewPos = -transpose(mat3(viewMatrix)) * vec3(viewMatrix[3]);
        vec3 viewDir = normalize(viewPos - FragPos);

        float exponent = shininess > 0.0 ? shininess : SHININESS;

        if (shadingModel == 1) {
            vec3 reflectDir = reflect(-lightDir, norm);
            spec = pow(max(dot(viewDir, reflectDir), 0.0), exponent);
        } else {
            vec3 halfDir = normalize(lightDir + viewDir);
            spec = pow(max(dot(norm, halfDir), 0.0), exponent);
        }
    }

    vec3 result = AMBIENT_STRENGTH * baseColor
                + att * (diff * baseColor + SPECULAR_STRENGTH * spec) * LIGHT_COLOR;

    frag_colour = vec4(result, 1.0);
}
