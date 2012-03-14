

// Deferred Values
varying vec4 vPosition;
varying vec3 vNormal;

// Deferred Output
layout(location = 0) out vec3 outColor;
layout(location = 1) out float outDepth;
layout(location = 2) out vec3 outNormal;

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
    outColor = uMaterial.DiffuseColor;
    outDepth = (gl_FragCoord.z)/100.0;
    outNormal = vNormal/2.0 + 0.5;
}
