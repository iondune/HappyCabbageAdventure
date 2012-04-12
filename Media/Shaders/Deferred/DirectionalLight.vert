

// Vertex input
attribute vec3 aPosition;

// Transformation
uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

// Tex Coord
varying vec4 vTexCoord;


void main()
{
    vTexCoord = gl_MultiTexCoord0;
    gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
}
