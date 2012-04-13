uniform sampler2D uSceneColor;
uniform sampler2D uLightPass;

varying vec2 vTexCoord;

void main()
{
    float ToonLevels = 8.0;
    
    vec4 FinalColor = texture2D(uLightPass, vTexCoord);
    
    FinalColor *= ToonLevels;
    FinalColor.x = round(FinalColor.x);
    FinalColor.y = round(FinalColor.y);
    FinalColor.z = round(FinalColor.z);
    FinalColor /= ToonLevels;
    
    gl_FragColor = texture2D(uSceneColor, vTexCoord) * FinalColor;
}
