

// Vertex input
attribute vec3 aPosition;

// Transformation
uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

// Tex Coord
varying vec2 vTexCoord;

// Deferred Values
varying vec3 vLightPosition;

void main()
{
    vec4 Position = uModelMatrix * vec4(aPosition, 1);
    vLightPosition = Position.xyz;
    gl_Position = uProjMatrix * uViewMatrix * Position;
    vTexCoord = (gl_Position.xy + 1.0) / 2.0;
}
