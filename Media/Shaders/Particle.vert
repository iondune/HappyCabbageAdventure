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
   gl_Position = uProjMatrix*uViewMatrix*uModelMatrix * vec4(aPosition, 1.);
   vColor = vec4(aColor, 1.);

   //aPosition is the center of the particle.
   if(aPosition.x == -50.0 && aPosition.y == -50.0 && aPosition.z == -50.0) {
      gl_PointSize = 0.0;
   }
   else {
      gl_PointSize = aPointSize * 10.0 / gl_Position.z;
   }
}
