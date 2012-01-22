attribute vec3 aPosition;
attribute vec3 aColor;

varying vec3 vColor;

void main()
{
  gl_Position = gl_ModelViewProjectionMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1);
  gl_FrontColor = vec4(aColor.r, aColor.g, aColor.b, 1.0);
  vColor = vec3(aColor.r, aColor.g, aColor.b);
}
