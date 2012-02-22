uniform sampler2D uTexColor;

varying vec4 vColor;
varying vec3 vLight;
varying vec3 vNormal;
varying vec2 vTexCoord;

void main()
{
    const vec3 AmbientColor = vec3(0.8, 0.8, 0.8);
    const vec3 DiffuseColor = vec3(0.8, 0.8, 0.8);

    vec3 vDiffuse = DiffuseColor * clamp(dot(vNormal, vLight), 0.0, 1.0);
    vec4 texCol = texture2D(uTexColor, vTexCoord);
    if (texCol.rgb == vec3(240.0/255.0, 203/255.0, 0))
        texCol = vec4(0, 0.6, 0.9, 1);
    gl_FragColor = vec4(vDiffuse + AmbientColor, 1)  * texCol;
}
