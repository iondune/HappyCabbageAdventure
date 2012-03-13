uniform sampler2D uSceneColor;
uniform sampler2D uLightPass;

varying vec2 vTexCoord;

void main()
{
    gl_FragColor = texture2D(uSceneColor, vTexCoord) + texture2D(uLightPass, vTexCoord);
}
