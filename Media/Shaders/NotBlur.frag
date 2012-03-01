uniform sampler2D uTexColor;

void main()
{
    gl_FragColor = texture2D(uTexColor, gl_TexCoord[0].st); 
}
