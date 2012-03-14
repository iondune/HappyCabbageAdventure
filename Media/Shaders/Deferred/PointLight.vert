

// Vertex input
attribute vec3 aPosition;

// Transformation
uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

uniform vec3 uLightPosition;

// Tex Coord
varying vec4 gPosition;

// Deferred Values
varying vec3 vLightPosition;


void main()
{
    vLightPosition = uLightPosition;
    gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1);
    gPosition = gl_Position;
}
