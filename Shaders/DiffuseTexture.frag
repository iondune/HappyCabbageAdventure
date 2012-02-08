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
    gl_FragColor = vec4(vDiffuse + AmbientColor, 1)  * texture2D(uTexColor, vTexCoord);
}
