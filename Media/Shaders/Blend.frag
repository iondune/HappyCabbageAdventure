uniform sampler2D scene;
uniform sampler2D ssao;
uniform sampler2D bloom;

varying vec2 uv;

void main()
{
   vec4 color = texture2D(bloom, uv);
   vec4 color2 = texture2D(scene, uv)*texture2D(ssao, uv).r;
//Don't do bloom
   if(color.r == 1.0 && color.g == 0.0 && color.b == 1.0) {
      gl_FragColor = color2;
   }
//Do bloom (brighter color)
   else if(color.r > color2.r && color.g > color2.g && color.b > color2.b) {
      gl_FragColor = color;
   }
//Do bloom (darker color)
   else {
      gl_FragColor = color2;
   }
}
