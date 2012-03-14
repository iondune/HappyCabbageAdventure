

// Textures
uniform sampler2D uNormal;
uniform sampler2D uPosition;

uniform float uRadius;

// Tex Coord
varying vec4 gPosition;
uniform mat4 uInvProjMatrix;

// Deferred Values
varying vec3 vLightPosition;


void main()
{
    const vec3 AmbientValue = vec3(0.2, 0.2, 0.2);
    const vec3 DiffuseColor = vec3(0.6, 0.6, 0.6);
    
    vec2 vTexCoord = (gPosition.xy / gPosition.w + 1.0) / 2.0;
    
    vec4 Position = texture2D(uPosition, vTexCoord);
    
    vec3 LightVector = vLightPosition - Position.xyz;
    
    const float Distance = length(LightVector);
    
    if (Distance > uRadius)
        discard;
        
    LightVector = normalize(LightVector);
    
    const float Attenuation = 1.0 - Distance / uRadius;
    
    vec3 Normal = texture2D(uNormal, vTexCoord).rgb * 2.0 - 1.0;
    
    
    vec3 DiffuseValue = DiffuseColor * clamp(dot(Normal, LightVector), 0.0, 1.0);
    
    gl_FragColor = vec4((DiffuseValue + AmbientValue) * Attenuation, 1);
}