attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec3 aColor;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;

varying vec3 vColor;
varying vec3 vNormal, vLightDir;

void main()
{
    gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1);
    gl_FrontColor = vec4(aColor, 1.0);

    vColor = vec3(aColor.r, aColor.g, aColor.b);
    vNormal = vec3(uNormalMatrix * vec4(aNormal, 0.0));
    vec3 vertex = vec3(uModelMatrix * vec4(aPosition, 1));
    vLightDir = vec3(vec3(1,10,5) - vertex);
    //vEyeVec = -vertex;
}
