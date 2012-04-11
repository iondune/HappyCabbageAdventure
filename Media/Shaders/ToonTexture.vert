attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;

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

varying vec3 EyespaceNormal;
varying vec3 Eye;
varying vec3 LightPosition;
varying vec3 vLight[4];
varying vec3 vLightColor[4];
varying vec3 vEye;
varying float vAttenuation[4];
varying vec2 vTexCoord;

void main()
{
    vec4 vPosition;

    vPosition = uModelMatrix * vec4(aPosition, 1);
    LightPosition = normalize(uLights[0].Position - vec3(vPosition));
    vTexCoord = vec2(aTexCoord.x, aTexCoord.y);

    vec3 vtl[4]; 
    for (int i = 0; i < 4 && i < uLightCount; ++ i)
    {
        vLight[i] = normalize(uLights[i].Position - vec3(vPosition));
        vLightColor[i] = uLights[i].Color;

        vtl[i] = uLights[i].Position - vPosition.xyz;
        vAttenuation[i] = 1.0; 
        //vAttenuation[i] = clamp(400.0/(vtl[i].x + vtl[i].y + vtl[i].z), 0.0, 1.0);
    }

    vEye = vec3(uViewMatrix * vPosition) * -1.0;
    
    Eye = vec3(uViewMatrix * vPosition) * -1.0;

    gl_Position = uProjMatrix * uViewMatrix * vPosition;

    EyespaceNormal = normalize(vec3(uNormalMatrix * vec4(aNormal, 1)));
}
