

// Deferred Values
varying vec3 vPosition;
varying vec3 vNormal;

struct SMaterial
{
    vec3 SpecularColor;
    vec3 AmbientColor;
    vec3 DiffuseColor;
    float Shininess;
};

uniform SMaterial uMaterial;

void main()
{
    gl_FragData[0] = vec4(uMaterial.DiffuseColor, 1.0);
    gl_FragData[1] = vec4(vPosition, 1.0);
    gl_FragData[2] = vec4(vNormal/2.0 + 0.5, 1.0);
}
