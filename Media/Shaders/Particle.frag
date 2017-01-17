#version 120

uniform sampler2D uTexColor;

varying vec4 vColor;

void main()
{
   vec4 color = texture2D(uTexColor, gl_PointCoord);
/*
   if(color.r < 0.1) {
      gl_FragColor = vec4(0.0);
      //discard;
   }
*/
   gl_FragColor = /*gl_FragColor +*/ vec4(vColor.xyz, color.r / 1.0);
}
