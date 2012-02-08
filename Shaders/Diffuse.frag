varying vec4 vColor;
varying vec3 vNormal;

uniform int uLightCount;

struct SMaterial
{
    vec3 AmbientColor;
    vec3 DiffuseColor;
};

uniform SMaterial uMaterial;

varying vec3 vLight[10];
varying vec3 vLightColor[10];

void main()
{
    const vec3 AmbientColor = vec3(0.4, 0.4, 0.4);
    const vec3 DiffuseColor = vec3(0.6, 0.6, 0.6);

    vec3 vDiffuse = vec3(0, 0, 0);
    for (int i = 0; i < 10 && i < uLightCount; ++ i)
        vDiffuse += DiffuseColor * clamp(dot(normalize(vNormal), normalize(vLight[0])), 0.0, 1.0) * vLightColor[0];
    gl_FragColor = vec4(vDiffuse + AmbientColor, 1);
}
