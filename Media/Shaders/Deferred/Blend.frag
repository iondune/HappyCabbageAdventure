uniform sampler2D uSceneColor;
uniform sampler2D uLightPass;

varying vec2 vTexCoord;

void main()
{
    float ToonLevels = 8.0;
    
    vec4 FinalColor = texture2D(uLightPass, vTexCoord);
    
    gl_FragColor = texture2D(uSceneColor, vTexCoord) * FinalColor;
}
