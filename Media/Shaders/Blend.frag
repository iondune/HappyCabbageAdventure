uniform sampler2D scene;
uniform sampler2D ssao;
uniform sampler2D bloom;

varying vec2 uv;

void main()
{
    vec4 color = texture2D(bloom, uv);
    vec4 color2 = texture2D(scene, uv)*texture2D(ssao, uv).r;
    if(color.g > 0.1 && color.g < 1.0) {
       gl_FragColor = color;
    }
    else {
       gl_FragColor = color2; 
    }
}
