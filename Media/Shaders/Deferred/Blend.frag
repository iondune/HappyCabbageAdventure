uniform sampler2D uSceneColor;
uniform sampler2D uLightPass;

varying vec2 vTexCoord;

void main()
{
    vec4 FinalColor = texture2D(uSceneColor, vTexCoord) * texture2D(uLightPass, vTexCoord);
    FinalColor *= 10.0;
    FinalColor.x = round(FinalColor.x);
    FinalColor.y = round(FinalColor.y);
    FinalColor.z = round(FinalColor.z);
    
    FinalColor /= 10.0;
    gl_FragColor = FinalColor;
}
