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

uniform SLight uLights[4];

varying vec4 vColor;
varying vec3 vLight[4];
varying vec3 vLightColor[4];
varying vec3 vNormal;

void main()
{
    vec4 vPosition;

    vPosition = uModelMatrix * vec4(aPosition, 1);
    for (int i = 0; i < 4 && i < uLightCount; ++ i)
    {
        vLight[i] = normalize(uLights[i].Position - vec3(vPosition));
        vLightColor[i] = uLights[i].Color;
    }
    gl_Position = uProjMatrix * uViewMatrix * vPosition;

    vNormal = normalize(vec3(uNormalMatrix * vec4(aNormal, 1)));
}
