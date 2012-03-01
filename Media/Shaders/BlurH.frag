uniform sampler2D uTexColor;

varying vec2 vTexCoord;
varying vec2 vTexCoord2;

void main()
{
    float blurSize = 0.003;
    vec3 color = (texture2D(uTexColor, vec2(vTexCoord.x - 4.0*blurSize, vTexCoord.y))*0.05 +
                  texture2D(uTexColor, vec2(vTexCoord.x - 3.0*blurSize, vTexCoord.y))*0.1 +
                  texture2D(uTexColor, vec2(vTexCoord.x - 2.0*blurSize, vTexCoord.y))*0.2 +
                  texture2D(uTexColor, vec2(vTexCoord.x - 1.0*blurSize, vTexCoord.y))*0.4 +
                  texture2D(uTexColor, vec2(vTexCoord.x, vTexCoord.y))*1.0 +
                  texture2D(uTexColor, vec2(vTexCoord.x + 1.0*blurSize, vTexCoord.y))*0.4 +
                  texture2D(uTexColor, vec2(vTexCoord.x + 2.0*blurSize, vTexCoord.y))*0.2 +
                  texture2D(uTexColor, vec2(vTexCoord.x + 3.0*blurSize, vTexCoord.y))*0.1 +
                  texture2D(uTexColor, vec2(vTexCoord.x + 4.0*blurSize, vTexCoord.y))*0.05
                 ).xyz / 2.5;
    gl_FragColor = vec4(color + 0.1, 1.0);
}
