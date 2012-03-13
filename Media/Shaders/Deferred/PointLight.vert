

// Vertex input
attribute vec3 aPosition;

// Transformation
uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

uniform vec3 uPosition;

// Tex Coord
varying vec4 gPosition;

// Deferred Values
varying vec3 vLightPosition;

void main()
{
    vec4 Position = uModelMatrix * vec4(aPosition, 1);
    vLightPosition = (uModelMatrix * vec4(uPosition, 1)).xyz;
    gl_Position = uProjMatrix * uViewMatrix * Position;
    gPosition = gl_Position;
}
