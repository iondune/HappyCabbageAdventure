

// Vertex input
attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;

// Transformation
uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;

// Deferred Values
varying vec4 vPosition;
varying vec3 vNormal;

void main()
{
    // Calculate screenspace coordinate
    vPosition = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
    gl_Position = vPosition;
    
    
    // Pass down the normal (world space)
    vNormal = normalize(vec3(uNormalMatrix * vec4(aNormal, 1)));
}
