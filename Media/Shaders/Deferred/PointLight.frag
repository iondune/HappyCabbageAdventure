

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
    const float Shininess = 20.0;
    
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
    
    // Toon!
    float diffuse = 0.0;
    float specular = 0.0;
    vec3 N = normalize(Normal);
    vec3 L = normalize(LightVector);
    vec3 H = normalize(L + Eye);
    
    float df = Attenuation*max(0.0, dot(N, L));
    float sf = Attenuation*max(0.0, dot(N, H));
    sf = pow(sf, Shininess);

    const float A = 0.1;
    const float B = 0.3;
    const float C = 0.6;
    const float D = 1.1;
    float E = fwidth(df);

    if      (df > A - E && df < A + E) diffuse += stepmix(A, B, E, df);
    else if (df > B - E && df < B + E) diffuse += stepmix(B, C, E, df);
    else if (df > C - E && df < C + E) diffuse += stepmix(C, D, E, df);
    else if (df < A) diffuse += 0.0;
    else if (df < B) diffuse += B;
    else if (df < C) diffuse += C;
    else diffuse += D;

    E = fwidth(sf);
    if (sf > 0.5 - E && sf < 0.5 + E)
    {
        specular += clamp(0.5 * (sf - 0.5 + E) / E, 0.0, 1.0);
    }
    else
    {
        specular += step(0.5, sf);
    }
    
    
    
    //vec3 Reflection = reflect(LightVector, Normal);
    //vec3 DiffuseValue = DiffuseColor * clamp(dot(Normal, LightVector), 0.0, 1.0) * uColor;
    //vec3 SpecularValue = SpecularColor * pow(clamp(dot(normalize(Eye), normalize(Reflection)), 0.0, 1.0), Shininess) * uColor;
    
    vec3 DiffuseValue = diffuse * DiffuseColor * uColor;
    vec3 SpecularValue = SpecularColor * specular * uColor;
    gl_FragColor = vec4((DiffuseValue + AmbientValue + SpecularValue) * Attenuation, 1);
}