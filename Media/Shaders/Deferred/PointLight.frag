

// Textures
uniform sampler2D uNormal;
uniform sampler2D uPosition;

// Tex Coord
varying vec4 gPosition;
uniform mat4 uInvProjMatrix;

// Deferred Values
varying vec3 vLightPosition;


void main()
{
    const vec3 AmbientValue = vec3(0.2, 0.2, 0.2);
    const vec3 DiffuseColor = vec3(0.6, 0.6, 0.6);
    const float Radius = 3.f;
    
    vec2 vTexCoord = (gPosition.xy / gPosition.w + 1.0) / 2.0;
    
    vec4 Position = texture2D(uPosition, vTexCoord);/*
    Position.xy = gl_FragCoord.xy;
    Position.z = texture2D(uDepth, vTexCoord).r;
    Position.w = 1.0;*/
    
    //Position = uInvProjMatrix * Position;
    //Position /= Position.w;
    
    //gl_FragColor = vec4(vLightPosition, 1.0)/100.0;
    //return;
    
    vec3 LightVector = vLightPosition - Position.xyz;
    
    const float Distance = length(LightVector);
    
    //if (Distance > Radius)
    //    discard;
        
    LightVector = normalize(LightVector);
    
    const float Attenuation = 1.0;// - Distance / Radius;
    
    vec3 Normal = texture2D(uNormal, vTexCoord).rgb * 2.0 - 1.0;
    
    
    vec3 DiffuseValue = DiffuseColor * clamp(dot(Normal, LightVector), 0.0, 1.0);
    
    gl_FragColor = vec4((DiffuseValue + AmbientValue) * Attenuation, 1);
}