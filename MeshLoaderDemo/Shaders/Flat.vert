attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

varying vec4 vColor;

void main()
{
    const vec3 AmbientColor = vec3(0.4, 0.4, 0.4);
    const vec3 DiffuseColor = vec3(1.0, 1.0, 1.0);
    const vec3 LightPosition = vec3(1, 2, 1);

    vec4 vPosition;
    vec3 vLight;
    vec3 vNormal;

    vPosition = uModelMatrix * vec4(aPosition, 1);
    vLight = normalize(LightPosition - vec3(vPosition));
    gl_Position = uProjMatrix * uViewMatrix * vPosition;

    vNormal = normalize(vec3(uModelMatrix * vec4(aNormal, 0.0)));

    vec3 diffuse = DiffuseColor * clamp(dot(vNormal, vLight), 0.0, 1.0);
    vColor = vec4(diffuse + AmbientColor, 1);
}
