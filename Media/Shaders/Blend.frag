uniform sampler2D scene;
uniform sampler2D ssao;

varying vec2 uv;

void main()
{
    gl_FragColor = texture2D(scene, uv)*texture2D(ssao, uv).r;
}
