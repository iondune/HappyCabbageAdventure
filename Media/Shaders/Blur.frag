uniform sampler2D uTexColor;

varying vec2 vTexCoord;
varying vec2 vTexCoord2;

void main()
{
    float blurSize = 0.5;
    vec3 color = (texture2D(uTexColor, vec2(vTexCoord.x - 4.0*blurSize, vTexCoord.y)) +
                  texture2D(uTexColor, vec2(vTexCoord.x - 2.0*blurSize, vTexCoord.y)) +
                  texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y)) +
                  texture2D(uTexColor, vec2(vTexCoord.x + 2.0*blurSize, vTexCoord.y)) +
                  texture2D(uTexColor, vec2(vTexCoord.x + 4.0*blurSize, vTexCoord.y))).xyz / 4.0;
    gl_FragColor = vec4(color, 1.0);
}
