varying vec4 vColor;
varying vec3 vNormal;

uniform int uLightCount;

struct SMaterial
{
    vec3 AmbientColor;
    vec3 DiffuseColor;
    float Shininess;
};

uniform SMaterial uMaterial;

varying vec3 vLight[10];
varying vec3 vLightColor[10];
varying vec3 vEye[10];

void main()
{
    const vec3 SpecularColor = vec3(1, 1, 1);

    vec3 nNormal = normalize(vNormal);

    vec3 vDiffuse = vec3(0, 0, 0);
    vec3 vSpecular = vec3(0, 0, 0);
    for (int i = 0; i < 10 && i < uLightCount; ++ i)
    {
        vec3 nLight = normalize(vLight[i]);

        vec3 vReflection = reflect(-nLight, nNormal);//-nLight + 2 * dot(nNormal, nLight) * nNormal;
        vDiffuse += uMaterial.DiffuseColor * clamp(dot(nNormal, nLight), 0.0, 1.0) * vLightColor[i];
        vSpecular += SpecularColor * pow(clamp(dot(normalize(vEye[i]), normalize(vReflection)), 0.0, 1.0), uMaterial.Shininess) * vLightColor[i];
    }

    gl_FragColor = vec4(vDiffuse + vSpecular + uMaterial.AmbientColor, 1);
}
