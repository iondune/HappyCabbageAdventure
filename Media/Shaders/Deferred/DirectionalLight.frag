

// Textures
uniform sampler2D uNormal;
uniform sampler2D uPosition;

uniform vec3 uDirection;
uniform vec3 uColor;

// Tex Coord
varying vec4 gPosition;
varying vec2 vTexCoord;

// Deferred Values
varying vec3 vLightPosition;


void main()
{
    const vec3 AmbientValue = vec3(0.2, 0.2, 0.2);
    const vec3 DiffuseColor = vec3(0.6, 0.6, 0.6);
    
    vec2 vTexCoord = (gPosition.xy / gPosition.w + 1.0) / 2.0;
    
    vec4 Position = texture2D(uPosition, vTexCoord);
    
    vec3 LightVector = uDirection;

    LightVector = normalize(LightVector);
    
    vec3 Normal = texture2D(uNormal, vTexCoord).rgb * 2.0 - 1.0;
    
    vec3 DiffuseValue = DiffuseColor * clamp(dot(Normal, LightVector), 0.0, 1.0);
    
    gl_FragColor = vec4((DiffuseValue + AmbientValue) * uColor, 1);
}