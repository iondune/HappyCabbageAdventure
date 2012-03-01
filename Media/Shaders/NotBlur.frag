uniform sampler2D uTexColor;

varying vec2 vTexCoord;
varying vec2 vTexCoord2;

void main()
{
    float blurSize = 5.5;
    vec3 color = texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y)).xyz;
    gl_FragColor = vec4(color, 1.0);
}
