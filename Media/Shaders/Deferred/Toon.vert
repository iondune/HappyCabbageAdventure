

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
varying vec3 vPosition;
varying vec3 vNormal;

void main()
{
    // Calculate world coordinate
    vec4 Position = uModelMatrix * vec4(aPosition, 1.0);
    
    // Save for deferred output
    vPosition = Position.xyz;
    
    // Calculate screenspace coordinate
    gl_Position = uProjMatrix * uViewMatrix * Position;
    
    
    // Pass down the normal (world space)
    vNormal = normalize(vec3(uNormalMatrix * vec4(aNormal, 1)));
}
