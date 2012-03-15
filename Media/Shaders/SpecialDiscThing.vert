attribute vec3 aPosition;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

uniform float cutoff;
uniform float start;

varying float vCutoff;
varying float vStart;

varying vec4 gPosition;

void main()
{
    const float left = -0.5;
    const float right = 0.5;
    
    vStart = uProjMatrix * uViewMatrix * uModelMatrix * vec4(left + start * (right - left), aPosition.y, aPosition.z, 1);
    vCutoff = uProjMatrix * uViewMatrix * uModelMatrix * vec4(left + cutoff * (right - left), aPosition.y, aPosition.z, 1);
    gl_Position = gPosition = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1);
}
