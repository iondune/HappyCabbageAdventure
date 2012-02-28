uniform sampler2D uTexColor;

varying vec4 vColor;
varying vec3 vLight;
varying vec3 vNormal;
varying vec2 vTexCoord;

void main()
{
    const vec3 AmbientColor = vec3(1.0, 1.0, 1.0);
    const vec3 DiffuseColor = vec3(1.0, 1.0, 1.0);

    vec3 vDiffuse = DiffuseColor * clamp(dot(vNormal, vLight), 0.0, 1.0);
    gl_FragColor = vec4(vDiffuse + AmbientColor, 1)  * texture2D(uTexColor, vTexCoord);
}
