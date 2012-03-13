

// Textures
uniform smapler2D uPosition;
uniform sampler2D uNormal;

// Tex Coord
varying vec2 vTexCoord;

// Deferred Values
varying vec3 vLightPosition;

void main()
{
    const vec3 AmbientValue = vec3(0.2, 0.2, 0.2);
    const vec3 DiffuseColor = vec3(0.6, 0.6, 0.6);
    const float Radius = 3.f;
    
    
    vec3 Position = texture2D(uPosition, vTexCoord).xyz;
    LightVector = vLightPosition - Position;
    
    const float Distance = length(LightVector);
    
    if (Distance > Radius)
        discard;
    
    float const Attenuation = 1.0 - Distance / Radius;
    
    vec3 Normal = texture2D(uNormal, vTexCoord).xyz;
    
    
    vec3 DiffuseValue = DiffuseColor * clamp(dot(Normal, LightVector), 0.0, 1.0);
    
    gl_FragColor = vec4((Diffuse + AmbientValue) * Attenuation, 1);
}
