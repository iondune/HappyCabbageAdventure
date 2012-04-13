uniform sampler2D uTexColor;

varying vec4 vColor;
varying vec3 vPosition;

void main()
{
   vec4 color = texture2D(uTexColor, gl_PointCoord);
   if(color.r < 0.1) {
      discard;
   }
   gl_FragData[0] = vec4(vColor.xyz, color.r / 1.0);
   gl_FragData[1] = vec4(vPosition, 1.0); 
   //gl_FragData[2] = vec4(1.0, 1.0, 1.0, 1.0);
   gl_FragData[2] = vec4(0.5, 0.5, 1.0, 1.0);
}
