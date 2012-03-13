uniform sampler2D uSceneColor;
uniform sampler2D uLightPass;

varying vec2 vTexCoord;

void main()
{
    gl_FragColor = uSceneColor + uLightPass;
}
