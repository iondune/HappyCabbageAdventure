// Fragment.GL2

varying vec3 EyespaceNormal;
varying vec3 Eye;
varying vec3 LightPosition;
varying vec3 vLight[4];
varying vec3 vLightColor[4];
varying vec3 vEye[4];
uniform int uLightCount;

//uniform vec3 LightPosition;
//uniform vec3 uMaterial.AmbientColor;
//uniform vec3 uMaterial.SpecularColor;
//uniform float uMaterial.Shininess;

struct SMaterial
{
    vec3 SpecularColor;
    vec3 AmbientColor;
    vec3 DiffuseColor;
    float Shininess;
};

uniform SMaterial uMaterial;

float stepmix(float edge0, float edge1, float E, float x)
{
    float T = clamp(0.5 * (x - edge0) / E, 0.0, 1.0);
    return mix(edge0, edge1, T);
}

void main()
{
  float diffuse = 0.0;
  float specular = 0.0;

  for (int i = 0; i < 4 && i < uLightCount; ++ i) 
  {
    vec3 N = normalize(EyespaceNormal);
    vec3 L = normalize(vLight[i]);
    vec3 H = normalize(L + Eye);
    
    float df = max(0.0, dot(N, L));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, uMaterial.Shininess);

    const float A = 0.1;
    const float B = 0.3;
    const float C = 0.6;
    const float D = 1.0;
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
  }

    vec3 SpecularColor = vec3(1, 1, 1);
    vec3 color = uMaterial.AmbientColor + diffuse * uMaterial.DiffuseColor + specular * SpecularColor; //vec3
    gl_FragColor = vec4(color, 1.0);
}
