// Vertex.GL2

attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uProjMatrix;
uniform mat4 uModelMatrix;
uniform mat3 uNormalMatrix;
//uniform vec3 uMaterial.DiffuseColor;
struct SMaterial
{
    vec3 SpecularColor;
    vec3 AmbientColor;
    vec3 DiffuseColor;
    float Shininess;
};

uniform SMaterial uMaterial;

varying vec3 EyespaceNormal;
varying vec3 Diffuse;

void main()
{
    vec4 newPosition = vec4(aPosition, 1);
    EyespaceNormal = uNormalMatrix * aNormal;
    gl_Position = uProjMatrix * uModelMatrix * newPosition;
    Diffuse = uMaterial.DiffuseColor;
}