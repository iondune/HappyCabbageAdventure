attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;

varying vec2 vTexCoord;
varying vec2 vTexCoord2;

void main()
{
    vec4 vPosition;

    vPosition = uModelMatrix * vec4(aPosition, 1);
    vTexCoord = vec2(aTexCoord.x, aTexCoord.y);
    vTexCoord2 = vec2(aTexCoord.x+1.0, aTexCoord.y+1.0);

    gl_Position = uProjMatrix * uViewMatrix * vPosition;
}
