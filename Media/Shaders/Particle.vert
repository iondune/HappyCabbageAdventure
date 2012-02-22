attribute vec3 aPosition; //aPosition = center of point
attribute vec3 aColor;
attribute float aPointSize;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;

varying vec4 vColor;

void main()
{
   //aPosition is the center of the particle.

   gl_PointSize = aPointSize;
// * gl_Position.w;
   vColor = vec4(aColor, 1.);
   gl_Position = uProjMatrix*uViewMatrix*uModelMatrix * vec4(aPosition, 1.);
}
