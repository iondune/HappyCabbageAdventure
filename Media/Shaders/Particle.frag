uniform sampler2D uTexColor;

varying vec4 vColor;

void main()
{
   vec4 color = texture2D(uTexColor, gl_PointCoord);
   if(color.r < 0.1) {
      discard;
   }
   gl_FragColor = vColor;
}
