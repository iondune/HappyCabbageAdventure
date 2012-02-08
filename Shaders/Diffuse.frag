varying vec4 vColor;
varying vec3 vNormal;

uniform int uLightCount;

struct SMaterial
{
    vec3 AmbientColor;
    vec3 DiffuseColor;
};

uniform SMaterial uMaterial;

varying vec3 vLight[4];
varying vec3 vLightColor[4];

void main()
{
    vec3 vDiffuse = vec3(0, 0, 0);
    for (int i = 0; i < 4 && i < uLightCount; ++ i)
        vDiffuse += uMaterial.DiffuseColor * clamp(dot(normalize(vNormal), normalize(vLight[i])), 0.0, 1.0) * vLightColor[i];
    gl_FragColor = vec4(vDiffuse + uMaterial.AmbientColor, 1);
}
