

// Textures
uniform sampler2D uNormal;
uniform sampler2D uPosition;

uniform float uRadius;
uniform vec3 uColor;
uniform mat4 uViewMatrix;

// Tex Coord
varying vec4 gPosition;
uniform mat4 uInvProjMatrix;

// Deferred Values
varying vec3 vLightPosition;

float stepmix(float edge0, float edge1, float E, float x)
{
    float T = clamp(0.5 * (x - edge0) / E, 0.0, 1.0);
    return mix(edge0, edge1, T);
}

void main()
{
    const vec3 AmbientValue = vec3(0.2, 0.2, 0.2);
    const vec3 DiffuseColor = vec3(0.6, 0.6, 0.6);
    const vec3 SpecularColor = vec3(1.8, 1.8, 1.8);
    const float Shininess = 0.80;
    
    vec2 vTexCoord = (gPosition.xy / gPosition.w + 1.0) / 2.0;
    
    vec4 Position = texture2D(uPosition, vTexCoord);
    vec3 Eye = vec3(uViewMatrix * Position) * 1.0;
    
    vec3 LightVector = vLightPosition - Position.xyz;
    
    float Distance = length(LightVector);
    
    if (Distance > uRadius)
        discard;
        
    LightVector = normalize(LightVector);
    
    float Attenuation = 1.0 - Distance / uRadius;
    
    vec3 Normal = texture2D(uNormal, vTexCoord).rgb * 2.0 - 1.0;
    
    vec3 Reflection = reflect(LightVector, Normal);
    vec3 DiffuseValue = DiffuseColor * clamp(dot(Normal, LightVector), 0.0, 1.0) * uColor;
    vec3 SpecularValue = SpecularColor * pow(clamp(dot(normalize(Eye), normalize(Reflection)), 0.0, 1.0), Shininess) * uColor;
    
    gl_FragColor = vec4((DiffuseValue + AmbientValue + SpecularValue) * Attenuation, 1);
}
