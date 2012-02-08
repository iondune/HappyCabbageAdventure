varying vec4 vColor;
varying vec3 vLight;
varying vec3 vNormal;

struct SMaterial
{
    vec3 AmbientColor;
    vec3 DiffuseColor;
};

uniform SMaterial uMaterial;

void main()
{
    const vec3 AmbientColor = vec3(0.4, 0.4, 0.4);
    const vec3 DiffuseColor = vec3(0.6, 0.6, 0.6);

    vec3 vDiffuse = DiffuseColor * clamp(dot(normalize(vNormal), normalize(vLight)), 0.0, 1.0);
    gl_FragColor = vec4(vDiffuse + AmbientColor, 1);
}
