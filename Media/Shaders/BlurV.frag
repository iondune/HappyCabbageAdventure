uniform sampler2D uTexColor;

void main()
{
    float blurSize = 0.003;
    vec2 vTexCoord = gl_TexCoord[0].st;
    vec3 color = (texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y - 4.0*blurSize))*0.05 +
                  texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y - 3.0*blurSize))*0.1 +
                  texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y - 2.0*blurSize))*0.2 +
                  texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y - 1.0*blurSize))*0.4 +
                  texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y))*1.0 +
                  texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y + 1.0*blurSize))*0.4 +
                  texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y + 2.0*blurSize))*0.2 +
                  texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y + 3.0*blurSize))*0.1 +
                  texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y + 4.0*blurSize))*0.05
                 ).xyz / 2.5;
    gl_FragColor = vec4(color, 1.0);
}
