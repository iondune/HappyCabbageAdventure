attribute vec3 aPosition;
attribute vec3 aColor;
attribute vec2 aTexCoord;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

varying vec3 vColor;
varying vec2 vTexCoord;

void main()
{
	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1);
	gl_FrontColor = vec4(aColor.r, aColor.g, aColor.b, 1.0);
	vColor = vec3(aColor.r, aColor.g, aColor.b);
	vTexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
