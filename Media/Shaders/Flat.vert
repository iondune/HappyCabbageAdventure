attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;
uniform int uLightCount;

struct SLight
{
    vec3 Position;
    vec3 Color;
};

uniform SLight uLights[10];

struct SMaterial
{
    vec3 AmbientColor;
    vec3 DiffuseColor;
};

uniform SMaterial uMaterial;

varying vec4 vColor;

void main()
{
    const vec3 LightPosition = vec3(1, 2, 1);

    vec4 vPosition;
    vec3 vLight[10];
    vec3 vLightColor[10];
    vec3 vNormal;

    vPosition = uModelMatrix * vec4(aPosition, 1);
    for (int i = 0; i < 10 && i < uLightCount; ++ i)
    {
        vLight[i] = normalize(uLights[i].Position - vec3(vPosition));
        vLightColor[i] = uLights[i].Color;
    }
    gl_Position = uProjMatrix * uViewMatrix * vPosition;

    vNormal = normalize(vec3(uNormalMatrix * vec4(aNormal, 1)));

    vec3 vDiffuse = vec3(0, 0, 0);
    for (int i = 0; i < 10 && i < uLightCount; ++ i)
        vDiffuse += uMaterial.DiffuseColor * clamp(dot(vNormal, vLight[i]), 0.0, 1.0) * vLightColor[i];
    vColor = vec4(vDiffuse + uMaterial.AmbientColor, 1);
}
