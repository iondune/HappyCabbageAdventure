varying vec3 vColor;
varying vec2 vTexCoord;

uniform sampler2D uTexColor;
varying vec3 vNormal, vLightDir;

void main()
{
    //gl_FragColor = texture2D(uTexColor, vTexCoord);

    // Defining The Material Colors
    const vec4 AmbientColor = vec4(0.4, 0.4, 0.4, 1.0);
    const vec4 DiffuseColor = vec4(1.0, 1.0, 1.0, 1.0);

    // Scaling The Input Vector To Length 1
    vec3 normalized_normal = normalize(vNormal);
    vec3 normalized_vertex_to_light_vector = normalize(vLightDir);

    // Calculating The Diffuse Term And Clamping It To [0;1]
    float DiffuseTerm = clamp(dot(normalized_normal, normalized_vertex_to_light_vector), 0.0, 1.0);

    // Calculating The Final Color
    gl_FragColor = (AmbientColor + DiffuseColor * DiffuseTerm) * texture2D(uTexColor, vTexCoord);
}
